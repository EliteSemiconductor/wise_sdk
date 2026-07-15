#ifndef __HAL_DRV_RADIO_WEAK_H__
#define __HAL_DRV_RADIO_WEAK_H__

#include <stdint.h>

void hal_drv_radio_test(uint32_t val);
void hal_drv_radio_chsd_init(void);
void hal_drv_radio_fpga_init(FPGA_INFO_T *fpga_info);
void hal_drv_radio_set_la_init(uint8_t la_output_fmt);
void hal_drv_radio_chsd_set_zero_if(void);


#endif /* __HAL_DRV_RADIO_WEAK_H__ */
