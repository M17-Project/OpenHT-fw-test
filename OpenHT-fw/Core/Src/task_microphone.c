/*
 * TaskMicrophone.c
 *
 *  Created on: May 22, 2023
 *      Author: morga
 */

#include <main.h>
#include <cmsis_os2.h>
#include <pdm2pcm.h>
#include "task_microphone.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>

/* Lengths for one execution of the filter process*/
#define DECIMATION 	(64)						// 64 pdm samples (bits) -> 1 PCM sample (16 bits)
#define PCM_SAMPLES (16)						/* 16 */
#define PCM_BYTES	(PCM_SAMPLES*2)				/* 32 */
#define PDM_SAMPLES (PCM_SAMPLES*DECIMATION/16) /* 64 */
#define PDM_BYTES	(PDM_SAMPLES*2) 			/* 128 */


#define ACQ_DONE_FLAG (1<<0)

extern osEventFlagsId_t microphoneEventsHandle;
I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_rx;

/*
 * at the output we need 16 samples of 2 bytes each
 *
 * at the input we need 16*64 samples of 1 bit each
 */
uint8_t pdm_buffer[2*PDM_BYTES] = {0}; // Each byte is 8 samples so we divide decimation by 8
uint8_t pcm_buffer[PCM_BYTES] = {0};
uint8_t *pdm_reading_ptr;
volatile bool save_file = false;
volatile bool restart_dma = false;

__attribute__ ( (section(".sdram"))) uint8_t sound_buffer[2*1024*1024];
__attribute__ ( (section(".sdram"))) uint8_t raw_pdm_storage[4*1024*1024];

FRESULT save_wav_file(const char *filename, const uint8_t *data, const uint32_t number_bytes);

/**
* @brief Function implementing the microphones thread.
* @param argument: Not used
* @retval None
*/
void StartMicrophonesTask(void *argument)
{

	//Init the PDM2PCM library
	//MX_PDM2PCM_Init(); // Initialized in main
	PDM_Filter_Handler_t pdm_handle = {
			.bit_order = PDM_FILTER_BIT_ORDER_LSB,
			.endianness = PDM_FILTER_ENDIANNESS_BE,
			.high_pass_tap = 2143188679, //0.998*(2^31-1)
			.in_ptr_channels = 1,
			.out_ptr_channels = 1
	};

	PDM_Filter_Config_t pdm_config = {
			.decimation_factor = PDM_FILTER_DEC_FACTOR_64,
			.mic_gain = 24,
			.output_samples_number = PCM_SAMPLES
	};

	PDM_Filter_Init(&pdm_handle);
	PDM_Filter_setConfig(&pdm_handle, &pdm_config);

	// DMA
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

	//Init I2S3
	hi2s3.Instance = SPI3;
	hi2s3.Init.Mode = I2S_MODE_MASTER_RX;
	hi2s3.Init.Standard = I2S_STANDARD_LSB;
	hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
	hi2s3.Init.AudioFreq = 32000; // I2S clk -> 2.048M
	hi2s3.Init.CPOL = I2S_CPOL_LOW;
	hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
	hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
	if (HAL_I2S_Init(&hi2s3) != HAL_OK)
	{
		Error_Handler();
	}

	uint32_t sound_buffer_offset = 0;
	uint32_t raw_buffer_offset = 0;
	memset(sound_buffer, 0, sizeof(sound_buffer));

	/* Infinite loop */
	for(;;)
	{
		osEventFlagsWait(microphoneEventsHandle, ACQ_DONE_FLAG, 0, osWaitForever);

		// if save_file is true, that means that PTT was released and we have to save the file.
		if(save_file){
			save_wav_file("rec.wav", sound_buffer, sound_buffer_offset);
			sound_buffer_offset = 0;

			FIL fp;
			UINT bw;
			f_open(&fp, "/raw.bin", FA_CREATE_ALWAYS | FA_WRITE);
			f_write(&fp, raw_pdm_storage, raw_buffer_offset, &bw);
			f_close(&fp);
			save_file = 0;
		}else{
			PDM_Filter(pdm_reading_ptr, pcm_buffer, &pdm_handle);
			memcpy(sound_buffer + sound_buffer_offset, pcm_buffer, PCM_BYTES);
			sound_buffer_offset += PCM_BYTES;

			if(raw_buffer_offset < sizeof(raw_pdm_storage)){
				memcpy(raw_pdm_storage + raw_buffer_offset, pdm_buffer, PDM_BYTES);
				raw_buffer_offset += PDM_BYTES;
			}

		}
	}
}


void start_microphone_acquisition() {
	// Start DMA acquisition
	HAL_I2S_Receive_DMA(&hi2s3, (uint16_t *)pdm_buffer, PDM_SAMPLES*2);
}

void stop_microphone_acquisition() {
	save_file = true;
	HAL_I2S_DMAStop(&hi2s3);
	osEventFlagsSet(microphoneEventsHandle, ACQ_DONE_FLAG);
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hdma){
	pdm_reading_ptr = pdm_buffer;
	osEventFlagsSet(microphoneEventsHandle, ACQ_DONE_FLAG);
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hdma){
	osEventFlagsSet(microphoneEventsHandle, ACQ_DONE_FLAG);
}

/**
* @brief I2S MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hi2s->Instance==SPI3)
  {

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 5;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SPI3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**I2S3 GPIO Configuration
    PB3     ------> I2S3_CK
    PA15     ------> I2S3_WS
    PD6     ------> I2S3_SD
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_I2S3ext;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* I2S3 DMA Init */
    /* SPI3_RX Init */
    hdma_spi3_rx.Instance = DMA1_Stream0;
    hdma_spi3_rx.Init.Channel = DMA_CHANNEL_0;
    hdma_spi3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi3_rx.Init.Mode = DMA_CIRCULAR;
    hdma_spi3_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    /*hdma_spi3_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_spi3_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_spi3_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_spi3_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;*/
    if (HAL_DMA_Init(&hdma_spi3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hi2s,hdmarx,hdma_spi3_rx);
  }
}

/**
* @brief I2S MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspDeInit(I2S_HandleTypeDef* hi2s)
{
  if(hi2s->Instance==SPI3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();

    /**I2S3 GPIO Configuration
    PB3     ------> I2S3_CK
    PD6     ------> I2S3_SD
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6);

    /* I2S3 DMA DeInit */
    HAL_DMA_DeInit(hi2s->hdmarx);
  }

}

FRESULT save_wav_file(const char *filename, const uint8_t *data, const uint32_t number_bytes){
	// Save the file
	FIL fp;
	FRESULT res = f_open(&fp, filename, FA_CREATE_ALWAYS | FA_WRITE);

	uint32_t RIFF_cksize = 36 + number_bytes;
	uint8_t riff_chunk[12] = {
			'R', 'I', 'F', 'F',
			(uint8_t)(RIFF_cksize & 0xFF),				/* 				*/
				(uint8_t)((RIFF_cksize >> 8) & 0xFF),	/* RIFF 		*/
				(uint8_t)((RIFF_cksize >> 16) & 0xFF),  /* cksize		*/
				(uint8_t)(RIFF_cksize >> 24),			/* 				*/
			'W', 'A', 'V', 'E'};
	/*                        fmt              , cksize 16 , audio format 1, 1 ch, sample rate of 16kHz  , byte rate of 32k      , block align of 4*/
	uint8_t fmt_chunk[24] = {
			'f', 'm', 't', ' ', /* "fmt " chunk */
			16, 0, 0, 0,		/* chunk size of 16 */
			1, 0,				/* audio format is 1 for PCM*/
			1, 0,				/* 1 channel */
			0x80, 0x3E, 0, 0, 	/* sample rate 16000 Hz (0x3E80) */
			0x00, 0x7D, 0, 0, 	/* byterate (sample_rate * n_ch * bytes_per_sample) (0x7D00) */
			2, 0,				/* block align: n_ch * bytes_per_sample */
			16, 0 };			/* bits per sample */

	uint8_t data_chunk[8] = {
			'd', 'a', 't', 'a',
			(uint8_t)(number_bytes & 0xFF),
				(uint8_t)((number_bytes >> 8) & 0xFF),
				(uint8_t)((number_bytes >> 16) & 0xFF),
				(uint8_t)(number_bytes >> 24)};

	UINT bytes_written;

	res = f_write(&fp, riff_chunk, sizeof(riff_chunk), &bytes_written);
	if(bytes_written != sizeof(riff_chunk)){
		printf("[WAV] Written only %u bytes on %d for riff chunk.\n", bytes_written, sizeof(riff_chunk));
	}

	res = f_write(&fp, fmt_chunk, sizeof(fmt_chunk), &bytes_written);
	if(bytes_written != sizeof(fmt_chunk)){
		printf("[WAV] Written only %u bytes on %d for fmt chunk.\n", bytes_written, sizeof(fmt_chunk));
	}
	res = f_write(&fp, data_chunk, sizeof(data_chunk), &bytes_written);
	if(bytes_written != sizeof(data_chunk)){
		printf("[WAV] Written only %u bytes on %d for data chunk.\n", bytes_written, sizeof(data_chunk));
	}

	res = f_write(&fp, data, number_bytes, &bytes_written);
	if(bytes_written != number_bytes){
		printf("[WAV] Written only %u bytes on %lu for sound data.\n", bytes_written, number_bytes);
	}

	res = f_close(&fp);

	return res;
}
