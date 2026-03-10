#ifndef __WMBUS_FSM_H__
#define __WMBUS_FSM_H__



typedef enum{
WMBUS_DEC_TIMER            = 0,
WMBUS_FAC_TIMER            = 1,
WMBUS_TX_DONE              = 2,
WMBUS_TX_FAIL              = 3,
WMBUS_RX_DONE              = 4,
WMBUS_RX_TIMEOUT           = 5,
WMBUS_RESERVED_EVENT       = 6,
WMBUS_TRIGGER_INSTALL      = 7,
WMBUS_RX_ERROR             = 8,  //wise_event ask to be power of 2
WMBUS_RX_SYNC_WORD         = 16,
WMBUS_DUMMY_EVENT          = 99
} t_WMBUS_Event_Definition;



typedef struct{
	uint8_t  state; // Main State for WMBUS device
	uint8_t  sub_state; // Substate is dependent on what the main state is.
} t_WMBUS_Config;

//t_WMBUS_Config WMBUS_config;

typedef enum{
	WMBUS_GW          = 0,
	WMBUS_METER           = 1
} t_WMBUS_role;

typedef enum{
	STATE_UNINSTALLED     = 0,
	STATE_STANDBY	= 1,
} t_WMBUS_state;


typedef enum{
	SUBSTATE_MTR_IDLE       = 0,
	W4_SND_IR_TX           = 1,
	W4_CNF_IR_RX       = 2,
//	W4_DELAY_End       = 3,
//	W4_INSTALL_End = 4
} t_WMBUS_meter_uninstalled_substate;

typedef enum{
	SUBSTATE_MTR_STANDBY       = 0,
	W4_FRT_PKT_TX           = 1,
	W4_REQ_PKT_RX       = 2,
	W4_RSP_ACK_TX       = 3,
	W4_NEXT_PKT_RX = 4
} t_WMBUS_meter_standby_substate;

/*
typedef enum{
	STATE_STANDBY       = 0,
	W4_CNF_IR_TX           = 1,
	W4_NKE_TX       = 2,
} t_WMBUS_GW_installer_substate;
*/

typedef enum{
	SUBSTATE_GW_STANDBY       = 0,
	W4_PKT_TX               = 1,
	W4_RSP_RX                  =2,
	W4_MEAS_TIME_TX           = 3,
//	W4_NKE_TX       = 3,
} t_WMBUS_GW_standby_substate;


void wmbus_link_fsm_init(void);
void wmbus_link_fsm(uint8_t trigger_event);
void wmbus_link_fsm_translate(uint8_t module, uint32_t event);

void meter_handle_install_fsm(uint8_t trigger_event);
void meter_handle_fsm(uint8_t trigger_event);

void gw_handle_fsm(uint8_t trigger_event);

#endif 

