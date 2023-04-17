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
