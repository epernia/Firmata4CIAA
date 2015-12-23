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

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and definitions]=================================*/

#define TIMER_MATCH_0      0
#define TIMER_MATCH_1      1
#define TIMER_MATCH_2      2
#define TIMER_MATCH_3      3

#define TIMER_0			0
#define TIMER_1			1
#define TIMER_2			2
#define TIMER_3			3

/*==================[typedef]================================================*/
typedef void (*voidFunctionPointer_t)(void);

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
void TIMER_DRIVER_InitTimer(uint8_t  , uint32_t, voidFunctionPointer_t);
void TIMER_DRIVER_DeinitTimer(uint8_t timerNumber);
uint32_t TIMER_DRIVER_microsecondsToTicks(uint32_t uS);
void TIMER_DRIVER_EnableCompareMatchOnTimer(uint8_t , uint8_t , uint32_t , voidFunctionPointer_t );
void TIMER_DRIVER_DisableCompareMatchOnTimer(uint8_t , uint8_t);
void TIMER_DRIVER_SetTimerMatch(uint8_t , uint8_t ,uint32_t );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/


#endif /* TIMER_DRIVER_H_ */
