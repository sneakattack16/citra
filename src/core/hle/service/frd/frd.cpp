// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/string_util.h"

#include "core/hle/service/service.h"
#include "core/hle/service/frd/frd.h"
#include "core/hle/service/frd/frd_a.h"
#include "core/hle/service/frd/frd_u.h"

namespace Service {
namespace FRD {

void GetMyPresence(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // private mode
    cmd_buff[3] = 0; // don't show current game
    cmd_buff[4] = 0; // unknown

    LOG_WARNING(Service_FRD, "(STUBBED) called");
}

void GetFriendKeyList(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 unknown   = cmd_buff[1];
    u32 frd_count = cmd_buff[2];
    u32 buf_addr  = cmd_buff[65];
    FriendKey* frd_key_list = reinterpret_cast<FriendKey*>(Memory::GetPointer(buf_addr));
    memset(frd_key_list, 0, frd_count * sizeof(FriendKey));

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // 0 friends
    LOG_WARNING(Service_FRD, "(STUBBED) called, unknown=%d, frd_count=%d, buf_addr=0x%08X", unknown,
                frd_count, buf_addr);
}

void GetFriendProfile(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    LOG_WARNING(Service_FRD, "(STUBBED) called");
}

void GetFriendScreenName(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    LOG_WARNING(Service_FRD, "(STUBBED) called");
}

void GetFriendAttributeFlags(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    LOG_WARNING(Service_FRD, "(STUBBED) called");
}

void GetMyFriendKey(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    LOG_WARNING(Service_FRD, "(STUBBED) called");
}

void GetMyScreenName(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = 0; // No error
    // TODO: (mailwl) get the name from config
    Common::UTF8ToUTF16("Citra").copy(reinterpret_cast<char16_t*>(&cmd_buff[2]), 20);
    LOG_WARNING(Service_FRD, "(STUBBED) called");
}

void Init() {
    using namespace Kernel;

    AddService(new FRD_A_Interface);
    AddService(new FRD_U_Interface);
}

void Shutdown() {
}

} // namespace FRD

} // namespace Service
