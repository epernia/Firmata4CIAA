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
#include "FIRMATA.h"
#include "UART_DRIVER.h"
#include "HAL.h"

/*==================[macros and definitions]=================================*/


/*-------------------Firmata command messages--------------------*/
/* Version numbers for the protocol.  The protocol is still changing, so these
 * version numbers are important.  This number can be queried so that host
 * software can test whether it will be compatible with the currently
 * installed firmware. */
#define FIRMATA_MAJOR_VERSION   2 // for non-compatible changes
#define FIRMATA_MINOR_VERSION   3 // for backwards compatible changes

#define MAX_DATA_BYTES 32 // max number of data bytes in non-Sysex messages

// Message Types (128-255/0x80-0xFF)
//Data Messages
#define DIGITAL_MESSAGE         0x90 // send data for a digital pin
#define ANALOG_MESSAGE          0xE0 // send data for an analog pin (or PWM). Max pin=15
//Control Messages
#define REPORT_ANALOG           0xC0 // enable analog input by pin #
#define REPORT_DIGITAL          0xD0 // enable digital input by port pair
#define SET_PIN_MODE            0xF4 // set a pin to INPUT/OUTPUT/PWM/etc
//Protocol Version
#define REPORT_VERSION          0xF9 // report protocol version
//System Reset
#define SYSTEM_RESET            0xFF // reset from MIDI
//Sysex Messages
#define START_SYSEX             0xF0 // start a MIDI Sysex message
#define END_SYSEX               0xF7 // end a MIDI Sysex message

// Sysex Command Messages (0-127/0x00-0x7F)
// 0x00-0x0F reserved for user-defined commands
#define ANALOG_MAPPING_QUERY    0x69 // ask for mapping of analog to pin numbers
#define ANALOG_MAPPING_RESPONSE 0x6A // reply with mapping info
#define CAPABILITY_QUERY        0x6B // ask for supported modes and resolution of all pins
#define CAPABILITY_RESPONSE     0x6C // reply with supported modes and resolution
#define PIN_STATE_QUERY         0x6D // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE      0x6E // reply with pin's current mode and value
#define EXTENDED_ANALOG         0x6F // analog write (PWM, Servo, etc) to any pin
#define SERVO_CONFIG            0x70 // set max angle, minPulse, maxPulse, freq
#define STRING_DATA             0x71 // a string message with 14-bits per char
#define SHIFT_DATA              0x75 // a bitstream to/from a shift register
#define I2C_REQUEST             0x76 // send an I2C read/write request
#define I2C_REPLY               0x77 // a reply to an I2C read request
#define I2C_CONFIG              0x78 // config I2C settings such as delay times and power pins
#define REPORT_FIRMWARE         0x79 // report name and version of the firmware
#define SAMPLING_INTERVAL       0x7A // set the poll rate of the main loop
#define SYSEX_NON_REALTIME      0x7E // MIDI Reserved for non-realtime messages
#define SYSEX_REALTIME          0x7F // MIDI Reserved for realtime messages


/*==================[internal data declaration]==============================*/
uint8_t REPORT_FIRMWARE_MSG[] = {0x46,0x00,0x49,0x00,0x52,0x00,0x4D,0x00,0x41,0x00,0x54,0x00,0x41,0x00,0x2E,0x00,0x63,0x00}; //FIRMATA.c

//Guarda los valores impuestos por los comandos de control C0~CF and D0~DF para saber que datos hay que mandar y cuales no.
//FIXME: no est� andando bien; solo lee C1 y C7. momentaneamente pongo all en 1
OPTIMAL_DATA_TYPE analogInStatus[16] = {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};	//Which analog pins (inputs) will report their status
//FIXME: no llega ningun Dport, asi q ninguno se pone en 1, nunca.
OPTIMAL_DATA_TYPE digitalInStatus[16] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};	//Which digital ports (inputs) will report their status


static struct digitalData{
	uint8_t port, firstByte, secondByte;
}digitalDataReceived;

/*==================[internal functions declaration]=========================*/
/*---- Protocol related functions ----*/
void startSysex(void);
void endSysex(void);

void printFirmwareVersion(void);
void printVersion(void);
void printAnalogMappingResponse(void);	//0x6A
void printCapabilityResponse(void);		//0x6C
void printPinStateResponse(void);		//0x6E

/*---- Data handling functions ----*/
static void sendValueAsTwo7bitBytes(uint16_t);
static void sendAnalogInputs(void);
uint8_t dataMessagesTo8bitsByte(uint8_t , uint8_t);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
/*---- Protocol related functions ----*/
void startSysex(void)
{
	UART_DRIVER_SendByte(START_SYSEX);
}
void endSysex(void)
{
	UART_DRIVER_SendByte(END_SYSEX);
}

void printFirmwareVersion(void)
{
	startSysex();
	UART_DRIVER_SendByte(REPORT_FIRMWARE);
	UART_DRIVER_SendByte(FIRMATA_MAJOR_VERSION);
	UART_DRIVER_SendByte(FIRMATA_MINOR_VERSION);
	UART_DRIVER_SendString(REPORT_FIRMWARE_MSG, sizeof(REPORT_FIRMWARE_MSG));
	endSysex();
}

void printVersion(void)
{
	UART_DRIVER_SendByte(REPORT_VERSION);
	UART_DRIVER_SendByte(FIRMATA_MAJOR_VERSION);
	UART_DRIVER_SendByte(FIRMATA_MINOR_VERSION);
}

/*Analog Mapping Response (0x6A)
127 = 0x7F for digital pins (does not support analog); 00 to 0F for analog channel (16 max)*/
void printAnalogMappingResponse(void)
{
	OPTIMAL_DATA_TYPE cont;
	startSysex();
	UART_DRIVER_SendByte(ANALOG_MAPPING_RESPONSE);

	for(cont=0; cont<TOTAL_DIGITAL_PINS; cont++)
	{
		UART_DRIVER_SendByte(0x7F);
	}
	for(cont=0; cont<TOTAL_ANALOG_PINS; cont++)
	{
		UART_DRIVER_SendByte(cont & 0x0F); //Analog channels can only be 0-15, so chop higher bits
	}


	endSysex();
}

/*Capability Response (0x6C)
1st Byte - pin function: 00 = Input; 01 = Output; 02= Analog; 03= Pwm; 04=Servo; 06=I2C
2nd Byte - pin resolution
3rd Byte - 0x7F to mark the end of the pin's modes*/
void printCapabilityResponse(void)
{
	OPTIMAL_DATA_TYPE pinNumber;
	startSysex();
	UART_DRIVER_SendByte(CAPABILITY_RESPONSE);

	for(pinNumber=PIN0; pinNumber<TOTAL_PINS;pinNumber++)
	{
		//pines digitales
		if(pinNumber<TOTAL_DIGITAL_PINS)
		{
			UART_DRIVER_SendByte(INPUT);
			UART_DRIVER_SendByte(0x01);
			UART_DRIVER_SendByte(OUTPUT);
			UART_DRIVER_SendByte(0x01);
			if (HAL_CheckPinPWMFunc(pinNumber))
			{
				UART_DRIVER_SendByte(PWM);
				UART_DRIVER_SendByte(0x08);
				if (pinNumber == PIN12 || pinNumber==PIN18 || pinNumber==PIN19 || pinNumber==PIN20 || pinNumber==PIN21||pinNumber==PIN26 || pinNumber==PIN27 || pinNumber==PIN28 || pinNumber==PIN29)
				{
					//Los servos quedan limitados a estos 4 pines por ahora pq sino la el timer 1 handler se hace muy largo
					UART_DRIVER_SendByte(SERVO);
					UART_DRIVER_SendByte(0x08);
				}
			}
		}
		//pines digitales
		else
		{
			UART_DRIVER_SendByte(ANALOG);
			UART_DRIVER_SendByte(0x0A);	//resoluci�n 10 bit
		}
		UART_DRIVER_SendByte(0x7F);
	}
	endSysex();
}

/*Pin State Response (0x6E)
 1st Byte - pin n�
 2nd Byte - default pin mode
 3nd Byte - pin state*/
void printPinStateResponse(void)
{
	OPTIMAL_DATA_TYPE pinNumber=0;
	//pines digitales
	for(pinNumber=PIN0; pinNumber<TOTAL_PINS;pinNumber++)
	{
		startSysex();
		UART_DRIVER_SendByte(PIN_STATE_RESPONSE);
		UART_DRIVER_SendByte(pinNumber);
		//fixme: manda hasta 0x0F (16) y manda 64 strings, no 32
		//fixme: sacar despues el if trucho (este de abajo), o no, quien sabe.

		//pines digitales
		if(pinNumber<TOTAL_DIGITAL_PINS)
		{
			//if(pinNumber<DIGITAL_INPUTS) podr�a ser esto tmb, por si queres borrar la funcion CheckPin, q no es mala idea
			if(!HAL_CheckPin(pinNumber))
			{
				UART_DRIVER_SendByte(INPUT);
				UART_DRIVER_SendByte(ON);		//valor por defecto de los pulsadores (pull-up activado)
			}
			else
			{
				UART_DRIVER_SendByte(OUTPUT);
				UART_DRIVER_SendByte(OFF);		//valor por defecto de los leds
			}
		}
		//pines analogicos
		else
		{
			UART_DRIVER_SendByte(ANALOG);
			UART_DRIVER_SendByte(0);
		}


		endSysex();
	}

}

/*---- Data handling functions ----*/
/* Sends the 16 bit value to the UART by dividing it into 2 MIDI formated bytes (MSB = 0)*/
static void sendValueAsTwo7bitBytes(uint16_t valueToSend)
{
	UART_DRIVER_SendByte(valueToSend & 0b01111111);	// LSB
	UART_DRIVER_SendByte(valueToSend>>7 & 0b01111111); // MSB
}

/* Arranges the first and second data bytes, received after an analog (E0~EF) or digital (90~9F)
 *   I/O message, like a normal port byte. MIDI protocol uses them this way
 *   	 1st Byte (LSB)			|		 2nd Byte(MSB)				|		Port Byte
 *   0 x6 x5 x4 x3 x2 x1 x0		|	  0 y6 y5 y4 y3 y2 y1 y0		|	y0 x6 x5 x4 x3 x2 x1 x0
 *   */
uint8_t dataMessagesTo8bitsByte(uint8_t byte1, uint8_t byte2)
{
	uint8_t portValue;
	portValue = ((byte2 << 7) & 0b10000000) | (byte1 & 0b01111111);
	return (portValue);
}


//TODO: podes hacer que se junten ambas funciones en 1 sola.. pq son iguales. O separarlas bien
/* Sends all available analog values to UART with their corresponding identifier (E0 ~ EF: analog pin) */
static void sendAnalogInputs(void)
{
	OPTIMAL_DATA_TYPE analogChannel;
	int16_t value;
	for (analogChannel=0;analogChannel<=TOTAL_ANALOG_PINS;analogChannel++)
	{
		if(analogInStatus[analogChannel])	//Check if (C0 | pin) was sent. Si as� fue el campo del vector ser� 1 y quiere decir que dicho pin debe reportar su estado
		{
			value=HAL_GetAnalogPin(analogChannel+1);	//Get analog value. channel+1 because the first actual channel is ADC_CH1=1
			if(value!=-1)		//Check if value changed (-1 = no change) so it doesn't waste time re-sending useless information
			{
				UART_DRIVER_SendByte(ANALOG_MESSAGE | (analogChannel & 0x0F));	//highest posible pin: 15
				sendValueAsTwo7bitBytes((uint16_t)value);
			}
		}
	}
}


static void sendDigitalInputs(void)
{
	OPTIMAL_DATA_TYPE port;
	int16_t value;			//int16_t pq el puerto es de 8 bits y tiene que poder tener el -1, asi q int8_t no alcanza
	for (port=0;port<TOTAL_DIGITAL_PORTS;port++)
	{
		if(digitalInStatus[port])	//Check if (D0 | pin) was sent. Si as� fue el campo del vector ser� 1 y quiere decir que dicho pin debe reportar su estado
		{
			value = HAL_GetPort(port);	//Get digital port value.
			if(value!=-1)		//Check if value changed (-1 = no change) so it doesn't waste time re-sending useless information
			{
				UART_DRIVER_SendByte(DIGITAL_MESSAGE | (port & 0x0F));
				sendValueAsTwo7bitBytes((uint16_t)value);
			}
		}
	}
}




/*==================[external functions definition]==========================*/
/*Esta tarea se encarga de enviar los valores de las salidas digitales y anal�gicas
 * de la placa, bajo el marco del protocolo firmata, para que el cliente los tenga */
void FIRMATA_InputsUpdateTask( void *pvParameters )
{
	portTickType xLastExecutionTime;
	uint8_t receivedByte, digitalPort;

	xLastExecutionTime = xTaskGetTickCount();

	while(1)
	{
		sendDigitalInputs();
		sendAnalogInputs();
		vTaskDelayUntil( &xLastExecutionTime, 100 / portTICK_RATE_MS );
	}
}


/*Esta tarea se encarga de analizar lo que llega de la UART para poder interpretar y contestar
 * los mensajes del cliente*/
void FIRMATA_UpdateTask( void *pvParameters )
{
	portTickType xLastExecutionTime;
	//TODO: hacer que haya solo 2 variables para guardar bytes, e intent� que reemplace a la estructura q usas para el digital message
	uint8_t receivedByte, receivedByteTemp ,portBinValue;

	xLastExecutionTime = xTaskGetTickCount();
	while (1)
	{
		receivedByte = UART_DRIVER_ReceiveByte();	//get first received byte
		receivedByteTemp = receivedByte;
		//This filters the received data to handle correctly data and control messages
		//becouse they have ranged values (90~9F ; C0~CF ; D0~DF ; E0~EF)
		if(receivedByteTemp < 0xF0)
		{
			receivedByteTemp &= 0b11110000;
		}
		switch(receivedByteTemp)	//Handle Received Byte according to Message Types
		{
		//SySex Message handling
		case (START_SYSEX):
			receivedByte = UART_DRIVER_ReceiveByte();	//get the SySex command (second byte)
			switch (receivedByte)
			{
			case (REPORT_FIRMWARE):
				printFirmwareVersion();
				break;

			case (ANALOG_MAPPING_QUERY):
				printAnalogMappingResponse();
				break;

			case (CAPABILITY_QUERY):
				printCapabilityResponse();
				break;

			case (PIN_STATE_QUERY):
				printPinStateResponse();
				break;

			case(EXTENDED_ANALOG):
					/*This message has variable length. Its made by F0 6F pin message 7F
					 * "pin" can go from 0x10 (16) to 0xFF(255)
					 * "message" can go from 0x00 - 0x7F (only 1 byte), to 0x00 0x01 - 0x7F 0x01 (2 bytes)
					 */
				digitalDataReceived.port = UART_DRIVER_ReceiveByte();	//Pin number. From 0x10(16) to 0xFF(255)
				digitalDataReceived.firstByte = UART_DRIVER_ReceiveByte(); //the only byte used when the message is 1 byte long
				digitalDataReceived.secondByte = UART_DRIVER_ReceiveByte(); //stops being "END_SYSEX" when the first byte > 7F
				/*There is a problem in the sender. When firstByte <= 127, second byte is 0xF7 (END_SYSEX) and the message has goes from 0 to 127.
				 * But when firstByte = 127+1, the sender sends firstByte = 0 (because the MSB in MIDI is always 0),
				 * but the secondByte = 0xF7, which is the same as if the real received byte was 0.
				 * In other words, 0 and 128 are treated the same way because the SENDER does not distinguish them*/
				if(digitalDataReceived.secondByte==END_SYSEX)
				{
					digitalDataReceived.secondByte=0x00;
				}
				portBinValue = dataMessagesTo8bitsByte(digitalDataReceived.firstByte,digitalDataReceived.secondByte);
				HAL_SetPWM(digitalDataReceived.port,portBinValue);
				break;
			}

			break;

		//Protocol Version request handling
		case (REPORT_VERSION):
			printVersion();
			break;

		/*Data Message handling*/
		//FIXME: hacer bien esto
		case (DIGITAL_MESSAGE):
			digitalDataReceived.port = (receivedByte & 0b00001111);	//Port number
			digitalDataReceived.firstByte = UART_DRIVER_ReceiveByte();
			digitalDataReceived.secondByte = UART_DRIVER_ReceiveByte();
			portBinValue = dataMessagesTo8bitsByte(digitalDataReceived.firstByte,digitalDataReceived.secondByte);
			HAL_SetPort(digitalDataReceived.port,portBinValue);
			break;

		/**/
		case (ANALOG_MESSAGE):
			digitalDataReceived.port = (receivedByte & 0b00001111);	//Port number
			digitalDataReceived.firstByte = UART_DRIVER_ReceiveByte();
			digitalDataReceived.secondByte = UART_DRIVER_ReceiveByte();
			portBinValue = dataMessagesTo8bitsByte(digitalDataReceived.firstByte,digitalDataReceived.secondByte);
			HAL_SetPWM(digitalDataReceived.port,portBinValue);
			break;


		/*Control Message handling*/
		case(SET_PIN_MODE):

			receivedByte = UART_DRIVER_ReceiveByte();	//pin number
			receivedByteTemp = UART_DRIVER_ReceiveByte();  //Pin mode
			HAL_ChangePinMode(receivedByte,receivedByteTemp);

			break;

		case(REPORT_ANALOG):
			receivedByteTemp = UART_DRIVER_ReceiveByte();	//Get enabled/disabled byte (second byte)
			if(receivedByteTemp)	//Enable Analog pin reporting
			{
				analogInStatus[receivedByte & 0b00001111] = 1;
			}
			else			//Disable  Analog pin reporting
			{
				analogInStatus[receivedByte & 0b00001111] = 0;
			}
			break;

		case(REPORT_DIGITAL):
			receivedByteTemp = UART_DRIVER_ReceiveByte();	//Get enabled/disabled byte (second byte)
			if(receivedByteTemp)	//Enable digital port reporting
			{
				digitalInStatus[receivedByte & 0b00001111] = 1;
			}
			else			// Disable digital port reporting
			{
				digitalInStatus[receivedByte & 0b00001111] = 0;
			}

			break;


		//TODO: System Reset handling
		case(SYSTEM_RESET):
			break;
		}



	vTaskDelayUntil( &xLastExecutionTime, 20 / portTICK_RATE_MS ); // peri�dica
	}
}

/*==================[end of file]============================================*/


