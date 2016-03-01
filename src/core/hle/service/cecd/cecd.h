// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "core/hle/service/service.h"

namespace Service {
namespace CECD {

void GetCecInfoBuffer(Service::Interface* self);
void GetCecInfoEventHandle(Service::Interface* self);
void OpenAndReadFile(Service::Interface* self);
void SetData(Service::Interface* self);


/// Initialize CECD service(s)
void Init();

/// Shutdown CECD service(s)
void Shutdown();

} // namespace CECD
} // namespace Service
