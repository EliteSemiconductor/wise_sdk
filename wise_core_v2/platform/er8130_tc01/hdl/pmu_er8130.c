/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "esmt_chip_specific.h"
#include "drv/hal_drv_pmu.h"
#include "hal_drv_radio.h"
#include "hal_intf_nfc.h"

#include "hdl/pmu_er8130.h"
#include "hdl/dma_er8130.h"
#include "ana_er8130.h"


static inline void pmu_unlock()
{
    if (REG_R32(PCRMU_LOCK_ADDR)) {
        REG_W32(PCRMU_LOCK_ADDR, PMU_UNLOCK_KEY);
    }
}

static inline void pmu_lock()
{
    REG_W32(PCRMU_LOCK_ADDR, 0x0);
}

static inline uint32_t _pmu_get_status()
{
    return (REG_R32(PCRMU_PMU_STS_REG_ADDR) & (PCRMU_PMU_MODEM_STS_MASK | PCRMU_PMU_PERIPH_STS_MASK | PCRMU_PMU_NFC_STS_MASK));
}

void pmu_set_pwr_mode_er8130(uint8_t mode)
{
    uint32_t cfg;

    uint8_t mcu_pd_en;
    uint8_t mem_pd_en;
    uint8_t ana_pd_en;
    uint8_t flh_pd_en;
    uint8_t cache_pd_en;

    uint8_t mem_pd_utl_modem;
    uint8_t mem_pd_utl_periph;
    uint8_t mem_pd_utl_nfc;
    uint8_t ana_pd_utl_modem;
    uint8_t ana_pd_utl_periph;
    uint8_t ana_pd_utl_nfc;
    uint8_t modem_pd_wi_cpu;
    uint8_t periph_pd_wi_cpu;
    uint8_t nfc_pd_wi_cpu;
    uint8_t modem_wu_wi_cpu;
    uint8_t periph_wu_wi_cpu;
    uint8_t nfc_wu_wi_cpu;
    uint8_t uart0_wu_en;
    uint8_t spi0_wu_en;
    uint8_t uart0_ret_en;
    uint8_t spi0_ret_en;
    uint8_t tim_out_en;

    if (mode == PWR_MODE_ACTIVITY) {
        mcu_pd_en   = 0;
        mem_pd_en   = 0;
        ana_pd_en   = 0;
        flh_pd_en   = 0;
        cache_pd_en = 0;

        mem_pd_utl_modem  = 0;
        mem_pd_utl_periph = 0;
        mem_pd_utl_nfc    = 0;
        ana_pd_utl_modem  = 0;
        ana_pd_utl_periph = 0;
        ana_pd_utl_nfc    = 0;
        modem_pd_wi_cpu   = 0;
        periph_pd_wi_cpu  = 0;
        nfc_pd_wi_cpu     = 0;
        modem_wu_wi_cpu   = 0;
        periph_wu_wi_cpu  = 0;
        nfc_wu_wi_cpu     = 0;
        uart0_wu_en       = 0;
        spi0_wu_en        = 0;
        uart0_ret_en      = 0;
        spi0_ret_en       = 0;
        tim_out_en        = 0;
    } else {
        mcu_pd_en   = 1;
        mem_pd_en   = 1;
        ana_pd_en   = 1;
        flh_pd_en   = 0;
        cache_pd_en = 1;

        mem_pd_utl_modem  = 1;
        mem_pd_utl_periph = 0;
        mem_pd_utl_nfc    = 0;
        ana_pd_utl_modem  = 1;
        ana_pd_utl_periph = 1;
        ana_pd_utl_nfc    = 0;
        modem_pd_wi_cpu   = 1;
        periph_pd_wi_cpu  = 1;
        nfc_pd_wi_cpu     = 1;
        modem_wu_wi_cpu   = 1;
        periph_wu_wi_cpu  = 1;
        nfc_wu_wi_cpu     = 1;
        uart0_wu_en       = 0;
        spi0_wu_en        = 0;
        uart0_ret_en      = 1;
        spi0_ret_en       = 1;
        tim_out_en        = 0;
    }

    pmu_unlock();

    // 0x40009100, PNU_EN_REG
    cfg = REG_R32(PCRMU_PMU_EN_REG_ADDR);
    cfg = (cfg & ~(PCRMU_PMU_MCU_PD_EN_MASK + PCRMU_PMU_MEM_PD_EN_MASK + PCRMU_PMU_ANA_PD_EN_MASK + PCRMU_FLASH_PD_EN_MASK +
                   PCRMU_CACHE_RET1N_EN_MASK + PCRMU_WUTMR_WU_MODEM_EN_MASK)) |
          ((mcu_pd_en << PCRMU_PMU_MCU_PD_EN_POS) & PCRMU_PMU_MCU_PD_EN_MASK) | 
          ((mem_pd_en << PCRMU_PMU_MEM_PD_EN_POS) & PCRMU_PMU_MEM_PD_EN_MASK) |
          ((ana_pd_en << PCRMU_PMU_ANA_PD_EN_POS) & PCRMU_PMU_ANA_PD_EN_MASK) | 
          ((flh_pd_en << PCRMU_FLASH_PD_EN_POS) & PCRMU_FLASH_PD_EN_MASK) |
          ((cache_pd_en << PCRMU_CACHE_RET1N_EN_POS) & PCRMU_CACHE_RET1N_EN_MASK) |
          ((0x1 << PCRMU_WUTMR_WU_MODEM_EN_POS) & PCRMU_WUTMR_WU_MODEM_EN_MASK);
    //	cfg |= PCRMU_WUTMR_WU_MODEM_EN_MASK;
    REG_W32(PCRMU_PMU_EN_REG_ADDR, cfg);

    // 0x40009118, PMU_EN2_REG
    cfg = REG_R32(PCRMU_PMU_EN2_REG_ADDR);
    cfg = (cfg & ~(PCRMU_MEM_PD_UTL_MODEM_MASK + PCRMU_MEM_PD_UTL_PERIPH_MASK + PCRMU_MEM_PD_UTL_NFC_MASK + PCRMU_ANA_PD_UTL_MODEM_MASK +
                   PCRMU_ANA_PD_UTL_PERIPH_MASK + PCRMU_ANA_PD_UTL_NFC_MASK + PCRMU_MODEM_PD_WI_CPU_MASK + PCRMU_PERIPH_PD_WI_CPU_MASK +
                   PCRMU_NFC_PD_WI_CPU_MASK + PCRMU_MODEM_WU_WI_CPU_MASK + PCRMU_PERIPH_WU_WI_CPU_MASK + PCRMU_NFC_WU_WI_CPU_MASK +
                   PCRMU_UART0_WU_EN_MASK + PCRMU_SPI0_WU_EN_MASK + PCRMU_UART0_RET_EN_MASK + PCRMU_SPI0_RET_EN_MASK + PCRMU_TIM_OUT_EN_MASK)) |
          ((mem_pd_utl_modem << PCRMU_MEM_PD_UTL_MODEM_POS) & PCRMU_MEM_PD_UTL_MODEM_MASK) |
          ((mem_pd_utl_periph << PCRMU_MEM_PD_UTL_PERIPH_POS) & PCRMU_MEM_PD_UTL_PERIPH_MASK) |
          ((mem_pd_utl_nfc << PCRMU_MEM_PD_UTL_NFC_POS) & PCRMU_MEM_PD_UTL_NFC_MASK) |
          ((ana_pd_utl_modem << PCRMU_ANA_PD_UTL_MODEM_POS) & PCRMU_ANA_PD_UTL_MODEM_MASK) |
          ((ana_pd_utl_periph << PCRMU_ANA_PD_UTL_PERIPH_POS) & PCRMU_ANA_PD_UTL_PERIPH_MASK) |
          ((ana_pd_utl_nfc << PCRMU_ANA_PD_UTL_NFC_POS) & PCRMU_ANA_PD_UTL_NFC_MASK) |
          ((modem_pd_wi_cpu << PCRMU_MODEM_PD_WI_CPU_POS) & PCRMU_MODEM_PD_WI_CPU_MASK) |
          ((periph_pd_wi_cpu << PCRMU_PERIPH_PD_WI_CPU_POS) & PCRMU_PERIPH_PD_WI_CPU_MASK) |
          ((nfc_pd_wi_cpu << PCRMU_NFC_PD_WI_CPU_POS) & PCRMU_NFC_PD_WI_CPU_MASK) |
          ((modem_wu_wi_cpu << PCRMU_MODEM_WU_WI_CPU_POS) & PCRMU_MODEM_WU_WI_CPU_MASK) |
          ((periph_wu_wi_cpu << PCRMU_PERIPH_WU_WI_CPU_POS) & PCRMU_PERIPH_WU_WI_CPU_MASK) |
          ((nfc_wu_wi_cpu << PCRMU_NFC_WU_WI_CPU_POS) & PCRMU_NFC_WU_WI_CPU_MASK) |
          ((uart0_wu_en << PCRMU_UART0_WU_EN_POS) & PCRMU_UART0_WU_EN_MASK) | 
          ((spi0_wu_en << PCRMU_SPI0_WU_EN_POS) & PCRMU_SPI0_WU_EN_MASK) |
          ((uart0_ret_en << PCRMU_UART0_RET_EN_POS) & PCRMU_UART0_RET_EN_MASK) | 
          ((spi0_ret_en << PCRMU_SPI0_RET_EN_POS) & PCRMU_SPI0_RET_EN_MASK) |
          ((tim_out_en << PCRMU_TIM_OUT_EN_POS) & PCRMU_TIM_OUT_EN_MASK);

    REG_W32(PCRMU_PMU_EN2_REG_ADDR, cfg);

    pmu_lock();

    if (mode == PWR_MODE_SLEEP) {
        //        printf("PWR_MODE_SLEEP\n");
        /* Set the processor uses deep sleep as its low power mode */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

        /* Chip enter Power-down mode after CPU run WFI instruction */
        __WFI();

        while (_pmu_get_status() != PMU_WAKEUP_STATE)
            ;
    }

    //	if (mode == PWR_MODE_DEEP_SLEEP) {
    //		PMU_INFO("PWR_MODE_DEEP_SLEEP");
    //	}
}

void pmu_set_sw_reset_er8130(void)
{
    pmu_unlock();
    REG_W32(PCRMU_CHIP_SWRST_REG_ADDR, 1);
}

void pmu_module_clk_enable_er8130(uint32_t module_idx)
{
    uint32_t cur_clk_gate;

    pmu_unlock();
    cur_clk_gate  = REG_R32(PCRMU_DMA_CLK_EN_ADDR);
    cur_clk_gate |= module_idx;
    REG_W32(PCRMU_DMA_CLK_EN_ADDR, cur_clk_gate);
    pmu_lock();
}

void pmu_module_clk_disable_er8130(uint32_t module_idx)
{
    uint32_t cur_clk_gate;

    if (module_idx == DMA_MODULE && !dma_is_all_channel_disable_er8130()) {
        return; // Do not disable DMA clock if any channel is still enabled
    }

    pmu_unlock();
    cur_clk_gate  = REG_R32(PCRMU_DMA_CLK_EN_ADDR);
    cur_clk_gate &= (~module_idx);
    REG_W32(PCRMU_DMA_CLK_EN_ADDR, cur_clk_gate);
    pmu_lock();
}

bool pmu_module_clk_is_enabled_er8130(uint32_t module_idx)
{
    return ((REG_R32(PCRMU_DMA_CLK_EN_ADDR) & module_idx) != 0);
}

void pmu_set_dyn_gate_er8130(uint8_t gate_idx)
{
    pmu_unlock();

    REG_W32(PCRMU_MODEM_DYN_GATED_EN_ADDR, gate_idx);

    pmu_lock();
}

void pmu_set_clk_src_align_er8130(uint8_t clk_src)
{
    pmu_unlock();

    REG_W32(PCRMU_ALIGN_CLK_SRC_ADDR, clk_src);

    pmu_lock();
}

void pmu_set_mcu_clk_factor_er8130(uint8_t clk_fact)
{
    pmu_unlock();
    REG_W32(PCRMU_MCU_CLK_FAC_ADDR, clk_fact);
    pmu_lock();
}

void pmu_set_bbp_cic_clk_er8130(uint8_t osr, uint8_t dr_idx)
{
    // NOTE::
    // Real chip's HW supports ADC_CLK for 8 MHz,
    // so it will bypass the code below on FPGA version
    //#if ADC_CLK < 8000000
    uint32_t cic_table[2][9] = {{0x00010508, 0x0001010A, 0x00010105, 0x00010104, 0x00010205, 0x00010102, 0x00010101, 0x00010101, 0x00010101},
                                // NOTE:: not verified at osr 16
                                {0x0001080A, 0x0001020A, 0x0001010A, 0x00010108, 0x00010105, 0x00010104, 0x00010102, 0x00010102, 0x00010101}};
    uint8_t osr_sel          = (osr == OSR8) ? 0 : 1;

    pmu_unlock();
    REG_W32(PCRMU_BBP_RATE_R1_ADDR, cic_table[osr_sel][dr_idx]);
    pmu_lock();
    //#endif
}

void pmu_set_bbp_mbus_cic_clk_er8130(uint8_t dr_idx)
{
    uint32_t cic_table[3] = {0x1A01040D, 0x08010404, 0x0501010A};

    pmu_unlock();
    REG_W32(PCRMU_BBP_RATE_R1_ADDR, cic_table[dr_idx]);
    pmu_lock();
}

void pmu_set_bbp_rate_er8130(uint8_t phy_mode, uint8_t mod_type, uint32_t data_rate, uint8_t mod_idx)
{
    uint32_t bbp_rate = 0;

    if (phy_mode == PHY_MODE_TRANSPARENT || phy_mode == PHY_MODE_802154) {
        switch (data_rate) {
        case DATA_RATE_12P5K:
            bbp_rate = 0x00010508;
            //                if (mod_idx == MOD_IDX_4) {
            //                    bbp_rate = 0x00010504;
            //                }
            break;
        case DATA_RATE_50K:
            bbp_rate = 0x0001020A;
            break;
        case DATA_RATE_100K:
            bbp_rate = 0x0001010A;
            break;
        case DATA_RATE_125K:
            bbp_rate = 0x00010108;
            break;
        case DATA_RATE_250K:
            bbp_rate = 0x00010104;
            break;
        case DATA_RATE_500K:
            bbp_rate = 0x00010102;
            break;
        case DATA_RATE_1M:
        case DATA_RATE_2M:
        default:
            bbp_rate = 0x00010101;
            break;
        }
    } else if (phy_mode == PHY_MODE_MBUS) {
        if (mod_type == MOD_TYPE_GFSK) {
            switch (data_rate) {
                case DATA_RATE_100K:
                    bbp_rate = 0x0501010A;
                    break;
                case DATA_RATE_50K:
                    bbp_rate = 0x00010105;
                    break;
            }
        } else if (mod_type == MOD_TYPE_FSK) {
        switch (data_rate) {
            case DATA_RATE_4P8K:
                bbp_rate = 0x1A01040D;
                break;
            case DATA_RATE_32P768K:
                bbp_rate = 0x08010404;
                break;
            case DATA_RATE_100K:
            default:
                bbp_rate = 0x0501010A;
                break;
            }
        }
    } else {
        bbp_rate = 0x00010101; // fallback
    }

    pmu_unlock();
    REG_W32(PCRMU_BBP_RATE_R1_ADDR, bbp_rate);
    pmu_lock();
}

void pmu_set_csr_clk_gated_er8130(uint8_t on_off)
{
    pmu_unlock();

    // 0x004
    REG_W32(PCRMU_PCLKG_DYN_GATED_EN_ADDR, on_off);

    pmu_lock();
}

void pmu_set_bod_reset_en_er8130(uint8_t enable)
{
    uint32_t reg;

    pmu_unlock();

    reg = REG_R32(PCRMU_BOD_CTRL_REG_ADDR) & ~PCRMU_BOD_RESET_EN_MASK;
    if (enable) {
        reg |= PCRMU_BOD_RESET_EN_MASK;
    }
    REG_W32(PCRMU_BOD_CTRL_REG_ADDR, reg);

    pmu_lock();
}

void pmu_set_interrupt_er8130(uint32_t source)
{
    pmu_unlock();
    REG_W32(PCRMU_INT_EN_ADDR, source);
    NVIC_EnableIRQ((IRQn_Type)PCRMU_IRQn);
    pmu_lock();
}

uint32_t pmu_get_interrupt_status_er8130()
{
    return REG_R32(PCRMU_INT_ST_ADDR);
}

void pmu_clear_interrupt_status_er8130(uint32_t source)
{
    pmu_unlock();
    REG_W32(PCRMU_INT_ST_ADDR, source);
    pmu_lock();
}

void pmu_module_sw_reset_er8130(uint32_t module)
{
    uint32_t val;

    //bit4 uart & bit19 cannot do reset
    if (module == 0xFFFFFFFF) {
        module = 0xFFF7FFEF;
    }

    pmu_unlock();

    val  = REG_R32(PCRMU_MODULE_SWRST_REG_ADDR);
    val |= module;
    REG_W32(PCRMU_MODULE_SWRST_REG_ADDR, val);

    // need to toggle to 0
    REG_W32(PCRMU_MODULE_SWRST_REG_ADDR, 0x0);

    pmu_lock();
}
/**
 * @brief Set the internal clock source.
 * @param clk_src [0] : 32KHz, [1] : 16KHz
 */
void pmu_clk_src_sel_er8130(uint8_t clk_src)
{
#if 0
    uint32_t reg;

    pmu_unlock();

    reg  = REG_R32(PCRMU_CLK_SRC_32K_REG_ADDR);
    reg &= ~PCRMU_CLK_SRC_32K_IEB_MASK;
    reg &= ~PCRMU_CLK_SRC_32K16K_MASK;

    if (clk_src == HDL_PMU_INTCLK_SRC_32K) {
        reg |= PCRMU_CLK_SRC_32K_IEB_MASK;
    } else {
        reg |= PCRMU_CLK_SRC_32K16K_MASK;
        reg |= PCRMU_CLK_SRC_32K_IEB_MASK;
    }
    REG_W32(PCRMU_CLK_SRC_32K_REG_ADDR, reg);

    pmu_lock();
#endif

    uint32_t reg;
    uint8_t src_32k; //1=internal, 0=external
    uint8_t src_16k;

    uint8_t src_32k_pwr;
    uint8_t src_16k_pwr;
    uint8_t ext_32k_pwr;

    switch(clk_src){
    case HDL_PMU_INTCLK_SRC_32K: //SCLK_SRC_32K:
        src_32k = 1;
        src_16k = 0;
        src_32k_pwr = 1;
        src_16k_pwr = 0;
        ext_32k_pwr = 0;
        break;
    case HDL_PMU_INTCLK_SRC_16K: //SCLK_SRC_16K:
        src_32k = 1;
        src_16k = 1;
        src_32k_pwr = 0;
        src_16k_pwr = 1;
        ext_32k_pwr = 0;
        break;
    case HDL_PMU_EXTCLK_SRC_32K: //SCLK_SRC_EXT32K:
        src_32k = 0;
        src_16k = 0;
        src_32k_pwr = 0;
        src_16k_pwr = 0;
        ext_32k_pwr = 1;
        break;
    default:
        printf("sclk_idx is bad = %d\n", clk_src);
        src_32k = 1;
        src_16k = 0;
        src_32k_pwr = 1;
        src_16k_pwr = 0;
        ext_32k_pwr = 0;
        break;
    }

    pmu_unlock();
    reg = REG_R32(PCRMU_CLK_SRC_32K_REG_ADDR);
    reg = (reg & ~(PCRMU_CLK_SRC_32K_IEB_MASK + PCRMU_CLK_SRC_32K16K_MASK)) |
          ((src_32k << PCRMU_CLK_SRC_32K_IEB_POS)&PCRMU_CLK_SRC_32K_IEB_MASK) |
          ((src_16k << PCRMU_CLK_SRC_32K16K_POS) &PCRMU_CLK_SRC_32K16K_MASK);
    REG_W32(PCRMU_CLK_SRC_32K_REG_ADDR, reg);
    pmu_lock();

    //enable internal 32K power down
    ana_control_32kosc_power(src_32k_pwr);

    //enable internal 16K power down
    ana_control_16kosc_power(src_16k_pwr);

    //disable ana xtal power down
    reg = REG_R32(ANA_XO_CONFIG_2_ADDR);
    reg = (reg & ~(ANA_LV_LPXTAL_PD_MASK)) |
          ((~ext_32k_pwr << ANA_LV_LPXTAL_PD_POS)&ANA_LV_LPXTAL_PD_MASK);
    REG_W32(ANA_XO_CONFIG_2_ADDR, reg);
}

void pmu_lfosc_clk_src_disable_er8130()
{
    uint32_t reg;
    uint8_t src_32k_pwr = 0;
    uint8_t src_16k_pwr = 0;
    uint8_t ext_32k_pwr = 0;
    
    //enable internal 32K power down
    ana_control_32kosc_power(src_32k_pwr);

    //enable internal 16K power down
    ana_control_16kosc_power(src_16k_pwr);

    //disable xtal power down (debug regiseter)
    reg = REG_R32(0x40011584);
    reg = (reg & ~(BIT28)) |
          ((~ext_32k_pwr << 28)&BIT28);
    REG_W32(0x40011584, reg);

}

void pmu_switch_nfc_pwr_src_er8130(uint8_t src)
{
    uint32_t reg;
    uint8_t nfc_swrst;
    uint8_t efu_swrst;

    if (src == NFC_PWR_SRC_MCU) {
        //NFC_PWR_SRC_MCU
        //============= NFC digital engine switch back to system power ==================//
        //- Let NFC engine use NFC power

        nfc_swrst = 0;
        efu_swrst = 0;

        //- Release NFC, eFuse's reset
        pmu_unlock();

        reg = REG_R32(PCRMU_MODULE_SWRST_REG_ADDR);
        reg = (reg & ~(PCRMU_NFC_SWRST_MASK + PCRMU_EFUCTL_SWRST_MASK)) | 
              ((nfc_swrst << PCRMU_NFC_SWRST_POS) & PCRMU_NFC_SWRST_MASK) |
              ((efu_swrst << PCRMU_EFUCTL_SWRST_POS) & PCRMU_EFUCTL_SWRST_MASK);
        REG_W32(PCRMU_MODULE_SWRST_REG_ADDR, reg);

        pmu_lock();
    }

    if (src == NFC_PWR_SRC_READER) {
        //NFC_PWR_SRC_READER
        //============= NFC digital engine switch to NFC self power ==================//
        //- Assert NFC, eFuse's reset

        nfc_swrst = 1;
        efu_swrst = 1;

        pmu_unlock();

        reg = REG_R32(PCRMU_MODULE_SWRST_REG_ADDR);
        reg = (reg & ~(PCRMU_NFC_SWRST_MASK + PCRMU_EFUCTL_SWRST_MASK)) | 
              ((nfc_swrst << PCRMU_NFC_SWRST_POS) & PCRMU_NFC_SWRST_MASK) |
              ((efu_swrst << PCRMU_EFUCTL_SWRST_POS) & PCRMU_EFUCTL_SWRST_MASK);
        REG_W32(PCRMU_MODULE_SWRST_REG_ADDR, reg);

        pmu_lock();
    }
}

void pmu_set_cpu_pd_er8130(void)
{
    uint32_t reg;

    //- Enable cache enter retain mode
    //- 0x40009100 [6]
    //PCRMU->PMU_EN_REG_b.CACHE_RET1N_EN = 1;

    //- Enable MCU PD
    //- 0x40009100 [0]
    //PCRMU->PMU_EN_REG_b.PMU_MCU_PD_EN   = 1;
    pmu_unlock();

    reg = REG_R32(PCRMU_PMU_EN_REG_ADDR);
    reg = (reg & ~(PCRMU_PMU_MCU_PD_EN_MASK + PCRMU_CACHE_RET1N_EN_MASK)) | ((0x1 << PCRMU_PMU_MCU_PD_EN_POS) & PCRMU_PMU_MCU_PD_EN_MASK) |
          ((0x1 << PCRMU_CACHE_RET1N_EN_POS) & PCRMU_CACHE_RET1N_EN_MASK);
    REG_W32(PCRMU_PMU_EN_REG_ADDR, reg);

    pmu_lock();

    // Ensure we use deep SLEEP - SLEEPDEEP should be set
    // SCR[2] = SLEEPDEEP
    SCB->SCR |= (1UL << 2);

    //- let CPU go to sleep and power down CPU domain
    __WFI();
}

void pmu_module_reset_er8130(uint32_t module_idx)
{
    uint32_t reg;

    pmu_unlock();
    reg  = REG_R32(PCRMU_MODULE_SWRST_REG_ADDR);
    reg |= module_idx;
    REG_W32(PCRMU_MODULE_SWRST_REG_ADDR, reg);
    for (int i = 0; i < 10000; i++) {
        ; // wait for reset to complete
    }
    reg &= ~module_idx;
    REG_W32(PCRMU_MODULE_SWRST_REG_ADDR, reg);
    pmu_lock();
}

void pmu_reset_module_clock_er8130(void)
{
    pmu_unlock();
    REG_W32(PCRMU_MODULE_CLK_GATE_REG_ADDR, 0x0);
    pmu_lock();
}

void pmu_reset_module_core_er8130(uint32_t module_idx)
{
    uint32_t reg;

    pmu_unlock();
    reg  = REG_R32(PCRMU_MODULE_CORE_SWRST_REG_ADDR);
    reg |= module_idx;
    REG_W32(PCRMU_MODULE_CORE_SWRST_REG_ADDR, reg);
    reg &= ~module_idx;
    REG_W32(PCRMU_MODULE_CORE_SWRST_REG_ADDR, reg);
    pmu_lock();
}
