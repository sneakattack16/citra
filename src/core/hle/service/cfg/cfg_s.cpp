// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "core/hle/service/cfg/cfg.h"
#include "core/hle/service/cfg/cfg_s.h"

namespace Service {
namespace CFG {

const Interface::FunctionInfo FunctionTable[] = {
    // cfg common
    {0x00010082, GetConfig,                            "GetConfig"},
    {0x00020000, GetRegion,                            "GetRegion"},
    {0x00030040, GetTransferableId,                    "GetTransferableId"},
    {0x00040000, IsCoppacsSupported,                   "IsCoppacsSupported"},
    {0x00050000, GetSystemModel,                       "GetSystemModel"},
    {0x00060000, GetModelNintendo2DS,                  "GetModelNintendo2DS"},
    {0x00070040, nullptr,                              "WriteToFirstByteCfgSavegame"},
    {0x00080080, nullptr,                              "GoThroughTable"},
    {0x00090040, GetCountryCodeString,                 "GetCountryCodeString"},
    {0x000A0040, GetCountryCodeID,                     "GetCountryCodeID"},
    // cfg:s
    {0x04010082, GetConfigForSys,                      "GetConfigForSys"},
    {0x04020082, nullptr,                              "SetConfigForSys"},
    {0x04030000, FlushConfigForSys,                    "FlushConfigForSys"},
    {0x04040042, nullptr,                              "GetLocalFriendCodeSeedCertificateForSys"},
    {0x04050000, nullptr,                              "GetLocalFriendCodeSeedForSys"},
    {0x04060000, GetRegion,                            "GetRegionForSys"},
    {0x04070000, nullptr,                              "GetFlagsForSys"},
    {0x04080042, nullptr,                              "GetSerialNoForSys"},
    {0x04090000, nullptr,                              "ResetAccelerometerCalibrationForSys"},
    {0x040A0000, nullptr,                              "EvacuateSaveDataForSys"},
    {0x040B0000, nullptr,                              "DeleteEvacuatedSaveDataForSys"},
};

CFG_S_Interface::CFG_S_Interface() {
    Register(FunctionTable);
}

} // namespace CFG
} // namespace Service
