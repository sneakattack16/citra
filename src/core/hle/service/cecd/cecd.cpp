// Copyright 2015 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/logging/log.h"

#include "core/hle/kernel/event.h"
#include "core/hle/service/service.h"
#include "core/hle/service/cecd/cecd.h"
#include "core/hle/service/cecd/cecd_s.h"
#include "core/hle/service/cecd/cecd_u.h"

namespace Service {
namespace CECD {

static Kernel::SharedPtr<Kernel::Event> cecinfo_event;
static Kernel::SharedPtr<Kernel::Event> change_state_event;

void GetCecStateAbbreviated(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = static_cast<u32>(CecStateAbbreviated::CEC_STATE_ABBREV_IDLE);

    LOG_WARNING(Service_CECD, "(STUBBED) called");
}

void GetCecInfoEventHandle(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[3] = Kernel::g_handle_table.Create(cecinfo_event).MoveFrom(); // Event handle

    LOG_WARNING(Service_CECD, "(STUBBED) called");
}

void GetChangeStateEventHandle(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[3] = Kernel::g_handle_table.Create(change_state_event).MoveFrom(); // Event handle

    LOG_WARNING(Service_CECD, "(STUBBED) called");
}

void OpenAndWrite(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 size = cmd_buff[1];
    u32 cec_title_id = cmd_buff[2];
    u32 data_type = cmd_buff[3];
    u32 option = cmd_buff[4];
    // 5 - 0x20
    // 7 - size * 16 | 0xA
    u32 addr = cmd_buff[8];

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_CECD, "(STUBBED) called size=%d, addr=0x%08X, title=0x%08X, data_type=%d, option=0x%X", size, addr, cec_title_id, data_type, option);
    //Common::Dump(addr, size);
}

void OpenAndRead(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 size = cmd_buff[1];
    u32 cec_title_id = cmd_buff[2];
    u32 data_type = cmd_buff[3];
    u32 option = cmd_buff[4];
    // 5 - 0x20
    // 7 - size * 16 | 0xA
    u32 addr = cmd_buff[8];

    for(u32 i=0; i<size; i++) {
        Memory::Write8(addr + i, 0);
    }

    LOG_WARNING(Service_CECD, "(STUBBED) called size=%d, addr=0x%08X, title=0x%08X, data_type=%d, option=0x%X", size, addr, cec_title_id, data_type, option);

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // output size
}

void OpenMailbox(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 cec_title_id = cmd_buff[1];
    u32 data_type = cmd_buff[2];
    u32 option = cmd_buff[3];

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // output size

    LOG_WARNING(Service_CECD, "(STUBBED) called title_id=0x%08X, data_type=%d, option=0x%X", cec_title_id, data_type, option);
}

// nn::Result SetData( u32 cecTitleId, const u8 pData[], size_t len, u32 option );
void SetData(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 cec_title_id = cmd_buff[1];
    u32 size = cmd_buff[2];
    u32 option = cmd_buff[3];
    // 4: size *16 & 0xFFFFFFF0 | 0xA
    u32 addr = cmd_buff[5];

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error

    LOG_WARNING(Service_CECD, "(STUBBED) called title_id=0x%08X, addr=0x%08X, size=%d, option=0x%X", cec_title_id, addr, size, option);
    //if(addr) Common::Dump(addr, size);
}

// nn::Result ReadData( u8 pReadBuf[], size_t len, u32 option, const u8 optionData[], size_t optionDataLen );
void ReadData(Service::Interface* self) {
    u32* cmd_buff = Kernel::GetCommandBuffer();

    u32 size = cmd_buff[1];
    u32 option = cmd_buff[2];
    u32 size_opt = cmd_buff[3];
    // 4: size *16 & 0xFFFFFFF0 | 0xA
    u32 addr_opt = cmd_buff[5];
    // 6: opt_size *16 & 0xFFFFFFF0 | 0xA
    u32 addr = cmd_buff[7];

    cmd_buff[1] = RESULT_SUCCESS.raw; // No error
    cmd_buff[2] = 0; // File Size

    LOG_WARNING(Service_CECD, "(STUBBED) called addr=0x%08X, size=%d, option=0x%X", addr, size, option);
    //if (addr_opt) Common::Dump(addr_opt, size_opt);
}

void Init() {
    AddService(new CECD_S_Interface);
    AddService(new CECD_U_Interface);

    cecinfo_event = Kernel::Event::Create(Kernel::ResetType::OneShot, "CECD_U::cecinfo_event");
    change_state_event = Kernel::Event::Create(Kernel::ResetType::OneShot, "CECD_U::change_state_event");
}

void Shutdown() {
    cecinfo_event = nullptr;
    change_state_event = nullptr;
}

} // namespace CECD

} // namespace Service
