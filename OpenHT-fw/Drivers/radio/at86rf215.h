/*
 * Copyright (C) 2023 M17 Project and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef RADIO_AT86RF215_H_
#define RADIO_AT86RF215_H_

/* Registers are stored in reversed byte order so that
 * when used in a little endian system and converting a uint16_t
 * to a uint8_t[2] the bytes are in the correct order.*/

/* Registers definitions */
#define RF09_IRQS 			(0x0000)
#define RF24_IRQS			(0x0100)	// Addr 0x0001

#define RFn_IRQx_IQIFSF		(1<<5)		// Also used for
#define RFn_IRQx_TXRERR		(1<<4)		// RF09_IRQM
#define RFn_IRQx_BATLOW		(1<<3)		// and RF24_IRQM
#define RFn_IRQx_EDC		(1<<2)
#define RFn_IRQx_TXRXRDY	(1<<1)
#define RFn_IRQx_WAKEUP		(1<<0)

#define RF_RST				(0x0500)	// Addr 0x0005
#define RF_RST_CMD			(0x7)

#define RF_CFG				(0x0600)	// Addr 0x0006
#define RF_CFG_IRQMM		(1<<3)
#define RF_CFG_IRQP			(1<<2)
#define RF_CFG_DRV2			(0x0)
#define RF_CFG_DRV4			(0x1)
#define RF_CFG_DRV6			(0x2)
#define RF_CFG_DRV8			(0x3)

#define RF_CLKO				(0x0700)	// Addr 0x0007
#define RF_DRVCLKO2			(0x0<<3)
#define RF_DRVCLKO4			(0x1<<3)
#define RF_DRVCLKO6			(0x2<<3)
#define RF_DRVCLKO8			(0x3<<3)
#define RF_CLKO_OS_OFF  	(0x0)
#define RF_CLKO_OS_26MHZ	(0x1)
#define RF_CLKO_OS_32MHZ	(0x2)
#define RF_CLKO_OS_16MHZ	(0x3)
#define RF_CLKO_OS_08MHZ	(0x4)
#define RF_CLKO_OS_04MHZ	(0x5)
#define RF_CLKO_OS_02MHZ	(0x6)
#define RF_CLKO_OS_01MHZ	(0x7)

#define RF_BMDVC			(0x0800)	// Addr 0x0008
#define RF_BMDVC_BMS		(1<<5)
#define RF_BMDVC_BMHR		(1<<4)
	/* See datasheet for the BMVTH values */

#define RF_XOC				(0x00900)
#define RF_XOC_FS			(1<<4)
	/* TRIM is +0.3pF multiplied by the value of the TRIM field (4 bits)*/

#define RF_IQIFC0			(0x0A00)	// Addr 0x000A
#define RF_IQIFC0_EXTLB		(1<<7)
#define RF_IQIFC0_SF		(1<<6)
#define RF_IQIFC0_DRV1		(0x0<<4)
#define RF_IQIFC0_DRV2		(0x1<<4)
#define RF_IQIFC0_DRV3		(0x2<<4)
#define RF_IQIFC0_DRV4		(0x3<<4)
#define RF_IQIFC0_CMV150	(0x0<<2)
#define RF_IQIFC0_CMV200	(0x1<<2)
#define RF_IQIFC0_CMV250	(0x2<<2)
#define RF_IQIFC0_CMV300	(0x3<<2)
#define RF_IQIFC0_CMV1V2	(0x1<<1)
#define RF_IQIFC0_EEC		(0x1)

#define RF_IQIFC1			(0x0B00)	// Addr 0x000B
#define RF_IQIFC1_FAILSF	(0x1<<7)
#define RF_IQIFC1_BBRF 		(0x0<<4)
#define RF_IQIFC1_RF		(0x1<<4)
#define RF_IQIFC1_BBRF09	(0x4<<4)
#define RF_IQIFC1_BBRF24	(0x5<<4)
#define RF_IQIFC1_SKEWDRV19	(0x0)
#define RF_IQIFC1_SKEWDRV29 (0x1)
#define RF_IQIFC1_SKEWDRV39	(0x2)
#define RF_IQIFC1_SKEWDRV49 (0x3)


#define RF_IQIFC2			(0x0C00)	// Addr 0x000C
#define RF_IQIFC2_SYNC		(0x1<<7)

#define RF_PN				(0x0D00)	// Addr 0x000D
#define RF_VN				(0x0E00)	// Addr 0x000E

#define RF09_IRQM			(0x0001)	// Addr 0x0100
#define RF24_IRQM			(0x0002)	// Addr 0x0200

#define RF09_AUXS			(0x0101)	// Addr 0x0101
#define RF24_AUXS			(0x0102)	// Addr 0x0201
#define RFn_AUXS_EXTLNABYP	(0x1<<7)
#define RFn_AUXS_AGCMAPOFF	(0x0<<5)
#define RFn_AUXS_AGCMAP_9	(0x1<<5)
#define RFn_AUXS_AGCMAP_12	(0x2<<5)
#define RFn_AUXS_AVEXT		(0x1<<4)
#define RFn_AUXS_AVEN		(0x1<<3)
#define RFn_AUXS_AVS		(0x1<<2)
#define RFn_AUXS_PAVC20		(0x0)
#define RFn_AUXS_PAVC22		(0x1)
#define RFn_AUXS_PAVC24		(0x2)

#define RF09_STATE			(0x0201)	// Addr 0x0102
#define RF24_STATE			(0x0202)	// Addr 0x0202

#define RF09_CMD			(0x0301) 	// Addr 0x0103
#define RF24_CMD			(0x0302)	// Addr 0x0203
#define RFn_CMD_NOP			(0x0)
#define RFn_CMD_SLEEP		(0x1)
#define RFn_CMD_TRXOFF		(0x2)
#define RFn_CMD_TXPREP		(0x3)
#define RFn_CMD_TX			(0x4)
#define RFn_CMD_RX			(0x5)
#define RFn_CMD_RESET		(0x7)

#define RF09_CS				(0x0401)	// Addr 0x0104
#define RF24_CS				(0x0402)	// Addr 0x0204
	/* Channel spacing by increments of 25 kHz */

#define RF09_CCF0L			(0x0501)	// Addr 0x0105
#define RF24_CCF0L			(0x0502)	// Addr 0x0205
	/* Channel center frequency (low) by increment of 25kHz */

#define RF09_CCF0H			(0x0601)	// Addr 0x0106
#define RF24_CCF0H			(0x0602)	// Addr 0x0206
	/* Channel center frequency (high) by increment of 25kHz */

#define RF09_CNL			(0x0701)	// Addr 0x0107
#define RF24_CNL			(0x0702)	// Addr 0x0207
	/* Channel number low byte */

#define RF09_CNM			(0x0801)	// Addr 0x0108
#define RF24_CNM			(0x0802)	// Addr 0x0208
#define RFn_CNM_CM_IEEE		(0x0<<6)
#define RFn_CNM_CM_FINE1	(0x1<<6)
#define RFn_CNM_CM_FINE2	(0x2<<6)
#define RFn_CNM_CM_FINE3	(0x3<<6)
#define RFn_CNM_CNH			(0x1)

#define RF09_RXBWC			(0x0901)	// Addr 0x0109
#define RF24_RXBWC			(0x0902)	// Addr 0x0209
#define RFn_RXBWC_IFINV		(0x1<<5)
#define RFn_RXBWC_IFSHIFT	(0x1<<4)
#define RFn_RXBWC_BW160K	(0x0)
#define RFn_RXBWC_BW200K	(0x1)
#define RFn_RXBWC_BW250K	(0x2)
#define RFn_RXBWC_BW320K	(0x3)
#define RFn_RXBWC_BW400K	(0x4)
#define RFn_RXBWC_BW500K	(0x5)
#define RFn_RXBWC_BW630K	(0x6)
#define RFn_RXBWC_BW800K	(0x7)
#define RFn_RXBWC_BW1M		(0x8)
#define RFn_RXBWC_BW1M25	(0x9)
#define RFn_RXBWC_BW1M6		(0xA)
#define RFn_RXBWC_BW2M		(0xB)

#define RF09_RXDFE			(0x0A01)	// Addr 0x010A
#define RF24_RXDFE			(0x0A02)	// Addr 0x020A
#define RFn_RXDFE_RCUT_0_25	(0x0<<5)
#define RFn_RXDFE_RCUT_0_37	(0x1<<5)
#define RFn_RXDFE_RCUT_0_5	(0x2<<5)
#define RFn_RXDFE_RCUT_0_75	(0x3<<5)
#define RFn_RXDFE_RCUT_1	(0x4<<5)
#define RFn_RXDFE_SR_4M		(0x1)
#define RFn_RXDFE_SR_2M		(0x2)
#define RFn_RXDFE_SR_1M333	(0x3)
#define RFn_RXDFE_SR_1M		(0x4)
#define RFn_RXDFE_SR_800K	(0x5)
#define RFn_RXDFE_SR_666K66	(0x6)
#define RFn_RXDFE_SR_500K	(0x8)
#define RFn_RXDFE_SR_400K	(0xA)

#define RF09_AGCC			(0x0B01)	// Addr 0x010B
#define RF24_AGCC			(0x0B02)	// Addr 0x020B
#define RFn_AGCC_AGCI		(0x1<<6)
#define RFn_AGCC_AVGS8		(0x0<<4)
#define RFn_AGCC_AVGS16		(0x1<<4)
#define RFn_AGCC_AVGS32		(0x2<<4)
#define RFn_AGCC_AVGS64		(0x3<<4)
#define RFn_AGCC_RST		(0x1<<3)
#define RFn_AGCC_FRZS		(0x1<<2)
#define RFn_AGCC_FRZC		(0x1<<1)
#define RFn_AGCC_EN			(0x1)

#define RF09_AGCS			(0x0C01)	// Addr 0x010C
#define RF24_AGCS			(0x0C02)	// Addr 0x020C
#define RFn_AGCS_TGT_n21	(0x0<<5)
#define RFn_AGCS_TGT_n24	(0x1<<5)
#define RFn_AGCS_TGT_n27	(0x2<<5)
#define RFn_AGCS_TGT_n30	(0x3<<5)
#define RFn_AGCS_TGT_n33	(0x4<<5)
#define RFn_AGCS_TGT_n36	(0x5<<5)
#define RFn_AGCS_TGT_n39	(0x6<<5)
#define RFn_AGCS_TGT_n42	(0x7<<5)
	/* RFn_AGCS_GCW is the RX gain (by increments of 3dB)*/

#define RF09_RSSI			(0x0D01)	// Addr 0x010D
#define RF24_RSSI			(0x0D02)	// Addr 0x020D

#define RF09_EDC			(0x0E01) 	// Addr 0x010E
#define RF24_EDC			(0x0E02)	// Addr 0x020E
#define RFn_EDC_EDAUTO		(0x0)
#define RFn_EDC_EDSINGLE	(0x1)
#define RFn_EDC_EDCONT		(0x2)
#define RFn_EDC_EDOFF		(0x3)

#define RF09_EDD			(0x0F01)	// Addr 0x010F
#define RF24_EDD			(0x0F02)	// Addr 0x020F
	/* RFn_EDD_DF averaging time  T = DF*DTB (<<3) */
#define RFn_EDD_DTB2u		(0x0)
#define RFn_EDD_DTB8u		(0x1)
#define RFn_EDD_DTB32u		(0x2)
#define RFn_EDD_DTB128u		(0x3)

#define RF09_EDV			(0x1001)	// Addr 0x0110
#define RF24_EDV			(0x1002)	// Addr 0x0210

#define RF09_RNDV			(0x1101)	// Addr 0x0111
#define RF24_RNDV			(0x1102)	// Addr 0x0211

#define RF09_TXCUTC			(0x1201)	// Addr 0x0112
#define RF24_TXCUTC			(0x1202)	// Addr 0x0212
#define RFn_TXCUTC_PARAMP4	(0x0<<6)
#define RFn_TXCUTC_PARAMP8	(0x1<<6)
#define RFn_TXCUTC_PARAMP16	(0x2<<6)
#define RFn_TXCUTC_PARAMP32	(0x3<<6)
#define RFn_TXCUTC_FLC_80K  (0x0)
#define RFn_TXCUTC_FLC_100K (0x1)
#define RFn_TXCUTC_FLC_125K (0x2)
#define RFn_TXCUTC_FLC_160K (0x3)
#define RFn_TXCUTC_FLC_200K (0x4)
#define RFn_TXCUTC_FLC_250K (0x5)
#define RFn_TXCUTC_FLC_315K (0x6)
#define RFn_TXCUTC_FLC_400K (0x7)
#define RFn_TXCUTC_FLC_500K (0x8)
#define RFn_TXCUTC_FLC_625K (0x9)
#define RFn_TXCUTC_FLC_800K (0xA)
#define RFn_TXCUTC_FLC_1M	(0xB)

#define RF09_TXDFE			(0x1301)	// Addr 0x0113
#define RF24_TXDFE			(0x1302)	// Addr 0x0213
#define RFn_TXDFE_RCUT_0_25	(0x0<<5)
#define RFn_TXDFE_RCUT_0_37	(0x1<<5)
#define RFn_TXDFE_RCUT_0_5	(0x2<<5)
#define RFn_TXDFE_RCUT_0_75	(0x3<<5)
#define RFn_TXDFE_RCUT_1	(0x4<<5)
#define RFn_TXDFE_DM		(0x1<<4)
#define RFn_TXDFE_SR_4M		(0x1)
#define RFn_TXDFE_SR_2M		(0x2)
#define RFn_TXDFE_SR_1M333	(0x3)
#define RFn_TXDFE_SR_1M		(0x4)
#define RFn_TXDFE_SR_800K	(0x5)
#define RFn_TXDFE_SR_666K66	(0x6)
#define RFn_TXDFE_SR_500K	(0x8)
#define RFn_TXDFE_SR_400K	(0xA)

#define RF09_PAC			(0x1401)	// Addr 0x0114
#define RF24_PAC			(0x1402)	// Addr 0x0214
#define RFn_PAC_PACUR_n22	(0x0<<5)
#define RFn_PAC_PACUR_n18	(0x1<<5)
#define RFn_PAC_PACUR_n11	(0x2<<5)
#define RFn_PAC_PACUR_MAX	(0x3<<5)
#define RFn_PAC_TXPWR_MIN	(0x00)
	/* Between 0x00 and 0x1F, increments by 1dB */
#define RFn_PAC_TXPWR_MAX	(0x1F)

#define RF09_PADFE			(0x1601)	// Addr 0x0116
#define RF24_PADFE			(0x1602)	// Addr 0x0216
#define RFn_PADFE_MODE0		(0x0<<6)
#define RFn_PADFE_MODE1		(0x1<<6)
#define RFn_PADFE_MODE2		(0x2<<6)
#define RFn_PADFE_MODE3		(0x3<<6)

#define RF09_PLL			(0x2101)	// Addr 0x0121
#define RF24_PLL			(0x2102)	// Addr 0x0221
#define RFn_PLL_LBW_DEFAULT	(0x0<<4)
#define RFn_PLL_LBW_n15		(0x1<<4)
#define RFn_PLL_LBW_p15		(0x2<<4)
#define RFn_PLL_LS			(0x1<<1)

#define RF09_PLLCF			(0x2201)	// Addr 0x0122
#define RF24_PLLCF			(0x2202)	// Addr 0x0222
	/* PLL center frequency */

#define RF09_TXCI			(0x2501)	// Addr 0x0125
#define RF24_TXCI			(0x2502)	// Addr 0x0225
#define RF09_TXCQ			(0x2601)	// Addr 0x0126
#define RF24_TXCQ			(0x2602)	// Addr 0x0226
	/* TX path calibration value for I/Q*/

#define RF09_TXDACI			(0x2701)	// Addr 0x0127
#define RF24_TXDACI			(0x2702)	// Addr 0x0227
#define RF09_TXDACQ			(0x2801)	// Addr 0x0128
#define RF24_TXDACQ			(0x2802)	// Addr 0x0228
#define RFn_TXDACx_ENTXDACx	(0x1<<7)
	/* Write to TX DAC directly*/

#endif /* RADIO_AT86RF215_H_ */
