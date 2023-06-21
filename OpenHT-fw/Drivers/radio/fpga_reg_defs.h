//----------------------------------------------------
// OpenHT's FPGA register #defines set
//
// W. Kaczmarski SP5WWP
// M17 Project, June 2023
//----------------------------------------------------

#ifndef FPGA_REG_DEFS
#define FPGA_REF_DEFS

#include <stdint.h>

//Read/Write
#define REG_RD						((uint16_t)0<<7)
#define REG_WR						((uint16_t)1<<7)
#define ADDR_NORM					((uint16_t)0<<6)
#define ADDR_INCR					((uint16_t)1<<6)

//Register 0 - Control Register 1
#define CR_1						((uint16_t)0)
#define SSB_USB						((uint16_t)0<<15)
#define SSB_LSB						((uint16_t)1<<15)
#define MOD_FM						((uint16_t)0<<12)
#define MOD_AM						((uint16_t)1<<12)
#define MOD_SSB						((uint16_t)2<<12)
#define IO0_LOW						((uint16_t)0<<9)
#define IO0_DRDY					((uint16_t)1<<9)
#define PD_OFF						((uint16_t)0<<5)
#define PD_ON						((uint16_t)1<<5)
#define DEM_FM						((uint16_t)0<<2)
#define DEM_AM						((uint16_t)1<<2)
#define DEM_SSB						((uint16_t)2<<2)
#define BAND_09						((uint16_t)0<<0)
#define BAND_24						((uint16_t)1<<0)

//Register 1 - Control Register 2
#define CR_2						((uint16_t)0x0100) // Addr 01
#define CH_RX_6_25					((uint16_t)0<<9)
#define CH_RX_12_5					((uint16_t)1<<9)
#define CH_RX_25					((uint16_t)2<<9)
#define FM_TX_N						((uint16_t)0<<8)
#define FM_TX_W						((uint16_t)1<<8)
#define CTCSS_TX_NONE				((uint16_t)0<<2)
#define CTCSS_TX_67_0				((uint16_t)1<<2)
#define CTCSS_TX_69_3				((uint16_t)2<<2)
/* TODO: fill this with the rest of the CTCSS tones */
#define CTCSS_TX_254_1				((uint16_t)63<<2)
#define STATE_IDLE					((uint16_t)0<<0)
#define STATE_TX					((uint16_t)1<<0)
#define STATE_RX					((uint16_t)2<<0)

//Register 2 - I branch offset null
#define I_OFFS_NULL					((uint16_t)0x0200) // Addr 02

//Register 3 - Q branch offset null
#define Q_OFFS_NULL					((uint16_t)0x0300) // Addr 03

//Register 4 - I branch gain
#define I_GAIN						((uint16_t)0x0400) // Addr 04

//Register 5 - Q branch gain
#define Q_GAIN						((uint16_t)0x0500) // Addr 05

//Register 6 - Digital Predistortion Register 1
#define DPD_1						((uint16_t)0x0600) // Addr 06

//Register 7 - Digital Predistortion Register 2
#define DPD_2						((uint16_t)0x0700) // Addr 07

//Register 8 - Digital Predistortion Register 3
#define DPD_3						((uint16_t)0x0800) // Addr 08

//Register 9 - Modulation Word Register
#define MOD_IN						((uint16_t)0x0900) // Addr 09

//Registers 10-12 are reserved for later use

//Register 13 - Status Register 1
#define SR_1						((uint16_t)0x0D00) // Addr 13

//Register 14 - Status Register 2
#define SR_2						((uint16_t)0x0E00) // Addr 14
#define PLL0_LOCK					((uint16_t)1<<0)
#define PLL1_LOCK					((uint16_t)1<<1)

//Register 15 - Demodulation Register
#define DEMOD						((uint16_t)0x0F00) // Addr 15

//Register 16 - RSSI Register
#define RSSI						((uint16_t)0x1000) // Addr 16

//Register 17 - Raw I branch readout
#define I_RAW						((uint16_t)0x1100) // Addr 17

//Register 18 - Raw Q branch readout
#define Q_RAW						((uint16_t)0x1200) // Addr 18

//Register 19 - Filtered I branch readout
#define I_FLT						((uint16_t)0x1300) // Addr 19

//Register 20 - Filtered Q branch readout
#define Q_FLT						((uint16_t)0x1400) // Addr 20

#endif
