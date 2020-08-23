/*
 * Copyright 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>

#include "stack/include/acl_api_types.h"
#include "stack/include/bt_types.h"
#include "stack/include/btm_api_types.h"
#include "stack/include/hcidefs.h"
#include "stack/include/hcimsgs.h"
#include "types/bt_transport.h"
#include "types/raw_address.h"

enum btm_acl_encrypt_state_t {
  BTM_ACL_ENCRYPT_STATE_IDLE = 0,
  BTM_ACL_ENCRYPT_STATE_ENCRYPT_OFF = 1,
  BTM_ACL_ENCRYPT_STATE_TEMP_FUNC = 2,
  BTM_ACL_ENCRYPT_STATE_ENCRYPT_ON = 3,
};

/* Structure returned with Role Switch information (in tBTM_CMPL_CB callback
 * function) in response to BTM_SwitchRole call.
 */
typedef struct {
  RawAddress remote_bd_addr; /* Remote BD addr involved with the switch */
  uint8_t hci_status;        /* HCI status returned with the event */
  uint8_t role;              /* HCI_ROLE_MASTER or HCI_ROLE_SLAVE */
} tBTM_ROLE_SWITCH_CMPL;

typedef struct {
  BD_FEATURES peer_le_features;
  BD_FEATURES peer_lmp_feature_pages[HCI_EXT_FEATURES_PAGE_MAX + 1];
  RawAddress active_remote_addr;
  RawAddress conn_addr;
  RawAddress remote_addr;
  bool in_use;
  bool link_up_issued;
  tBT_TRANSPORT transport;
  uint16_t clock_offset;
  uint16_t flush_timeout_in_ticks;
  uint16_t hci_handle;
  uint16_t link_policy;
  uint16_t link_super_tout;
  uint16_t lmp_subversion;
  uint16_t manufacturer;
  uint16_t pkt_types_mask;
  uint8_t active_remote_addr_type;
  uint8_t conn_addr_type;
  uint8_t disconnect_reason;

 private:
  btm_acl_encrypt_state_t encrypt_state_;

 public:
  void set_encryption_off() {
    if (encrypt_state_ != BTM_ACL_ENCRYPT_STATE_ENCRYPT_OFF) {
      btsnd_hcic_set_conn_encrypt(hci_handle, false);
      encrypt_state_ = BTM_ACL_ENCRYPT_STATE_ENCRYPT_OFF;
    }
  }
  void set_encryption_on() {
    if (encrypt_state_ != BTM_ACL_ENCRYPT_STATE_ENCRYPT_ON) {
      btsnd_hcic_set_conn_encrypt(hci_handle, true);
      encrypt_state_ = BTM_ACL_ENCRYPT_STATE_ENCRYPT_ON;
    }
  }
  void set_encryption_idle() { encrypt_state_ = BTM_ACL_ENCRYPT_STATE_IDLE; }

  void set_encryption_switching() {
    encrypt_state_ = BTM_ACL_ENCRYPT_STATE_TEMP_FUNC;
  }

 public:
  uint8_t link_role;
  uint8_t lmp_version;
  uint8_t num_read_pages;
  uint8_t switch_role_failed_attempts;
  uint8_t switch_role_state;
  uint8_t sca; /* Sleep clock accuracy */
} tACL_CONN;

typedef uint8_t tBTM_PM_STATE;
typedef struct {
  bool chg_ind;
  tBTM_PM_PWR_MD req_mode[BTM_MAX_PM_RECORDS + 1];
  tBTM_PM_PWR_MD set_mode;
  tBTM_PM_STATE state;
  uint16_t interval;
  uint16_t max_lat;
  uint16_t min_loc_to;
  uint16_t min_rmt_to;
} tBTM_PM_MCB;

/****************************************************
 **      ACL Management API
 ****************************************************/
typedef struct {
 private:
  friend bool BTM_IsBleConnection(uint16_t hci_handle);
  friend bool BTM_ReadPowerMode(const RawAddress& remote_bda,
                                tBTM_PM_MODE* p_mode);
  friend bool acl_is_role_switch_allowed();
  friend const RawAddress acl_address_from_handle(uint16_t hci_handle);
  friend int btm_pm_find_acl_ind(const RawAddress& remote_bda);
  friend tACL_CONN* btm_bda_to_acl(const RawAddress& bda,
                                   tBT_TRANSPORT transport);
  friend tBTM_PM_MCB* acl_power_mode_from_handle(uint16_t hci_handle);
  friend tBTM_STATUS BTM_SetPowerMode(uint8_t pm_id,
                                      const RawAddress& remote_bda,
                                      const tBTM_PM_PWR_MD* p_mode);
  friend tBTM_STATUS BTM_SetSsrParams(const RawAddress& remote_bda,
                                      uint16_t max_lat, uint16_t min_rmt_to,
                                      uint16_t min_loc_to);
  friend tBTM_STATUS btm_read_power_mode_state(const RawAddress& remote_bda,
                                               tBTM_PM_STATE* pmState);
  friend uint16_t BTM_GetMaxPacketSize(const RawAddress& addr);
  friend uint16_t BTM_GetNumAclLinks(void);
  friend uint16_t acl_get_link_supervision_timeout();
  friend uint16_t acl_get_supported_packet_types();
  friend uint16_t btm_get_acl_disc_reason_code(void);
  friend uint8_t acl_get_disconnect_reason();
  friend uint8_t btm_handle_to_acl_index(uint16_t hci_handle);
  friend void BTM_SetDefaultLinkSuperTout(uint16_t timeout);
  friend void BTM_acl_after_controller_started();
  friend void BTM_default_block_role_switch();
  friend void BTM_default_unblock_role_switch();
  friend void acl_set_disconnect_reason(uint8_t acl_disc_reason);
  friend void btm_acl_created(const RawAddress& bda, uint16_t hci_handle,
                              uint8_t link_role, tBT_TRANSPORT transport);
  friend void btm_acl_device_down(void);
  friend void btm_acl_encrypt_change(uint16_t handle, uint8_t status,
                                     uint8_t encr_enable);
  friend void btm_acl_init(void);
  friend void btm_acl_process_sca_cmpl_pkt(uint8_t evt_len, uint8_t* p);
  friend void btm_acl_role_changed(uint8_t hci_status,
                                   const RawAddress& bd_addr, uint8_t new_role);
  friend void btm_acl_update_conn_addr(uint16_t conn_handle,
                                       const RawAddress& address);
  friend void btm_pm_proc_cmd_status(uint8_t status);
  friend void btm_pm_proc_mode_change(uint8_t hci_status, uint16_t hci_handle,
                                      uint8_t mode, uint16_t interval);
  friend void btm_pm_proc_ssr_evt(uint8_t* p, uint16_t evt_len);
  friend void btm_pm_reset(void);
  friend void btm_pm_sm_alloc(uint8_t ind);
  friend void btm_process_clk_off_comp_evt(uint16_t hci_handle,
                                           uint16_t clock_offset);
  friend void btm_read_automatic_flush_timeout_complete(uint8_t* p);
  friend void btm_read_failed_contact_counter_complete(uint8_t* p);
  friend void btm_read_link_quality_complete(uint8_t* p);
  friend void btm_read_remote_ext_features_complete(uint8_t* p,
                                                    uint8_t evt_len);
  friend void btm_read_remote_ext_features_failed(uint8_t status,
                                                  uint16_t handle);
  friend void btm_read_remote_features_complete(uint8_t* p);
  friend void btm_read_remote_version_complete(uint8_t* p);
  friend void btm_read_rssi_complete(uint8_t* p);
  friend void btm_read_tx_power_complete(uint8_t* p, bool is_ble);

  friend struct StackAclBtmAcl;
  friend struct StackAclBtmPm;

  tACL_CONN acl_db[MAX_L2CAP_LINKS];
  tBTM_PM_MCB pm_mode_db[MAX_L2CAP_LINKS];
  tBTM_ROLE_SWITCH_CMPL switch_role_ref_data;
  uint16_t btm_acl_pkt_types_supported;
  uint16_t btm_def_link_policy;
  uint16_t btm_def_link_super_tout;
  uint8_t acl_disc_reason;
  uint8_t pm_pend_link;
} tACL_CB;
