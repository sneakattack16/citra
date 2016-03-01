// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common/common_types.h"

namespace Service {

class Interface;

namespace FRD {

struct FriendKey {
    u32 friend_id;
    u32 unknown;
    u64 friend_code;
};

/**
 * FRD::GetMyPresence service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : Public mode (0 = private, non-zero = public)
 *      3 : Show current game (byte, 0 = don't show, non-zero = show)
 *      4 : Unknown
 */
void GetMyPresence(Service::Interface* self);

/**
 * FRD::GetFriendKeyList service function
 *  Inputs:
 *      1 : Unknown
 *      2 : Max friends count
 *      65 : Address of FriendKey List
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : FriendKey count filled
 */
void GetFriendKeyList(Service::Interface* self);

/**
 * FRD::GetFriendProfile service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
void GetFriendProfile(Service::Interface* self);

/**
 * FRD::GetFriendAttributeFlags service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
void GetFriendAttributeFlags(Service::Interface* self);

/**
 * FRD::GetFriendScreenName service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
void GetFriendScreenName(Service::Interface* self);

/**
 * FRD::GetMyFriendKey service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 */
void GetMyFriendKey(Service::Interface* self);

/**
 * FRD::GetMyScreenName service function
 *  Outputs:
 *      1 : Result of function, 0 on success, otherwise error code
 *      2 : UTF16 encoded name
 */
void GetMyScreenName(Service::Interface* self);

/// Initialize FRD service(s)
void Init();

/// Shutdown FRD service(s)
void Shutdown();

} // namespace FRD
} // namespace Service
