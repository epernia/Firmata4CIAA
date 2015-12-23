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

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and definitions]=================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*----------------------Funciones productoras--------------------------------*/
/*Esta función le permite enviar un byte a la cola de transmision */
void UART_DRIVER_SendByte( uint8_t );
/*Esta función le permite enviar un string a la cola de transmision */
void UART_DRIVER_SendString( uint8_t*,uint32_t ); //second_parameter = sizeof(first_paremeter)

/*----------------------Funciones consumidoras--------------------------------*/
/*Funcion para recibir los datos de la cola de recepcion  */
uint8_t UART_DRIVER_ReceiveByte( void );

/*-------------------------Función de prueba----------------------------------*/
/* Esta tarea agarra los datos del buffer de transmición y los pone en el de recepción */
void UART_DRIVER_echoTask( void* );

/*--------------------------Manejo de la UART--------------------------------*/
/* Esta tarea es la encargada de hacer el update de lo que hay que enviar por la UART  */
void UART_DRIVER_TxGatekeeperTask( void* );
/* Esta tarea es la encargada de hacer el update de lo que se recibe de la UART  */
void UART_DRIVER_RxGatekeeperTask( void* );
/*    Inicialización de la UART de la baseboard, junto con las colas de transmisión y recepción  */
void UART_DRIVER_Init();
/*---------------------------------UART--------------------------------------*/
/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/

#endif /* UART_DRIVER_H_ */
