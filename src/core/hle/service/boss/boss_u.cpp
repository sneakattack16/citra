// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/hle/service/boss/boss.h"
#include "core/hle/service/boss/boss_u.h"

namespace Service {
namespace BOSS {

const Interface::FunctionInfo FunctionTable[] = {
    {0x00010082, InitializeSession,     "InitializeSession"},
    {0x00020100, nullptr,               "RegisterStorage"},
    {0x000A0000, GetOptoutFlag,         "GetOptoutFlag"},
    {0x000E0000, GetTaskIdList,         "GetTaskIdList"},
    {0x000C0082, nullptr,               "UnregisterTask"},
    {0x00160082, ReceiveProperty,       "ReceiveProperty"},
    {0x001E0042, nullptr,               "CancelTask"},
    {0x00330042, nullptr,               "StartTaskBgImmediate"},
};

BOSS_U_Interface::BOSS_U_Interface() {
    Register(FunctionTable);
}

} // namespace BOSS
} // namespace Service
