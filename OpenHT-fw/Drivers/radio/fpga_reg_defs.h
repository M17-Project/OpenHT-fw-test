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
#define APB_DECODE_BITS 3


//Read/Write
#define REG_RD						((uint16_t)0<<15)
#define REG_WR						((uint16_t)1<<15)
#define ADDR_NORM					((uint16_t)0<<14)
#define ADDR_INCR					((uint16_t)1<<14)

#define REG(slave, reg) ((slave << (14 - APB_DECODE_BITS)) | reg)

#define COM_VERSION REG(0, 0)
#define COM_STATUS REG(0, 1)
#define COM_CTRL REG(0, 2)
#define COM_CTRL_IDLE (0)
#define COM_CTRL_TX (1)
#define COM_CTRL_RX (2)

#define COM_IO REG(0, 3)
#define COM_IO_IO3_PLL_LOCK (0)
#define COM_IO_IO3_TX_AE (5)

#define COM_TX_FIFO REG(0, 4)
#define COM_TX_FIFO_STS REG(0, 5)

// TX Chain
#define TX_CTRL REG(1, 0)
#define TX_CTRL_MOD_FM (0)
#define TX_CTRL_MOD_AM (1)
#define TX_CTRL_MOD_SSB (2)
#define TX_CTRL_FMN (0 << 3)
#define TX_CTRL_FMW (1 << 3)
#define TX_CTRL_LSB (0 << 4)
#define TX_CTRL_USB (1 << 4)

#define TX_GAIN_I REG(2, 0)
#define TX_GAIN_Q REG(2, 1)

#define TX_OFFSET_I REG(3, 0)
#define TX_OFFSET_Q REG(3, 1)

#define TX_CTCSS_CTRL REG(4, 0)
#define TX_CTCSS_CTRL_DISABLED (0 << 0)
#define TX_CTCSS_CTRL_ENABLED (1 << 0)
#define TX_CTCSS_CTRL_IN_CONST (0 << 1)
#define TX_CTCSS_CTRL_IN_STREAM (1 << 1)
#define TX_CTCSS_CTRL_REPLACE (0 << 2)
#define TX_CTCSS_CTRL_ADD (1 << 2)

#define TX_CTCSS_AMPLITUDE REG(4, 1)
#define TX_CTCSS_FREQ REG(4, 2)

#endif
