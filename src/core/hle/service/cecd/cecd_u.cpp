// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/hle/service/cecd/cecd.h"
#include "core/hle/service/cecd/cecd_u.h"

namespace Service {
namespace CECD {

static const Interface::FunctionInfo FunctionTable[] = {
    {0x000100C2, OpenMailbox,               "OpenMailbox"},
    {0x00080102, nullptr,                   "DeleteMailbox"},
    {0x000900C2, SetData,                   "SetData"},
    {0x000A00C4, ReadData,                  "ReadData"},
    {0x000E0000, GetCecStateAbbreviated,    "GetCecStateAbbreviated"},
    {0x000F0000, GetCecInfoEventHandle,     "GetCecInfoEventHandle"},
    {0x00100000, GetChangeStateEventHandle, "GetChangeStateEventHandle"},
    {0x00110104, OpenAndWrite,              "OpenAndWrite"},
    {0x00120104, OpenAndRead,               "OpenAndRead"},
};

CECD_U_Interface::CECD_U_Interface() {
    Register(FunctionTable);
}

} // namespace CECD
} // namespace Service
