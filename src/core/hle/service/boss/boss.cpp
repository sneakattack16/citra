// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/logging/log.h"

#include "core/hle/service/service.h"
#include "core/hle/service/boss/boss.h"
#include "core/hle/service/boss/boss_p.h"
#include "core/hle/service/boss/boss_u.h"

namespace Service {
namespace BOSS {

void InitializeSession(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    LOG_WARNING(Service_BOSS, "(STUBBED) called");
}

void GetOptoutFlag(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0;
    LOG_WARNING(Service_BOSS, "(STUBBED) called");
}

void GetTaskIdList(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_BOSS, "(STUBBED) called");
}

void ReceiveProperty(Service::Interface* self){
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 property_id = cmd_buff[1];
    u32 size = cmd_buff[2];
    u32 addr = cmd_buff[4];

    for (u32 i = 0; i < size; ++i) {
        Memory::Write8(addr + i, 0);
    }

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    LOG_WARNING(Service_BOSS, "(STUBBED) called");
}

void Init() {
    using namespace Kernel;

    AddService(new BOSS_P_Interface);
    AddService(new BOSS_U_Interface);
}

void Shutdown() {
}

} // namespace BOSS

} // namespace Service
