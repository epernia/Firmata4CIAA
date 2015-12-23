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

#ifndef SERVO_DRIVER_H_
#define SERVO_DRIVER_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and definitions]=================================*/

/*==================[typedef]================================================*/
typedef struct
{
   int8_t pin;                    /* Pin number */
   uint32_t usFor0DegreAngle;     /* Microseconds for 0-degre angle */
   uint32_t usFor180DegreAngle;   /* Microseconds for 180-degre angle */
   uint32_t usForStop;            /* Microseconds for 90-degre angle or when stops */
} servoConfig_t;

/* Define Boolean Data Type */
typedef uint8_t bool_t;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
void servoConfig(void);
bool_t servoAttach( uint8_t pin);
bool_t servoAttachWithConfig( uint8_t pin, servoConfig_t * servoConfig );// TO BE IMPLEMENTED
uint8_t isServoAttached( uint8_t pin );
bool_t servoDetach( uint8_t pin );
bool_t servoWrite( uint8_t pin, uint8_t angle );
uint8_t servoRead( uint8_t pin );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/

#endif /* SERVO_DRIVER_H_ */
