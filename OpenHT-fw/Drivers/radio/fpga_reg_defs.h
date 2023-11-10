//----------------------------------------------------
// OpenHT's FPGA register #defines set
//
// W. Kaczmarski SP5WWP
// Sebastien Van Cauwenberghe, ON4SEB
// M17 Project, June 2023
//----------------------------------------------------

#ifndef FPGA_REG_DEFS
#define FPGA_REF_DEFS

#include <stdint.h>

// Match with FPGA regs
#define APB_DECODE_BITS 4


//Read/Write
#define REG_RD						((uint16_t)0<<15)
#define REG_WR						((uint16_t)1<<15)
#define ADDR_NORM					((uint16_t)0<<14)
#define ADDR_INCR					((uint16_t)1<<14)

// APB slaves list
#define SLV_COM (0)
#define SLV_TX_COM (1)
#define SLV_TX_FIR0 (2)
#define SLV_TX_CTCSS (3)
#define SLV_TX_FIR1 (4)
#define SLV_TX_FIR2 (5)
#define SLV_TX_FIR3 (6)
#define SLV_TX_IQ_GAIN (7)
#define SLV_TX_IQ_OFFSET (8)

#define REG(slave, reg) ((slave << (14 - APB_DECODE_BITS)) | reg)

// Filter definition macros
#define FILTER_CTRL (0)
#define FILTER_TAPS_CNT (1)
#define FILTER_L (2)
#define FILTER_M (3)
#define FILTER_TAPS_ADDR (4)
#define FILTER_TAPS_DATA (5)
#define FILTER_ACC_SHIFT_I (6)
#define FILTER_ACC_SHIFT_Q (7)

#define _REG_FILTER_CTRL(slave) (REG(slave, 0))
#define _REG_FILTER_TAPS_CNT(slave) (REG(slave, 1))
#define _REG_FILTER_L(slave) (REG(slave, 2))
#define _REG_FILTER_M(slave) (REG(slave, 3))
#define _REG_FILTER_TAPS_ADDR(slave) (REG(slave, 4))
#define _REG_FILTER_TAPS_DATA(slave) (REG(slave, 5))
#define _REG_FILTER_ACC_SHIFT_I(slave) (REG(slave, 6))
#define _REG_FILTER_ACC_SHIFT_Q(slave) (REG(slave, 7))

#define FILTER_ENABLE (1 << 0)
#define FILTER_DISABLE (0 << 0)
#define FILTER_PASS_IQ (0 << 1)
#define FILTER_DUPLICATE_I (1 << 1)

// FPGA Common info
#define COM_VERSION REG(SLV_COM, 0)
#define COM_STATUS REG(SLV_COM, 1)
#define COM_CTRL REG(SLV_COM, 2)
#define COM_CTRL_IDLE (0)
#define COM_CTRL_TX (1)
#define COM_CTRL_RX (2)

#define COM_IO REG(SLV_COM, 3)
#define COM_IO_IO3_PLL_LOCK (0)
#define COM_IO_IO3_TX_AE (5)

#define COM_TX_FIFO REG(SLV_COM, 4)
#define COM_TX_FIFO_STS REG(SLV_COM, 5)

// TX Chain Common
#define TX_CTRL REG(SLV_TX_COM, 0)
#define TX_CTRL_MOD_FM (0)
#define TX_CTRL_MOD_AM (1)
#define TX_CTRL_MOD_SSB (2)
#define TX_CTRL_FMN (0 << 3)
#define TX_CTRL_FMW (1 << 3)
#define TX_CTRL_LSB (0 << 4)
#define TX_CTRL_USB (1 << 4)

// TX Prefilter
#define TX_FIR0_CTRL _REG_FILTER_CTRL(2)
#define TX_FIR0_TAPS_CNT _REG_FILTER_TAPS_CNT(2)
#define TX_FIR0_L _REG_FILTER_L(2)
#define TX_FIR0_M _REG_FILTER_M(2)
#define TX_FIR0_TAPS_ADDR _REG_FILTER_TAPS_ADDR(2)
#define TX_FIR0_TAPS_DATA _REG_FILTER_TAPS_DATA(2)
#define TX_FIR0_ACC_SHIFT_I _REG_FILTER_ACC_SHIFT_I(2)
#define TX_FIR0_ACC_SHIFT_Q _REG_FILTER_ACC_SHIFT_Q(2)

// TX CTCSS
#define TX_CTCSS_CTRL REG(SLV_TX_CTCSS, 0)
#define TX_CTCSS_CTRL_DISABLED (0 << 0)
#define TX_CTCSS_CTRL_ENABLED (1 << 0)
#define TX_CTCSS_CTRL_IN_CONST (0 << 1)
#define TX_CTCSS_CTRL_IN_STREAM (1 << 1)
#define TX_CTCSS_CTRL_REPLACE (0 << 2)
#define TX_CTCSS_CTRL_ADD (1 << 2)

#define TX_CTCSS_AMPLITUDE REG(SLV_TX_CTCSS, 1)
#define TX_CTCSS_FREQ REG(SLV_TX_CTCSS, 2)

// TX interpolators
#define TX_FIR1_CTRL _REG_FILTER_CTRL(4)
#define TX_FIR1_TAPS_CNT _REG_FILTER_TAPS_CNT(4)
#define TX_FIR1_L _REG_FILTER_L(4)
#define TX_FIR1_M _REG_FILTER_M(4)
#define TX_FIR1_TAPS_ADDR _REG_FILTER_TAPS_ADDR(4)
#define TX_FIR1_TAPS_DATA _REG_FILTER_TAPS_DATA(4)
#define TX_FIR1_ACC_SHIFT_I _REG_FILTER_ACC_SHIFT_I(4)
#define TX_FIR1_ACC_SHIFT_Q _REG_FILTER_ACC_SHIFT_Q(4)

#define TX_FIR2_CTRL _REG_FILTER_CTRL(5)
#define TX_FIR2_TAPS_CNT _REG_FILTER_TAPS_CNT(5)
#define TX_FIR2_L _REG_FILTER_L(5)
#define TX_FIR2_M _REG_FILTER_M(5)
#define TX_FIR2_TAPS_ADDR _REG_FILTER_TAPS_ADDR(5)
#define TX_FIR2_TAPS_DATA _REG_FILTER_TAPS_DATA(5)
#define TX_FIR2_ACC_SHIFT_I _REG_FILTER_ACC_SHIFT_I(5)
#define TX_FIR2_ACC_SHIFT_Q _REG_FILTER_ACC_SHIFT_Q(5)

#define TX_FIR3_CTRL _REG_FILTER_CTRL(6)
#define TX_FIR3_TAPS_CNT _REG_FILTER_TAPS_CNT(6)
#define TX_FIR3_L _REG_FILTER_L(6)
#define TX_FIR3_M _REG_FILTER_M(6)
#define TX_FIR3_TAPS_ADDR _REG_FILTER_TAPS_ADDR(6)
#define TX_FIR3_TAPS_DATA _REG_FILTER_TAPS_DATA(6)
#define TX_FIR3_ACC_SHIFT_I _REG_FILTER_ACC_SHIFT_I(6)
#define TX_FIR3_ACC_SHIFT_Q _REG_FILTER_ACC_SHIFT_Q(6)

// TX IQ Gain/Offset
#define TX_GAIN_I REG(SLV_TX_IQ_GAIN, 0)
#define TX_GAIN_Q REG(SLV_TX_IQ_GAIN, 1)

#define TX_OFFSET_I REG(SLV_TX_IQ_OFFSET, 0)
#define TX_OFFSET_Q REG(SLV_TX_IQ_OFFSET, 1)

#endif
