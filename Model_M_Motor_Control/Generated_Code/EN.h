/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : EN.h
**     Project     : Model_M_Motor_control
**     Processor   : MKL25Z128VLK4
**     Component   : LED
**     Version     : Component 01.054, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-01-07, 15:26, # CodeGen: 0
**     Abstract    :
**          This component implements a universal driver for a single LED.
**     Settings    :
**          Component name                                 : EN
**          Turned On with initialization                  : no
**          HW Interface                                   : 
**            High pin value means ON                      : yes
**            On/Off                                       : Enabled
**              Pin                                        : LEDpin
**            PWM                                          : Disabled
**          Shell                                          : Disabled
**     Contents    :
**         Init - void EN_Init(void);
**         On   - void EN_On(void);
**         Off  - void EN_Off(void);
**         Neg  - void EN_Neg(void);
**         Get  - byte EN_Get(void);
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
** @file EN.h
** @version 01.00
** @brief
**          This component implements a universal driver for a single LED.
*/         
/*!
**  @addtogroup EN_module EN module documentation
**  @{
*/         

#ifndef __EN_H
#define __EN_H

/* MODULE EN. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "LEDpin1.h"

#include "Cpu.h"

#define EN_ClrVal()    LEDpin1_ClrVal() /* put the pin on low level */
#define EN_SetVal()    LEDpin1_SetVal() /* put the pin on high level */
#define EN_SetInput()  LEDpin1_SetInput() /* use the pin as input pin */
#define EN_SetOutput() LEDpin1_SetOutput() /* use the pin as output pin */

#define EN_PARSE_COMMAND_ENABLED  0 /* set to 1 if method ParseCommand() is present, 0 otherwise */


#define EN_On() LEDpin1_SetVal()
/*
** ===================================================================
**     Method      :  EN_On (component LED)
**     Description :
**         This turns the LED on.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define EN_Off() LEDpin1_ClrVal()
/*
** ===================================================================
**     Method      :  EN_Off (component LED)
**     Description :
**         This turns the LED off.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define EN_Neg() LEDpin1_NegVal()
/*
** ===================================================================
**     Method      :  EN_Neg (component LED)
**     Description :
**         This negates/toggles the LED
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define EN_Get() LEDpin1_GetVal()
/*
** ===================================================================
**     Method      :  EN_Get (component LED)
**     Description :
**         This returns logical 1 in case the LED is on, 0 otherwise.
**     Parameters  : None
**     Returns     :
**         ---             - Status of the LED (on or off)
** ===================================================================
*/

#define EN_Init() EN_Off()
/*
** ===================================================================
**     Method      :  EN_Init (component LED)
**     Description :
**         Performs the LED driver initialization.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END EN. */

#endif
/* ifndef __EN_H */
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
