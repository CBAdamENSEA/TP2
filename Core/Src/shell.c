/*
 * shell.c
 *
 *  Created on: Sep 23, 2022
 *      Author: cheik
 */
#include "shell.h"
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>



extern int state;
extern int shell_on;

char RX_buffer[50];
int reading=1;
char state_msg[3];
int led_freq=1;
int RX_buffer_size=0;

int reading_uart(char RX_buffer[50], int *reading)
{
	char RX_ch_buffer;
	int buffer_size=0;

	while(*reading)
	{

		HAL_UART_Receive(&huart1,&RX_ch_buffer, 1, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, &RX_ch_buffer, 1, HAL_MAX_DELAY);
		RX_buffer[buffer_size]=RX_ch_buffer;
		buffer_size++;
		if (RX_ch_buffer==13)
		{
			RX_buffer[buffer_size-1]='\0';
			HAL_UART_Transmit(&huart1, "\n\r", 2, HAL_MAX_DELAY);

			*reading=0;
		}
	}
	return buffer_size;
}
void shell(void)
{

	int i=0;
	while(shell_on)
	{
		HAL_UART_Transmit(&huart1, "STM32--->", 9, HAL_MAX_DELAY);
		RX_buffer_size=reading_uart(RX_buffer, &reading);
		//HAL_UART_Transmit(&huart1, RX_buffer, RX_buffer_size, HAL_MAX_DELAY);

		if (strcmp(RX_buffer,"LED_ON")==0)
		{
			state=LED_ON;
			if (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin)==GPIO_PIN_RESET)
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_SET);
			}
			HAL_UART_Transmit(&huart1, "LED turned ON\n\r", 15, HAL_MAX_DELAY);
		}
		else if (strcmp(RX_buffer,"LED_OFF")==0)
		{
			state=LED_OFF;
			if (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin)==GPIO_PIN_SET)
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_RESET);
			}
			HAL_UART_Transmit(&huart1, "LED turned OFF\n\r", 16, HAL_MAX_DELAY);
		}

		else if (strcmp(RX_buffer,"LED_TOGGLE")==0)
		{
			state=LED_TOGGLE;
			HAL_UART_Transmit(&huart1, "Choose a frequency between 1Hz and 5000Hz: ", 43, HAL_MAX_DELAY);
			reading=1;
			RX_buffer_size=reading_uart(RX_buffer, &reading);
			led_freq=atoi(RX_buffer);
			TIM2->PSC=(int)(108000000.0/(10800*led_freq)-1);
			HAL_UART_Transmit(&huart1, "LED toggling\n\r", 14, HAL_MAX_DELAY);



		}
		else if (strcmp(RX_buffer,"LED_FREQ")==0)
		{
			state=LED_TOGGLE;
		}
		//sprintf(state_msg,"%d\n\r",strcmp(RX_buffer,"LED_ON"));
		//HAL_UART_Transmit(&huart1, RX_buffer, buffer_size, HAL_MAX_DELAY);
		//HAL_UART_Transmit(&huart1, state_msg, 5, HAL_MAX_DELAY);
		reading=1;

	}
}
