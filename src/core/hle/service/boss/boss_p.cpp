// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/hle/service/boss/boss_p.h"

namespace Service {
namespace BOSS {


const Interface::FunctionInfo FunctionTable[] = {
    {0x041500C0, nullptr,                    "DeleteNsDataPrivileged"},
    {0x04170182, nullptr,                    "ReadNsDataPrivileged"},
    {0x041A0100, nullptr,                    "SetNsDataNewFlagPrivileged"},
    {0x041B00C0, nullptr,                    "GetNsDataNewFlagPrivileged"},
};

BOSS_P_Interface::BOSS_P_Interface() {
    Register(FunctionTable);
}

} // namespace BOSS
} // namespace Service
