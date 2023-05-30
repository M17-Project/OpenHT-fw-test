## HAL-based test firmware/GUI for the OpenHT

License: GPLv2

### Status
This user interface does not currently RX or TX with the hardware. In fact, the FPGA HW is currently being worked here: https://github.com/M17-Project/OpenHT-hw. The goal is to show a Proof of Concept (PoC) of RX and TX with different modes, including FM, M17, FreeDV, etc. As with any open source project, help is wanted. So if you are excited about the possibility to *OWN* your own radio and be able to use/modify/extend without asking for permission. Come join us!

##### OpenHT-fw contains the STM32CubeIDE (Eclipse based) project
Note: Don't forget to initialize submodule (`OpenHT-fw/Drivers/lvgl`) to pull in lvgl source, currently using v8.3.6 tag.

##### OpenHT_UI contains the SquareLine Studio project
Note: When generating C files with SL Studio the destination must be in `OpenHT-fw/Core/Src/lvgl_ui`

#### Hardware (PoC)

1. MCU will target the STM32F469I-DISCO Discovery board
2. OpenHT FPGA board will interface with the existing DISCO Arduino headers

Together they will create the OpenHT PoC. (TX power will be extremely limited on PoC)

The OpenHT FPGA board contains the speaker, FPGA, RF, power distribution, PTT and Power switches. The MCU is the OpenHT control interface. It contains the touchscreen user interface, the microphone, flash memory and USB interface.

When the PTT button is pressed by the operator, the MCU (over SPI) will instruct the FPGA board to begin transmitting and will then provide the audio or baseband data samples to the FPGA board for transmission. Once transmission is complete the MCU will instruct the FPGA board to switch to receive mode. Any received signal will be sent by the FPGA board to the MCU for conversion into audio based on the radio mode setting.

## Gallery
#### Main Screen (About...)
<center><img src="https://github.com/M17-Project/OpenHT-fw-test/blob/main/docs/openht_main_screen.jpg" width="400"></center>

Callsign             |  Frequency
:-------------------------:|:-------------------------:
![](https://github.com/M17-Project/OpenHT-fw-test/blob/main/docs/openht_screen_callsign.jpg)  |  ![](https://github.com/M17-Project/OpenHT-fw-test/blob/main/docs/openht_screen_freq.jpg)

## FPGA SPI interface
### General setup
First, AT86RF215 needs to be initialized to accept (and output) I/Q baseband over LVDS:<br>
`AT86_ConfigCommon();`

After that's done, the chip needs to be tuned to the required frequency for either transmission or reception:<br>
```
AT86_TX09(433475000, 2.0f, 0x13);
AT86_TX24(2400000000, 1.1f, 0x17);
AT86_RX09(433475000+40000, 1.1f, 1, 0);
AT86_RX24(2400000000+40000, 1.1f, 1, 0);
```

AT86_xXnn - x -> R=reception, T=transmission; nn -> 09=sub-GHz or 24=2.4GHz<br>
The parameters are as follows:<br>
- frequency in Hz, for RX add frequency translation of 40kHz, `uint32_t`
- frequency correction in ppm (float)
- for TX: power setting, 0-23 (0=min, 23=max)
- for RX: AGC enable (0=disabled, 1=enabled) and AGC target level (0=max, 7=min)

### Register setups
It's assumed that two `uint16_t` variables are declared: `reg_addr` and `reg_val`. Additionally, one array of `uint8_t` is needed to hold the data for SPI transfer.

#### FM transceiver, sub-GHz, narrow
```
reg_addr=REG_WR|CR_1;
reg_val=MOD_FM|PD_ON|DEM_FM|BAND_09;
...
reg_addr=REG_WR|CR_2;
reg_val=CTCSS_TX_NONE|STATE_RX|FM_TX_N;
...
```
