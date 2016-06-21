// Copyright 201 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/hle/kernel/event.h"

#include "core/hw/camera.h"
#include <memory>

namespace HW {
namespace Camera {

struct CameraContext {
    Size size;
    Effect effect;
    Flip flip;
};

struct CameraConfig {
    Context current_context = Context::A;
    CameraContext context_A;
    CameraContext context_B;
};

struct PortConfig {
    bool is_capture = false;
    bool is_busy = false;
    u32 image_size;
    u32 trans_unit;
    VAddr dest;
    u16 transfer_lines;
    u16 width;
    u16 height;
    u32 transfer_bytes;
    bool trimming;

    Kernel::SharedPtr<Kernel::Event> completion_event_cam = Kernel::Event::Create(Kernel::ResetType::OneShot, "CAM_U::completion_event_cam");
    Kernel::SharedPtr<Kernel::Event> interrupt_buffer_error_event = Kernel::Event::Create(Kernel::ResetType::OneShot, "CAM_U::interrupt_buffer_error_event");
    Kernel::SharedPtr<Kernel::Event> vsync_interrupt_event = Kernel::Event::Create(Kernel::ResetType::OneShot, "CAM_U::vsync_interrupt_event");

    ~PortConfig() {
        completion_event_cam = nullptr;
        interrupt_buffer_error_event = nullptr;
        vsync_interrupt_event = nullptr;
    }
};


static bool driver_initialized = false;
static u32 transfer_bytes = 5 * 1024;
static bool activated = false;
static bool camera_capture = false;

static FrameRate frame_rate = FrameRate::Rate_15;

static Port         current_port = Port::None;
static std::unique_ptr<PortConfig>     port1;
static std::unique_ptr<PortConfig>     port2;

static CameraSelect current_camera = CameraSelect::None;
static CameraConfig inner;
static CameraConfig outer1;
static CameraConfig outer2;

void SignalVblankInterrupt() {
    if (driver_initialized && activated) {
        static u64 cycles = 0;
        cycles = (cycles + 1) % 60;
        bool signal = false;

        switch (frame_rate) {
        case FrameRate::Rate_30:
        case FrameRate::Rate_30_To_5:
        case FrameRate::Rate_30_To_10:
            signal = (cycles % 2) == 0;
            break;
        case FrameRate::Rate_20:
        case FrameRate::Rate_20_To_5:
        case FrameRate::Rate_20_To_10:
            signal = (cycles % 3) == 0;
            break;
        case FrameRate::Rate_15:
        case FrameRate::Rate_15_To_5:
        case FrameRate::Rate_15_To_2:
        case FrameRate::Rate_15_To_10:
            signal = (cycles % 4) == 0;
            break;
        case FrameRate::Rate_10:
            signal = (cycles % 6) == 0;
            break;
        case FrameRate::Rate_5:
        case FrameRate::Rate_8_5:
            signal = (cycles % 12) == 0;
            break;
        }

        if (signal) {
            switch(current_camera) {
            case CameraSelect::In1:
            case CameraSelect::Out2:
            case CameraSelect::In1Out2:
                port1->vsync_interrupt_event->Signal();
                break;
            case CameraSelect::Out1:
                port2->vsync_interrupt_event->Signal();
                break;
            case CameraSelect::In1Out1:
            case CameraSelect::Out1Out2:
            case CameraSelect::All:
                port1->vsync_interrupt_event->Signal();
                port2->vsync_interrupt_event->Signal();
                break;
            case CameraSelect::None:
                break;
            }
        }
    }
}

PortConfig* GetPort(u8 port) {
    switch (static_cast<Port>(port)) {
    case Port::Cam1:
        return port1.get();
    case Port::Cam2:
        return port2.get();
    default:
        return nullptr;
    }
}

ResultCode StartCapture(u8 port) {
    current_port = static_cast<Port>(port);

    if (port & (u8)Port::Cam1) {
        port1->completion_event_cam->Signal();
        port1->is_capture = true;
    }

    if (port & (u8)Port::Cam2) {
        port2->completion_event_cam->Signal();
        port2->is_capture = true;
    }
    return RESULT_SUCCESS;
}

ResultCode StopCapture(u8 port) {
    if (port & (u8)Port::Cam1) {
        port1->completion_event_cam->Clear();
        port1->is_capture = false;
    }

    if (port & (u8)Port::Cam2) {
        port2->completion_event_cam->Clear();
        port2->is_capture = false;
    }
    return RESULT_SUCCESS;
}

ResultCode IsBusy(u8 port, bool& is_busy) {

    PortConfig* port_config = GetPort(port);
    if(port_config) {
        is_busy = port_config->is_busy;
    }
    return RESULT_SUCCESS;
}

ResultCode ClearBuffer(u8 port) {
    if (port & (u8)Port::Cam1) {
    }

    if (port & (u8)Port::Cam2) {
    }
    return RESULT_SUCCESS;
}

ResultCode GetVsyncInterruptEvent(u8 port, Handle& event) {
    PortConfig* port_config = GetPort(port);
    if (port_config) {
        event = Kernel::g_handle_table.Create(port_config->vsync_interrupt_event).MoveFrom();
    }
    return RESULT_SUCCESS;
}

ResultCode GetBufferErrorInterruptEvent(u8 port, Handle& event) {
    PortConfig* port_config = GetPort(port);
    if (port_config) {
        event = Kernel::g_handle_table.Create(port_config->interrupt_buffer_error_event).MoveFrom();
    }
    return RESULT_SUCCESS;
}

ResultCode SetReceiving(u8 port, VAddr dest, u32 image_size, u16 trans_unit, Handle& event) {
    PortConfig* port_config = GetPort(port);
    if(port_config) {
        port_config->completion_event_cam->Signal();
        event = Kernel::g_handle_table.Create(port_config->completion_event_cam).MoveFrom();
        port_config->image_size = image_size;
        port_config->trans_unit = trans_unit;
        port_config->dest = dest;
    }
    return RESULT_SUCCESS;
}

ResultCode SetTransferLines(u8 port, u16 transfer_lines, u16 width, u16 height) {
    if (port & (u8)Port::Cam1) {
        port1->transfer_lines = transfer_lines;
        port1->width = width;
        port1->height = height;
    }

    if (port & (u8)Port::Cam2) {
        port2->transfer_lines = transfer_lines;
        port2->width = width;
        port2->height = height;
    }
    return RESULT_SUCCESS;
}

ResultCode SetTransferBytes(u8 port, u32 transfer_bytes, u16 width, u16 height) {
    if (port & (u8)Port::Cam1) {
        port1->transfer_bytes = transfer_bytes;
        port1->width = width;
        port1->height = height;
    }

    if (port & (u8)Port::Cam2) {
        port2->transfer_bytes = transfer_bytes;
        port2->width = width;
        port2->height = height;
    }
    return RESULT_SUCCESS;
}

ResultCode GetTransferBytes(u8 port, u32& transfer_bytes) {
    PortConfig* port_config = GetPort(port);
    if (port_config) {
        transfer_bytes = port_config->transfer_bytes;
    }
    return RESULT_SUCCESS;
}

ResultCode SetTrimming(u8 port, bool trimming) {
    if (port & (u8)Port::Cam1) {
        port1->trimming = trimming;
    }

    if (port & (u8)Port::Cam2) {
        port2->trimming = trimming;
    }
    return RESULT_SUCCESS;
}

ResultCode Activate(u8 camera_select) {
    current_camera = static_cast<CameraSelect>(camera_select);
    activated = (current_camera == CameraSelect::None) ? false : true;
    return RESULT_SUCCESS;
}

ResultCode DriverInitialize() {
    activated = false;
    camera_capture = false;
    driver_initialized = true;
    return RESULT_SUCCESS;
}

ResultCode DriverFinalize() {
    activated = false;
    camera_capture = false;
    driver_initialized = false;
    return RESULT_SUCCESS;
}

void Init() {
    port1 = std::make_unique<PortConfig>();
    port2 = std::make_unique<PortConfig>();
}

void Shutdown() {
    port1 = nullptr;
    port2 = nullptr;
}

} // namespace Camera
} // namespace HW
