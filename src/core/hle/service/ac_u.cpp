// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/logging/log.h"
#include "core/hle/kernel/event.h"
#include "core/hle/service/ac_u.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Namespace AC_U

namespace AC_U {

/**
 * AC_U::GetConnectingApType service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : Output connection type, 0 = none, 1 = Old3DS Internet, 2 = New3DS Internet.
 */
static void GetConnectingApType(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    // TODO(purpasmart96): This function is only a stub,
    // it returns a valid result without implementing full functionality.

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // Connection type set to none

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
    cmd_buff[2] = false; // Not connected to ac:u service

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

static void CloseAsync(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    auto evt = Kernel::g_handle_table.Get<Kernel::Event>(cmd_buff[4]);
    if (evt) {
        evt->name = "AC_U:close_event";
        evt->Signal();
    }
    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_AC, "(STUBBED) called");
}

static void GetCloseResult(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = RESULT_SUCCESS.raw;
    LOG_WARNING(Service_AC, "(STUBBED) called");
}

const Interface::FunctionInfo FunctionTable[] = {
    {0x00010000, nullptr,               "CreateDefaultConfig"},
    {0x00020042, nullptr,               "DebugSetApType"},
    {0x00030042, nullptr,               "DebugSetNetworkArea"},
    {0x00040006, nullptr,               "ConnectAsync"},
    {0x00050002, nullptr,               "GetConnectResult"},
    {0x000600C6, nullptr,               "DebugSetNetworkSetting1"},
    {0x00070002, nullptr,               "CancelConnectAsync"},
    {0x00080004, CloseAsync,            "CloseAsync"},
    {0x00090002, nullptr,               "GetCloseResult"},
    {0x000A0000, nullptr,               "GetLastErrorCode"},
    {0x000B0000, nullptr,               "GetLastDetailErrorCode"},
    {0x000C0000, nullptr,               "GetStatus"},
    {0x000D0000, GetConnectingApType,   "GetConnectingApType"},
    {0x000E0042, nullptr,               "GetConnectingAccessPoint"},
    {0x000F0000, nullptr,               "GetConnectingInfraPriority"},
    {0x00100042, nullptr,               "GetConnectingNintendoZoneBeacon"},
    {0x00110042, nullptr,               "GetConnectingNintendoZoneBeaconSubset"},
    {0x00120042, nullptr,               "GetConnectingHotspot"},
    {0x00130042, nullptr,               "GetConnectingHotspotSubset"},
    {0x00140002, nullptr,               "GetConnectingLocation"},
    {0x00150004, nullptr,               "ExclusiveAsync"},
    {0x00160002, nullptr,               "GetExclusiveResult"},
    {0x00170004, nullptr,               "UnExclusiveAsync"},
    {0x00180002, nullptr,               "GetUnExclusiveResult"},
    {0x00190004, nullptr,               "CloseAllAsync"},
    {0x001A0002, nullptr,               "GetCloseAllResult"},
    {0x001B0004, nullptr,               "LogoutHotspotAsync"},
    {0x001C0002, nullptr,               "GetLogoutHotspotResult"},
    {0x001D0042, nullptr,               "ScanAccessPoint"},
    {0x001E0042, nullptr,               "ScanNintendoZone"},
    {0x001F0042, nullptr,               "ScanNintendoZoneSubset"},
    {0x00200005, nullptr,               "BeginScanUsbAccessPoint"},
    {0x00210002, nullptr,               "EndScanUsbAccessPoint"},
    {0x00220042, nullptr,               "SetAllowApType"},
    {0x00230042, nullptr,               "AddAllowApType"},
    {0x00240042, nullptr,               "AddDenyApType"},
    {0x00250042, nullptr,               "SetNetworkArea"},
    {0x00260042, nullptr,               "SetInfraPriority"},
    {0x00270002, nullptr,               "GetInfraPriority"},
    {0x00280042, nullptr,               "SetPowerSaveMode"},
    {0x00290002, nullptr,               "GetPowerSaveMode"},
    {0x002A0004, nullptr,               "SetBssidFilter"},
    {0x002B0004, nullptr,               "SetApnumFilter"},
    {0x002C0042, nullptr,               "SetFromApplication"},
    {0x002D0082, nullptr,               "SetRequestEulaVersion"},
    {0x002E00C4, nullptr,               "ConvertPassphraseToPsk"},
    {0x002F0004, nullptr,               "GetNintendoZoneBeaconNotFoundEvent"},
    {0x00300004, nullptr,               "RegisterDisconnectEvent"},
    {0x00310002, nullptr,               "GetStatusChangeEvent"},
    {0x00320042, nullptr,               "SetAuthServerType"},
    {0x00330000, nullptr,               "GetConnectingSecurityMode"},
    {0x00340000, nullptr,               "GetConnectingSsid"},
    {0x00350000, nullptr,               "GetConnectingSsidLength"},
    {0x00360000, nullptr,               "GetConnectingProxyEnable"},
    {0x00370000, nullptr,               "GetConnectingProxyAuthType"},
    {0x00380000, nullptr,               "GetConnectingProxyPort"},
    {0x00390000, nullptr,               "GetConnectingProxyHost"},
    {0x003A0000, nullptr,               "GetConnectingProxyUserName"},
    {0x003B0000, nullptr,               "GetConnectingProxyPassword"},
    {0x003C0042, nullptr,               "GetCecSsidList"},
    {0x003D0042, nullptr,               "SetZoneMacFilter"},
    {0x003E0042, IsConnected,           "IsConnected"},
    {0x003F0040, nullptr,               "GetNotAwakeMacFilter"},
    {0x00400042, nullptr,               "SetClientVersion"},
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface class

Interface::Interface() {
    Register(FunctionTable);
}

} // namespace
