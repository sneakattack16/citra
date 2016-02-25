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

#define PADDING4 int : 32
struct FriendKey {
    u32 id;
    PADDING4;
    u64 code;
};

void GetMyPresence(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();
    u32 size = cmd_buff[64];
    u8* data = Memory::GetPointer(cmd_buff[65]);
    (void)size;
    (void)data;

    cmd_buff[1] = RESULT_SUCCESS.raw;
    cmd_buff[2] = 0;
    cmd_buff[3] = 0;
    cmd_buff[4] = 0;

    LOG_TRACE(Service_FRD, "(STUBBED) called");
}

void GetFriendKeyList(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 offset = cmd_buff[1];
    u32 size = cmd_buff[2];
    u32 buf = cmd_buff[65];
    FriendKey* frd_list = (FriendKey*)Memory::GetPointer(buf);
    memset(frd_list + offset, 0, size);

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // 0 friends

    LOG_TRACE(Service_FRD, "(STUBBED) called");
}

void GetFriendProfile(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw;
    LOG_TRACE(Service_FRD, "(STUBBED) called");
}

void GetFriendScreenName(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw;
    LOG_TRACE(Service_FRD, "(STUBBED) called");
}

void GetFriendAttributeFlags(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw;
    LOG_TRACE(Service_FRD, "(STUBBED) called");
}

void GetFriendKey(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_TRACE(Service_FRD, "(STUBBED) called");
}

void GetMyScreenName(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = 0; // No error
    //cmd_buff[2] = 'A';

    Common::UTF8ToUTF16("Citra").copy((char16_t*)&cmd_buff[2], 20);

    LOG_TRACE(Service_FRD, "(STUBBED) called");
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
