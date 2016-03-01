// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/hle/service/cecd/cecd.h"
#include "core/hle/service/cecd/cecd_u.h"

namespace Service {
namespace CECD {

static const Interface::FunctionInfo FunctionTable[] = {
    {0x000100C2, nullptr,                   "Open"},
    {0x00020042, nullptr,                   "Read"},
    {0x00030104, nullptr,                   "ReadMessage"},
    {0x00040106, nullptr,                   "ReadMessageWithHmac"},
    {0x00050042, nullptr,                   "Write"},
    {0x00060104, nullptr,                   "WriteMessage"},
    {0x00070106, nullptr,                   "WriteMessageWithHmac"},
    {0x00080102, nullptr,                   "Delete"},
    {0x000900C2, SetData,                   "SetData"},
    {0x000A00C4, nullptr,                   "ReadData"},
    {0x000B0040, nullptr,                   "Start"},
    {0x000C0040, nullptr,                   "Stop"},
    {0x000D0082, GetCecInfoBuffer,          "GetCecInfoBuffer"},
    {0x000E0000, nullptr,                   "GetCecdState"},
    {0x000F0000, GetCecInfoEventHandle,     "GetCecInfoEventHandle"},
    {0x00100000, nullptr,                   "GetChangeStateEventHandle"},
    {0x00110104, nullptr,                   "OpenAndWriteFile"},
    {0x00120104, OpenAndReadFile,           "OpenAndReadFile"},
    {0x001E0082, nullptr,                   "GetEventLog"},
    {0x001F0000, nullptr,                   "GetEventLogStart"},
    {0x00200000, nullptr,                   "GetEventLogLength"},
    {0x00210080, nullptr,                   "CalcEventLogStart"},
};

CECD_U_Interface::CECD_U_Interface() {
    Register(FunctionTable);
}

} // namespace CECD
} // namespace Service
