// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

namespace Service {

class Interface;

namespace BOSS {

void InitializeSession(Service::Interface* self);
void GetOptoutFlag(Service::Interface* self);
void GetTaskIdList(Service::Interface* self);
void ReceiveProperty(Service::Interface* self);

/// Initialize BOSS service(s)
void Init();

/// Shutdown BOSS service(s)
void Shutdown();

} // namespace BOSS
} // namespace Service
