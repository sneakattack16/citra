// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/logging/log.h"

#include "core/hle/kernel/event.h"
#include "core/hle/service/ac_u.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Namespace AC_U

namespace AC_U {

static const u32 AC_CONFIG_SIZE = 512;

static struct AcConfig {
    u8 unknown[AC_CONFIG_SIZE];
} default_config = {};

static bool ac_connected = false;

static Kernel::SharedPtr<Kernel::Event> close_event = nullptr;
static Kernel::SharedPtr<Kernel::Event> connect_event = nullptr;
static Kernel::SharedPtr<Kernel::Event> disconnect_event = nullptr;

/**
 * AC_U::CreateDefaultConfig service function
 *  Inputs:
 *      64 : AcConfig size << 14 | 2
 *      65 : pointer to AcConfig struct
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
static void CreateDefaultConfig(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 ssize = cmd_buff[64];
    u32 ac_config = cmd_buff[65];

    if(ssize != (AC_CONFIG_SIZE << 14 | 2)) {
        // TODO: figure out right error code
        cmd_buff[1] = -1;
        LOG_ERROR(Service_AC, "(STUBBED) called, wrong AcConfig size");
    }
    else {
        Memory::WriteBlock(ac_config, reinterpret_cast<const u8*>(&default_config), AC_CONFIG_SIZE);
        cmd_buff[1] = RESULT_SUCCESS.raw; // No error
        LOG_WARNING(Service_AC, "(STUBBED) called");
    }
}

/**
 * AC_U::ConnectAsync service function
 *  Inputs:
 *      1 : ProcessId Header (must be 0x20)
 *      3 : Always 0
 *      4 : Event handle, should be signaled when AC connection is closed
 *      5 : Always 0x800402
 *      6 : pointer to AcConfig struct
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
static void ConnectAsync(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    connect_event = Kernel::g_handle_table.Get<Kernel::Event>(cmd_buff[4]);
    if (connect_event) {
        connect_event->name = "AC_U:connect_event";
        connect_event->Signal();
        ac_connected = true;
    }
    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

/**
 * AC_U::GetConnectResult service function
 *  Inputs:
 *      1 : ProcessId Header (must be 0x20)
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
static void GetConnectResult(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

/**
 * AC_U::CloseAsync service function
 *  Inputs:
 *      1 : ProcessId Header (must be 0x20)
 *      3 : Always 0
 *      4 : Event handle, should be signaled when AC connection is closed
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
static void CloseAsync(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();
    close_event = Kernel::g_handle_table.Get<Kernel::Event>(cmd_buff[4]);
    if (close_event) {
        close_event->name = "AC_U:close_event";
        close_event->Signal();
        ac_connected = false;

        if(disconnect_event) {
            disconnect_event->Signal();
        }
    }
    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

/**
 * AC_U::GetCloseResult service function
 *  Inputs:
 *      1 : ProcessId Header (must be 0x20)
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
static void GetCloseResult(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

/**
 * AC_U::GetWifiStatus service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : Output connection type, 0 = none, 1 = Old3DS Internet, 2 = New3DS Internet.
 */
static void GetWifiStatus(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // Connection type set to none

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

/**
 * AC_U::AddDenyApType service function
 *  Inputs:
 *      1 : ApType
 *      2 : AcConfig size << 14 | 2
 *      3 : Input pointer to AcConfig struct
 *      64 : AcConfig size << 14 | 2
 *      65 : Output pointer to AcConfig struct
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : Infra Priority
 */
static void AddDenyApType(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 ap_type = cmd_buff[1];

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_AC, "(STUBBED) called, ap_type=%d", ap_type);
}

/**
 * AC_U::GetInfraPriority service function
 *  Inputs:
 *      1 : Always 0x800402
 *      2 : pointer to AcConfig struct
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : Infra Priority
 */
static void GetInfraPriority(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // Infra Priority

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

/**
 * AC_U::SetRequestEulaVersion service function
 *  Inputs:
 *      1 : Eula Version major
 *      2 : Eula Version minor
 *      3 : AcConfig size << 14 | 2
 *      4 : Input pointer to AcConfig struct
 *      64 : AcConfig size << 14 | 2
 *      65 : Output pointer to AcConfig struct
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : Infra Priority
 */
static void SetRequestEulaVersion(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u8 major = cmd_buff[1] & 0xFF;
    u8 minor = cmd_buff[2] & 0xFF;

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // Infra Priority

    LOG_WARNING(Service_AC, "(STUBBED) called, major=%d, minor=%d", major, minor);
}

/**
 * AC_U::RegisterDisconnectEvent service function
 *  Inputs:
 *      1 : ProcessId Header (must be 0x20)
 *      3 : Always 0
 *      4 : Event handle, should be signaled when AC connection is closed
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
static void RegisterDisconnectEvent(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    disconnect_event = Kernel::g_handle_table.Get<Kernel::Event>(cmd_buff[4]);
    if (disconnect_event) {
        disconnect_event->name = "AC_U:disconnect_event";
    }
    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

/**
 * AC_U::IsConnected service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : bool, is connected
 */
static void IsConnected(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = ac_connected;

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

const Interface::FunctionInfo FunctionTable[] = {
    {0x00010000, CreateDefaultConfig,     "CreateDefaultConfig"},
    {0x00040006, ConnectAsync,            "ConnectAsync"},
    {0x00050002, GetConnectResult,        "GetConnectResult"},
    {0x00080004, CloseAsync,              "CloseAsync"},
    {0x00090002, GetCloseResult,          "GetCloseResult"},
    {0x000A0000, nullptr,                 "GetLastErrorCode"},
    {0x000D0000, GetWifiStatus,           "GetWifiStatus"},
    {0x000E0042, nullptr,                 "GetCurrentAPInfo"},
    {0x00100042, nullptr,                 "GetCurrentNZoneInfo"},
    {0x00110042, nullptr,                 "GetNZoneApNumService"},
    {0x001D0042, nullptr,                 "ScanAPs"},
    {0x00240042, AddDenyApType,           "AddDenyApType"},
    {0x00270002, GetInfraPriority,        "GetInfraPriority"},
    {0x002D0082, SetRequestEulaVersion,   "SetRequestEulaVersion"},
    {0x00300004, RegisterDisconnectEvent, "RegisterDisconnectEvent"},
    {0x003C0042, nullptr,                 "GetAPSSIDList"},
    {0x003E0042, IsConnected,             "IsConnected"},
    {0x00400042, nullptr,                 "SetClientVersion"},
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface class

Interface::Interface() {
    Register(FunctionTable);

    ac_connected = false;

    close_event = nullptr;
    connect_event = nullptr;
    disconnect_event = nullptr;
}

Interface::~Interface() {
    close_event = nullptr;
    connect_event = nullptr;
    disconnect_event = nullptr;
}

} // namespace
