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

#ifndef HAL_H_
#define HAL_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and definitions]=================================*/
#define OFF						0
#define ON						1

// Number of Analog and Digital pins used.
#define TOTAL_ANALOG_PINS		3	//
#define DIGITAL_INPUTS			4
#define DIGITAL_OUTPUTS			28	//FIXME: para el modulo firmata. sacar despues
#define TOTAL_DIGITAL_PINS		(DIGITAL_OUTPUTS + DIGITAL_INPUTS)
#define TOTAL_DIGITAL_PORTS		4 //ceil(TOTAL_DIGITAL_PINS/8)
#define TOTAL_PINS				(TOTAL_ANALOG_PINS + TOTAL_DIGITAL_PINS)

// pin modes
#define INPUT 	                0x00 // digital pin in analogInput mode
#define OUTPUT	                0x01 // digital pin in analogOutput mode
#define ANALOG                  0x02 // analog pin in analogInput mode
#define PWM                     0x03 // digital pin in PWM output mode
#define SERVO                   0x04 // digital pin in Servo output mode
#define SHIFT                   0x05 // shiftIn/shiftOut mode
#define I2C                     0x06 // pin included in I2C setup
#define TOTAL_PIN_MODES         7

/*	------------------ Mapeo de pines ------------------ */
#define PIN0 					0		/*PULS_0   TEC_1*/
#define PIN1 					1		/*PULS_1   TEC_2*/
#define PIN2 					2		/*PULS_2   TEC_3*/
#define PIN3 					3		/*PULS_3   TEC_4*/
#define PIN4 					4		/*LED_R*/
#define PIN5 					5		/*LED_G*/
#define PIN6 					6		/*LED_B*/
#define PIN7 					7		/*LED1*/
#define PIN8 					8		/*LED2*/
#define PIN9 					9		/*LED3*/
#define PIN10 					10		/*GPIO0*/
#define PIN11 					11		/*GPIO1*/
#define PIN12					12		/*GPIO2*/
#define PIN13 					13		/*GPIO3*/
#define PIN14 					14		/*GPIO4*/
#define PIN15 					15		/*GPIO5*/
#define PIN16 					16		/*GPIO6*/
#define PIN17 					17		/*GPIO7*/
#define PIN18 					18		/*GPIO8*/
#define PIN19					19		/*LCD1*/
#define PIN20 					20		/*LCD2*/
#define PIN21 					21		/*LCD3*/
#define PIN22 					22		/*LCD4*/
#define PIN23					23		/*LCD_RS*/
#define PIN24 					24		/*LCD_EN*/
#define PIN25 					25		/*T_FIL0*/
#define PIN26 					26		/*T_FIL1*/
#define PIN27 					27		/*T_FIL2*/
#define PIN28 					28		/*T_FIL3*/
#define PIN29 					29		/*T_COL0*/
#define PIN30 					30		/*T_COL1*/
#define PIN31 					31		/*T_COL2*/

/*==================[typedef]================================================*/
/*Pin name; necesary to configure pin function with Chip_SCU_PinMux.
 * eg: P2_12 ("port"=2, "pin"=12) for LED0_R*/
typedef struct
{
	OPTIMAL_DATA_TYPE port;
	OPTIMAL_DATA_TYPE pin;
}PN_t;

/*GPIO info of a pin, necesary to set a direction for a GPIO with Chip_GPIO_SetDir.
 * eg: GPIO1[12] ("port"=1, "pin"=12), FUNC0 for LED1*/
typedef struct
{
	OPTIMAL_DATA_TYPE port;
	OPTIMAL_DATA_TYPE pin;
	OPTIMAL_DATA_TYPE func;
}GPIO_t;

/*PWM info of a pin, necesary to config pwm output
 * eg: CTOUT_2, FUNC1 for LED1*/
typedef struct
{
	OPTIMAL_DATA_TYPE ctout;
	OPTIMAL_DATA_TYPE func;
	OPTIMAL_DATA_TYPE mode;
	OPTIMAL_DATA_TYPE servoValue;
}PWM_t;

/*Configuration structure. New functions can be added
 * by creating another struct with the necessary info for config */
typedef struct
{
	PN_t PNs;	//s de struct
	GPIO_t GPIOs;
	PWM_t PWMs;

} ciaaP;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
void HAL_Init(void);

OPTIMAL_DATA_TYPE HAL_CheckPin(OPTIMAL_DATA_TYPE );

OPTIMAL_DATA_TYPE HAL_CheckPinPWMFunc(OPTIMAL_DATA_TYPE);

void HAL_SetOutput(OPTIMAL_DATA_TYPE , OPTIMAL_DATA_TYPE );

OPTIMAL_DATA_TYPE HAL_GetPinState(OPTIMAL_DATA_TYPE );

int16_t HAL_GetAnalogPin(OPTIMAL_DATA_TYPE );

void HAL_SetPWM(OPTIMAL_DATA_TYPE , uint8_t );

void HAL_ChangePinMode(OPTIMAL_DATA_TYPE, OPTIMAL_DATA_TYPE );

void HAL_SetPort(uint8_t , uint8_t );

int16_t HAL_GetPort(uint8_t );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/

#endif /* HAL_H_ */
