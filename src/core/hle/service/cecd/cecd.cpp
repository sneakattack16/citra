// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/logging/log.h"

#include <core/hle/kernel/event.h>

#include "core/hle/service/service.h"
#include "core/hle/service/cecd/cecd.h"
#include "core/hle/service/cecd/cecd_s.h"
#include "core/hle/service/cecd/cecd_u.h"

namespace Service {
namespace CECD {

static Kernel::SharedPtr<Kernel::Event> cecinfo_event;

void GetCecInfoBuffer(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    LOG_WARNING(Service_CECD, "(STUBBED) called");
}

void GetCecInfoEventHandle(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[3] = Kernel::g_handle_table.Create(cecinfo_event).MoveFrom(); // Event handle
    cecinfo_event->Signal();
    LOG_WARNING(Service_CECD, "(STUBBED) called");
}

void OpenAndReadFile(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 buf_size = cmd_buff[1];
    u32 title_id = cmd_buff[2];
    u32 data_type = cmd_buff[3];
    u32 option = cmd_buff[4];
    u32 unk_0x20 = cmd_buff[5]; //0x20
    u32 size_shift = cmd_buff[7]; // 16 * buf_size | 0xC
    u32 out_buf = cmd_buff[8];


    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0;

    LOG_WARNING(Service_CECD, "(STUBBED) called");
}

void SetData(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 title_id = cmd_buff[1];
    u32 buf_addr = cmd_buff[2];
    u32 size = cmd_buff[3];
    u32 opt_size = cmd_buff[4];
    u32 size_shift = cmd_buff[5];
    (void)size_shift;

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_CECD, "(STUBBED) called, titleid = 0x%08X, buf = 0x%08X, size = %d, opt_size = %d",
        title_id, buf_addr, size, opt_size);
}

void Init() {
    using namespace Kernel;

    AddService(new CECD_S_Interface);
    AddService(new CECD_U_Interface);

    cecinfo_event = Kernel::Event::Create(RESETTYPE_ONESHOT, "CECD_U::cecinfo_event");
}

void Shutdown() {
}

} // namespace CECD

} // namespace Service
