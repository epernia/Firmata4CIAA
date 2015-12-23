/* Copyright 2015, Ian Olivieri <ianolivieri93@gmail.com>
 * All rights reserved.
 *
 * This file is part of the Firmata4CIAA program.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Short description of this file
 **
 ** Long description of this file
 **
 **/

/** \addtogroup Firmata4CIAA Firmata4CIAA
 ** @{ */
/** \addtogroup Firmata4CIAA
 ** @{ */


/*==================[inclusions]=============================================*/
#include "main.h"
#include "UART_DRIVER.h"

/*==================[macros and definitions]=================================*/
#define	SYSTEM_BAUD_RATE 57600
#define CIAA_BOARD_UART LPC_USART2
/*==================[internal data declaration]==============================*/
static xQueueHandle xRxQueue,xTxQueue;
/*==================[internal functions declaration]=========================*/
void UART2_INIT(void);
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void UART2_INIT(void)
{
//Enable UART Rx Interrupt
	Chip_UART_IntEnable(CIAA_BOARD_UART,UART_IER_RBRINT );	//Receiver Buffer Register Interrupt
	// Enable UART line status interrupt
	//Chip_UART_IntEnable(CIAA_BOARD_UART,UART_IER_RLSINT );	//LPC43xx User manual page 1118
	NVIC_SetPriority(USART2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
	// Enable Interrupt for UART channel
	NVIC_EnableIRQ(USART2_IRQn);
}

//RX Interrupt needed to detect NULL (0x00) in the reception buffer (can't be done by pooling)
void UART2_IRQHandler(void)
{
	//INFO: Using the FreeRTOS Real Time Kernel_R Barry, page 108
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	uint8_t receivedByte;

	if (Chip_UART_ReadLineStatus(CIAA_BOARD_UART) & UART_LSR_RDR) {
		receivedByte = Chip_UART_ReadByte(CIAA_BOARD_UART);
	}
	xQueueSendToBackFromISR( xRxQueue, &receivedByte , &xHigherPriorityTaskWoken );
}

/*==================[external functions definition]==========================*/

/*----------------------Funciones productoras--------------------------------*/
/* Sends a byte through the UART peripheral (CIAA_BOARD_UART)*/
void UART_DRIVER_SendByte(uint8_t ch)
{
	xQueueSendToBack( xTxQueue, &ch, 10 / portTICK_RATE_MS );
}

void UART_DRIVER_SendString( uint8_t *sPtr,uint32_t strLength )
{
	uint32_t i=0;
	for(i=0; i<strLength;i++)
	{
		UART_DRIVER_SendByte(sPtr[i]);
	}
}
/*----------------------Funciones consumidoras--------------------------------*/
uint8_t UART_DRIVER_ReceiveByte(void)
{
	uint8_t output;
	xQueueReceive( xRxQueue, &output, portMAX_DELAY );

	return (output);
}

/*-------------------------Función de prueba----------------------------------*/
/* Esta tarea agarra los datos del buffer de transmición y los pone en el de recepción */
void UART_DRIVER_echoTask( void *pvParameters )
{
portTickType xLastExecutionTime;
uint8_t data;

	xLastExecutionTime = xTaskGetTickCount();

	while (1)
	{
		data = UART_DRIVER_ReceiveByte();
		UART_DRIVER_SendByte(data);
		vTaskDelayUntil( &xLastExecutionTime, 20 / portTICK_RATE_MS ); // periódica cada 20 ms
	}
}


/*..........-------------Hardware dependent functions---------------.........*/
/*--------------------------Manejo de la UART--------------------------------*/
/* Esta tarea es la encargada de hacer el update de lo que hay que enviar por la UART  */
void UART_DRIVER_TxGatekeeperTask( void *pvParameters )
{
uint8_t data;
	while (1)
	{
		// Wait for a message to arrive
		xQueueReceive( xTxQueue, &data, portMAX_DELAY );

		while ((Chip_UART_ReadLineStatus(CIAA_BOARD_UART) & UART_LSR_THRE) == 0) {}	// Wait for space in FIFO
		Chip_UART_SendByte(CIAA_BOARD_UART, data);

		//Sends the byte through the UART peripheral (CIAA_BOARD_UART)
		//Chip_UART_SendBlocking(CIAA_BOARD_UART, &data, 1);
	}
}

/* Esta tarea es la encargada de hacer el update de lo que se recibe de la UART. NO puede recibir NULL  */
void UART_DRIVER_RxGatekeeperTask( void *pvParameters )
{
uint8_t receivedByte;
	while (1)
	{

		if (Chip_UART_ReadLineStatus(CIAA_BOARD_UART) & UART_LSR_RDR) {
			receivedByte = Chip_UART_ReadByte(CIAA_BOARD_UART);

			//Chip_UART_Read(CIAA_BOARD_UART,&receivedByte,1);
			if (receivedByte)
			{
				xQueueSendToBack( xRxQueue, &receivedByte , 10 / portTICK_RATE_MS ); //10 ticks es lo que tarda en hacer el pooling como máximo
			}
		}
	}
}

/*-----------------------------------------------------------*/

void UART_DRIVER_Init(void)
{
	xRxQueue = xQueueCreate( 5, sizeof( uint8_t ) );
	xTxQueue = xQueueCreate( 5, sizeof( uint8_t ) ); //no le puedo dar mucho tamaño pq sino no llega a mandar lo que está en el buffer el gatekeeper, por la velocidad de transmisión. 57600 bits/s = 7.2 bytes/ms puede enviar como máximo antes que el scheduler le quite la CPU
	/*
	TODO optimizar el tamaño de la cola de tx para que envíe lo máximo posible en un ciclo de clock a partir del SYSTEM_BAUD_RATE y el portTICK_RATE_MS (1ms actual)
	Esto es teniendo en cuenta que el baudrate son los bits por seg que envía -> tengo un máximo que puedo mandar por tick (1ms actual)
	*/

	/* UART2 (USB-UART)/Debugger */
	//Initialize peripheral
	Chip_UART_Init(CIAA_BOARD_UART);
	// Set Baud rate
	Chip_UART_SetBaud(CIAA_BOARD_UART, SYSTEM_BAUD_RATE);

	//Modify FCR (FIFO Control Register)
	Chip_UART_SetupFIFOS(CIAA_BOARD_UART, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0);

	// Enable UART Transmission
	Chip_UART_TXEnable(CIAA_BOARD_UART);

	Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              /* P7_1: UART2_TXD */
	Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2: UART2_RXD */

	UART2_INIT();
}

/*==================[end of file]============================================*/
