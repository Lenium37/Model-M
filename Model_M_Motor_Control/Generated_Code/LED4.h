/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : LED4.h
**     Project     : Model_M_Motor_control
**     Processor   : MKL25Z128VLK4
**     Component   : LED
**     Version     : Component 01.054, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-01-07, 17:48, # CodeGen: 1
**     Abstract    :
**          This component implements a universal driver for a single LED.
**     Settings    :
**          Component name                                 : LED4
**          Turned On with initialization                  : no
**          HW Interface                                   : 
**            High pin value means ON                      : yes
**            On/Off                                       : Enabled
**              Pin                                        : LEDpin
**            PWM                                          : Disabled
**          Shell                                          : Disabled
**     Contents    :
**         Init   - void LED4_Init(void);
**         Deinit - void LED4_Deinit(void);
**         On     - void LED4_On(void);
**         Off    - void LED4_Off(void);
**         Neg    - void LED4_Neg(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2012, all rights reserved.
**     Web: http://mcuoneclipse.com
**     This an open source software implementing an LED driver using Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file LED4.h
** @version 01.00
** @brief
**          This component implements a universal driver for a single LED.
*/         
/*!
**  @addtogroup LED4_module LED4 module documentation
**  @{
*/         

#ifndef __LED4_H
#define __LED4_H

/* MODULE LED4. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "LEDpin5.h"

#include "Cpu.h"

#define LED4_ClrVal()    LEDpin5_ClrVal() /* put the pin on low level */
#define LED4_SetVal()    LEDpin5_SetVal() /* put the pin on high level */
#define LED4_SetInput()  LEDpin5_SetInput() /* use the pin as input pin */
#define LED4_SetOutput() LEDpin5_SetOutput() /* use the pin as output pin */

#define LED4_PARSE_COMMAND_ENABLED  0 /* set to 1 if method ParseCommand() is present, 0 otherwise */


#define LED4_On() LEDpin5_SetVal()
/*
** ===================================================================
**     Method      :  LED4_On (component LED)
**     Description :
**         This turns the LED on.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define LED4_Off() LEDpin5_ClrVal()
/*
** ===================================================================
**     Method      :  LED4_Off (component LED)
**     Description :
**         This turns the LED off.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define LED4_Neg() LEDpin5_NegVal()
/*
** ===================================================================
**     Method      :  LED4_Neg (component LED)
**     Description :
**         This negates/toggles the LED
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define LED4_Init() LED4_Off()
/*
** ===================================================================
**     Method      :  LED4_Init (component LED)
**     Description :
**         Performs the LED driver initialization.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void LED4_Deinit(void);
/*
** ===================================================================
**     Method      :  LED4_Deinit (component LED)
**     Description :
**         Deinitializes the driver
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END LED4. */

#endif
/* ifndef __LED4_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
