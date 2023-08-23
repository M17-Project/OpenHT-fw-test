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

#ifndef AUDIO_CS43L22_H_
#define AUDIO_CS43L22_H_

#define VP_VOLTAGE_FORMULA(VPLVL, VA)	((float)VPLVL*(float)VA/63.3)
/* Register definitions */
#define CHIP_ID					(0x01)

#define PWR_CTRL_1				(0x02)
#define POWER_DOWN				(0x01)
#define POWER_UP				(0x9E)

#define PWR_CTRL_2				(0x04)
#define	PDN_HPB_INV				(0x0<<6)
#define PDN_HPB_NORM			(0x1<<6)
#define PDN_HPB_ON				(0x2<<6)
#define PDN_HPB_OFF				(0x3<<6)
#define	PDN_HPA_INV				(0x0<<4)
#define PDN_HPA_NORM			(0x1<<4)
#define PDN_HPA_ON				(0x2<<4)
#define PDN_HPA_OFF				(0x3<<4)
#define	PDN_SPKB_INV			(0x0<<2)
#define PDN_SPKB_NORM			(0x1<<2)
#define PDN_SPKB_ON				(0x2<<2)
#define PDN_SPKB_OFF			(0x3<<2)
#define	PDN_SPKA_INV			(0x0<<0)
#define PDN_SPKA_NORM			(0x1<<0)
#define PDN_SPKA_ON				(0x2<<0)
#define PDN_SPKA_OFF			(0x3<<0)

#define CLK_CTRL				(0x05)
#define CLK_AUTO				(0x1<<7)
#define CLK_SPEED_DS			(0x0<<5)
#define CLK_SPEED_SS			(0x1<<5)
#define CLK_SPEED_HS			(0x2<<5)
#define CLK_SPEED_QS			(0x3<<5)
#define CLK_32K_GROUP			(0x1<<4)
#define CLK_VIDEO_CLK			(0x1<<3)
#define CLK_M_LR_RATIO_128		(0x0<<1)
#define CLK_M_LR_RATIO_125		(0x1<<1)
#define CLK_M_LR_RATIO_132		(0x2<<1)
#define CLK_M_LR_RATIO_136		(0x3<<1)
#define CLK_MCLK_DIV2			(0x1<<0)

#define IFC_CTRL_1				(0x06)
#define IFC_SLAVE				(0x0<<7)
#define IFC_MASTER				(0x1<<7)
#define IFC_SCLK_POL_INV		(0x1<<6)
#define IFC_DSP_MODE			(0x1<<4)
#define IFC_DAC_FMT_LEFT		(0x0<<2)
#define IFC_DAC_FMT_I2S			(0x1<<2)
#define IFC_DAC_FMT_RIGHT		(0x2<<2)
#define IFC_AUDIO_WORD_32_24	(0x0<<0)
#define IFC_AUDIO_WORD_24_20	(0x1<<0)
#define IFC_AUDIO_WORD_20_18	(0x2<<0)
#define IFC_AUDIO_WORD_16_16	(0x3<<0)

#define IFC_CTRL_2				(0x07)
#define IFC_SCLK_EQ_MCLK		(0x1<<6)
#define IFC_INV_SPK_HP_SW		(0x1<<3)

// Passthrough registers have been ignored

#define PLAYBACK_CTRL_1			(0x0D)
#define HP_ANALOG_GAIN_0_3959	(0x0<<5)
#define HP_ANALOG_GAIN_0_4571	(0x1<<5)
#define HP_ANALOG_GAIN_0_5111	(0x2<<5)
#define HP_ANALOG_GAIN_0_6047	(0x3<<5)
#define HP_ANALOG_GAIN_0_7099	(0x4<<5)
#define HP_ANALOG_GAIN_0_8399	(0x5<<5)
#define HP_ANALOG_GAIN_1_0000	(0x6<<5)
#define HP_ANALOG_GAIN_1_1430	(0x7<<5)
#define PLAYBACK_VOL_A_EQ_B		(0x1<<4)
#define INVERT_PCMB				(0x1<<3)
#define INVERT_PCMA				(0x1<<2)
#define MSTRB_PLAYBACK_MUTE		(0x1<<1)
#define MSTRA_PLAYBACK_MUTE		(0x1<<0)

#define MISC_CTRL				(0x0E)
#define PASSTHROUGH_ANALOG_B	(0x1<<7)
#define PASSTHROUGH_ANALOG_A	(0x1<<6)
#define PASSTHROUGH_MUTE_B		(0x1<<5)
#define PASSTHROUGH_MUTE_A		(0x1<<4)
#define FREEZE_REGS				(0x1<<3)
#define DE_EMPHASIS				(0x1<<2)
#define	SOFT_RAMP_ENABLE		(0x1<<1)
#define ZERO_CROSS_ENABLE		(0x1<<0)

#define PLAYBACK_CTRL_2			(0x0F)
#define HPB_MUTE				(0x1<<7)
#define HPA_MUTE				(0x1<<6)
#define SPKB_MUTE				(0x1<<5)
#define SPKA_MUTE				(0x1<<4)
#define SPK_VOL_B_EQ_A			(0x1<<3)
#define SPK_CH_SWP				(0x1<<2)
#define	SPK_MONO				(0x1<<1)
#define SPK_MUTE_50_50			(0x1<<0)

#define PCMA_VOLUME				(0x1A)
#define PCMB_VOLUME				(0x1B)
#define PCMx_MUTE				(0x1<<7)
#define PCMx_VOL_MASK			(0x7F)
#define PCMx_VOL_MAX			(0x18) // +12dB
#define PCMx_VOL_ZERO			(0x00) // Steps of 0.5dB
#define PCMx_VOL_MIN			(0x19) // -51.5dB

// Because of the ugly sound it generates, beep generator registers are ignored

#define BEEP_TONE_CFG			(0x1E)
#define TREBLE_CORNER_5K		(0x0<<3)
#define TREBLE_CORNER_7K		(0x1<<3)
#define TREBLE_CORNER_10K		(0x2<<3)
#define TREBLE_CORNER_15K		(0x3<<3)
#define BASS_CORNER_50			(0x0<<1)
#define BASS_CORNER_100			(0x1<<1)
#define BASS_CORNER_200			(0x2<<1)
#define BASS_CORNER_250			(0x3<<1)
#define TONE_CTRL_EN			(0x1<<0)

#define TONE_CTRL				(0x1F)
#define TREBLE_MAX				(0x0<<4)
#define TREBLE_ZERO				(0x8<<4)
#define TREBLE_MIN				(0xF<<4)
#define BASS_MAX				(0x0<<0)
#define BASS_ZERO				(0x8<<0)
#define BASS_MIN				(0xF<<0)

#define MSTRA_VOLUME			(0x20)
#define	MSTRB_VOLUME			(0x21)
#define MSTRx_VOLUME_MAX		(0x18) // 24 in decimal, +12dB
#define MSTRx_VOLUME_MIN		(0x34) // -204 in decimal, -102dB

#define HPA_VOLUME				(0x22)
#define HPB_VOLUME				(0x23)
#define HPx_VOLUME_MAX			(0x00) // 0dB
#define HPx_VOLUME_MIN			(0x40) // -96dB
#define HPx_VOLUME_MUTED		(0x01)

#define SPKA_VOLUME				(0x24)
#define SPKB_VOLUME				(0x25)
#define SPKx_VOLUME_MAX			(0x00) // 0dB
#define SPKx_VOLUME_MIN			(0x40) // -96dB
#define SPKx_VOLUME_MUTED		(0x01)

#define PCM_CHAN_SWP			(0x26)
#define PCMA_SWP_LEFT			(0x0<<6)
#define PCMA_SWP_SUM			(0x1<<6)
#define PCMA_SWP_RIGHT			(0x3<<6)
#define PCMB_SWP_LEFT			(0x0<<4)
#define PCMB_SWP_SUM			(0x1<<4)
#define PCMB_SWP_RIGHT			(0x3<<4)

#define LIMIT_CTRL_1			(0x27)
#define LIMIT_MAX_0				(0x0<<5)
#define LIMIT_MAX_n3			(0x1<<5)
#define LIMIT_MAX_n6			(0x2<<5)
#define LIMIT_MAX_n9			(0x3<<5)
#define LIMIT_MAX_n12			(0x4<<5)
#define LIMIT_MAX_n18			(0x5<<5)
#define LIMIT_MAX_n24			(0x6<<5)
#define LIMIT_MAX_n30			(0x7<<5)
#define LIMIT_CSHN_0			(0x0<<2)
#define LIMIT_CSHN_n3			(0x1<<2)
#define LIMIT_CSHN_n6			(0x2<<2)
#define LIMIT_CSHN_n9			(0x3<<2)
#define LIMIT_CSHN_n12			(0x4<<2)
#define LIMIT_CSHN_n18			(0x5<<2)
#define LIMIT_CSHN_n24			(0x6<<2)
#define LIMIT_CSHN_n30			(0x7<<2)
#define LIMIT_SOFT_RAMP_ON		(0x1<<1)
#define LIMIT_ZERO_CROSS_ON		(0x1<<0)

#define LIMIT_CTRL_2			(0x28)
#define PEAK_DET_LIM_EN			(0x1<<7)
#define	PEAK_LIm_ALL			(0x1<<6)
#define LIMIT_REL_RATE_FASTEST	(0x0)
#define LIMIT_REL_RATE_SLOWEST	(0x3F)

#define LIMIT_ATK_RATE			(0x29)
#define LIMIT_ATK_RATE_FASTEST	(0x00)
#define LIMIT_ATK_RATE_SLOWEST	(0x3F)

#define STATUS					(0x2E)

#define BATT_COMP				(0x2F)
#define BATT_COMP_EN			(0x1<<7)
#define VP_MONITOR_EN			(0x1<<6)
#define VP_REF_MASK				(0xF)

#define VP_BATT_LEVEL			(0x30)

#define SPK_STATUS				(0x31)

#define CHG_PUMP_FREQ			(0x34)
#define CHG_PUMP_N0				(0x0<<4)
#define CHG_PUMP_N1				(0x1<<4)
#define CHG_PUMP_N2				(0x2<<4)
#define CHG_PUMP_N3				(0x3<<4)
#define CHG_PUMP_N4				(0x4<<4)
#define CHG_PUMP_N5				(0x5<<4)
#define CHG_PUMP_N6				(0x6<<4)
#define CHG_PUMP_N7				(0x7<<4)
#define CHG_PUMP_N8				(0x8<<4)
#define CHG_PUMP_N9				(0x9<<4)
#define CHG_PUMP_N10			(0xA<<4)
#define CHG_PUMP_N11			(0xB<<4)
#define CHG_PUMP_N12			(0xC<<4)
#define CHG_PUMP_N13			(0xD<<4)
#define CHG_PUMP_N14			(0xE<<4)
#define CHG_PUMP_N15			(0xF<<4)

#endif /* AUDIO_CS43L22_H_ */
