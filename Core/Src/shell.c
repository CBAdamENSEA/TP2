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
char RX_ch_buffer;
char RX_buffer[50];
int reading=1;
char state_msg[3];

void shell(void)
{
	int buffer_size=0;
	int i=0;
	char reference_buffer[]="LED_ON";
	while(shell_on)
	{
		buffer_size=0;
		while(reading)
		{
			HAL_UART_Receive(&huart1,&RX_ch_buffer, 1, HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, &RX_ch_buffer, 1, HAL_MAX_DELAY);
			RX_buffer[buffer_size]=RX_ch_buffer;
			buffer_size++;
			if (RX_ch_buffer==13)
			{
				RX_buffer[buffer_size-1]='\0';
				HAL_UART_Transmit(&huart1, "\n\r", 2, HAL_MAX_DELAY);
				//HAL_UART_Transmit(&huart1, RX_buffer, buffer_size, HAL_MAX_DELAY);
				reading=0;
			}
		}

		if (strcmp(RX_buffer,"LED_ON")==0)
		{
			state=LED_ON;
			if (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin)==GPIO_PIN_RESET)
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_SET);
			}
		}
		else if (strcmp(RX_buffer,"LED_OFF")==0)
		{
			state=LED_OFF;
			if (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin)==GPIO_PIN_SET)
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_RESET);
			}
		}

		else if (strcmp(RX_buffer,"LED_TOGGLE")==0)
		{
			state=LED_TOGGLE;
		}
		sprintf(state_msg,"%d\n\r",strcmp(RX_buffer,"LED_ON"));
		//HAL_UART_Transmit(&huart1, RX_buffer, buffer_size, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart1, state_msg, 5, HAL_MAX_DELAY);
		reading=1;

	}
}
