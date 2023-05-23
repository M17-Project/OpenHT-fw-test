//----------------------------------------------------
// OpenHT's FPGA register #defines set
//
// W. Kaczmarski SP5WWP
// M17 Project, May 2023
//----------------------------------------------------

#ifndef FPGA_REG_DEFS
#define FPGA_REF_DEFS

#include <stdint.h>

//Read/Write
#define REG_WR						((uint16_t)1<<15)
#define REG_RD						((uint16_t)0<<15)

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
#define CR_2						((uint16_t)1)
#define CTCSS_TX_NONE				((uint16_t)0<<2)
#define CTCSS_TX_67_0				((uint16_t)1<<2)
#define CTCSS_TX_69_3				((uint16_t)2<<2)
/* TODO: fill this with the rest of the CTCSS tones */
#define CTCSS_TX_254_1				((uint16_t)63<<2)
#define STATE_IDLE					((uint16_t)0<<0)
#define STATE_TX					((uint16_t)1<<0)
#define STATE_RX					((uint16_t)2<<0)

//Register 2 - I branch offset null
#define I_OFFS_NULL					((uint16_t)2)

//Register 3 - Q branch offset null
#define Q_OFFS_NULL					((uint16_t)3)

//Register 4 - I branch gain
#define I_GAIN						((uint16_t)4)

//Register 5 - Q branch gain
#define Q_GAIN						((uint16_t)5)

//Register 6 - Digital Predistortion Register 1
#define DPD_1						((uint16_t)6)

//Register 7 - Digital Predistortion Register 2
#define DPD_2						((uint16_t)7)

//Register 8 - Digital Predistortion Register 3
#define DPD_3						((uint16_t)8)

//Register 9 - Modulation Word Register
#define MOD_IN						((uint16_t)9)

//Registers 10-12 are reserved for later use

//Register 13 - Status Register 1
#define SR_1						((uint16_t)13)

//Register 14 - Status Register 2
#define SR_2						((uint16_t)14)
#define PLL0_LOCK					((uint16_t)1<<0)
#define PLL1_LOCK					((uint16_t)1<<1)

//Register 15 - Demodulation Register
#define DEMOD						((uint16_t)15)

//Register 16 - RSSI Register
#define RSSI						((uint16_t)16)

//Register 17 - I branch raw readout
#define I_RAW						((uint16_t)17)

//Register 18 - Q branch raw readout
#define Q_RAW						((uint16_t)18)

#endif
