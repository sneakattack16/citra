// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "core/hle/service/service.h"

namespace Service {
namespace FRD {

void GetMyPresence(Service::Interface* self);
void GetFriendKeyList(Service::Interface* self);
void GetFriendProfile(Service::Interface* self);
void GetFriendAttributeFlags(Service::Interface* self);
void GetFriendScreenName(Service::Interface* self);
void GetFriendKey(Service::Interface* self);
void GetMyScreenName(Service::Interface* self);


/// Initialize FRD service(s)
void Init();

/// Shutdown FRD service(s)
void Shutdown();

} // namespace FRD
} // namespace Service
