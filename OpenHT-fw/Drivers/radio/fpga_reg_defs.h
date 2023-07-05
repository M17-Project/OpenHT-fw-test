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
#define IO3_LOW						((uint16_t)0<<9)
#define IO3_DRDY					((uint16_t)1<<9)
#define IO3_FIFO_AE					((uint16_t)5<<9)
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
#define CTCSS_TX_71_9				((uint16_t)3<<2)
#define CTCSS_TX_74_4				((uint16_t)4<<2)
#define CTCSS_TX_77_0				((uint16_t)5<<2)
#define CTCSS_TX_79_7				((uint16_t)6<<2)
#define CTCSS_TX_82_5				((uint16_t)7<<2)
#define CTCSS_TX_85_4				((uint16_t)8<<2)
#define CTCSS_TX_88_5				((uint16_t)9<<2)
#define CTCSS_TX_91_5				((uint16_t)10<<2)
#define CTCSS_TX_94_8				((uint16_t)11<<2)
#define CTCSS_TX_97_4				((uint16_t)12<<2)
#define CTCSS_TX_100_0				((uint16_t)13<<2)
#define CTCSS_TX_103_5				((uint16_t)14<<2)
#define CTCSS_TX_107_2				((uint16_t)15<<2)
#define CTCSS_TX_110_9				((uint16_t)16<<2)
#define CTCSS_TX_114_8				((uint16_t)17<<2)
#define CTCSS_TX_118_8				((uint16_t)18<<2)
#define CTCSS_TX_123_0				((uint16_t)19<<2)
#define CTCSS_TX_127_3				((uint16_t)20<<2)
#define CTCSS_TX_131_8				((uint16_t)21<<2)
#define CTCSS_TX_136_5				((uint16_t)22<<2)
#define CTCSS_TX_141_3				((uint16_t)23<<2)
#define CTCSS_TX_146_2				((uint16_t)24<<2)
#define CTCSS_TX_151_4				((uint16_t)25<<2)
#define CTCSS_TX_156_7				((uint16_t)26<<2)
#define CTCSS_TX_159_8				((uint16_t)27<<2)
#define CTCSS_TX_162_2				((uint16_t)28<<2)
#define CTCSS_TX_165_5				((uint16_t)29<<2)
#define CTCSS_TX_167_9				((uint16_t)30<<2)
#define CTCSS_TX_171_3				((uint16_t)31<<2)
#define CTCSS_TX_173_8				((uint16_t)32<<2)
#define CTCSS_TX_177_3				((uint16_t)33<<2)
#define CTCSS_TX_179_9				((uint16_t)34<<2)
#define CTCSS_TX_183_5				((uint16_t)35<<2)
#define CTCSS_TX_186_2				((uint16_t)36<<2)
#define CTCSS_TX_189_9				((uint16_t)37<<2)
#define CTCSS_TX_192_8				((uint16_t)38<<2)
#define CTCSS_TX_196_6				((uint16_t)39<<2)
#define CTCSS_TX_199_5				((uint16_t)40<<2)
#define CTCSS_TX_203_5				((uint16_t)41<<2)
#define CTCSS_TX_206_5				((uint16_t)42<<2)
#define CTCSS_TX_210_7				((uint16_t)43<<2)
#define CTCSS_TX_218_1				((uint16_t)44<<2)
#define CTCSS_TX_225_7				((uint16_t)45<<2)
#define CTCSS_TX_229_1				((uint16_t)46<<2)
#define CTCSS_TX_233_6				((uint16_t)47<<2)
#define CTCSS_TX_241_8				((uint16_t)48<<2)
#define CTCSS_TX_250_3				((uint16_t)49<<2)
#define CTCSS_TX_254_1				((uint16_t)50<<2)
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
