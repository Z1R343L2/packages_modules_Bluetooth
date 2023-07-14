/*
 *  Copyright 2020 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#pragma once

#include <cstdint>

#include "osi/include/osi.h"  // UNUSED_ATTR
#include "stack/include/btm_ble_api_types.h"
#include "stack/include/hci_error_code.h"
#include "types/ble_address_with_type.h"
#include "types/raw_address.h"

// This header contains functions for HCI-ble to invoke
void btm_ble_process_adv_pkt(uint8_t len, const uint8_t* p);
void btm_ble_process_ext_adv_pkt(uint8_t len, const uint8_t* p);
void btm_ble_process_phy_update_pkt(uint8_t len, uint8_t* p);
void btm_ble_read_remote_features_complete(uint8_t* p, uint8_t length);
void btm_le_on_advertising_set_terminated(uint8_t* p, uint16_t length);
void btm_ble_write_adv_enable_complete(uint8_t* p, uint16_t evt_len);
void btm_ble_create_ll_conn_complete(tHCI_STATUS status);
void btm_ble_ltk_request(uint16_t handle, uint8_t rand[8], uint16_t ediv);
void btm_ble_test_command_complete(uint8_t* p);
bool btm_identity_addr_to_random_pseudo(RawAddress* bd_addr,
                                        tBLE_ADDR_TYPE* p_addr_type,
                                        bool refresh);
bool btm_identity_addr_to_random_pseudo_from_address_with_type(
    tBLE_BD_ADDR* address_with_type, bool refresh);
void btm_ble_read_resolving_list_entry_complete(const uint8_t* p,
                                                uint16_t evt_len);
void btm_ble_remove_resolving_list_entry_complete(uint8_t* p, uint16_t evt_len);
void btm_ble_add_resolving_list_entry_complete(uint8_t* p, uint16_t evt_len);
void btm_ble_clear_resolving_list_complete(uint8_t* p, uint16_t evt_len);
void btm_ble_process_periodic_adv_pkt(uint8_t len, const uint8_t* p);
void btm_ble_process_periodic_adv_sync_est_evt(uint8_t len, const uint8_t* p);
void btm_ble_process_periodic_adv_sync_lost_evt(uint8_t len, uint8_t* p);
