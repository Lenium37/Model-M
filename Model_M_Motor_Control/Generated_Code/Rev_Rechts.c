/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : Rev_Rechts.c
**     Project     : Model_M_Motor_control
**     Processor   : MKL25Z128VLK4
**     Component   : LED
**     Version     : Component 01.054, Driver 01.00, CPU db: 3.00.000
**     Repository  : My Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-02-12, 12:45, # CodeGen: 69
**     Abstract    :
**          This component implements a universal driver for a single LED.
**     Settings    :
**          Component name                                 : Rev_Rechts
**          Turned On with initialization                  : no
**          HW Interface                                   : 
**            High pin value means ON                      : yes
**            On/Off                                       : Enabled
**              Pin                                        : LEDpin
**            PWM                                          : Disabled
**          Shell                                          : Disabled
**     Contents    :
**         Init       - void Rev_Rechts_Init(void);
**         Deinit     - void Rev_Rechts_Deinit(void);
**         On         - void Rev_Rechts_On(void);
**         Off        - void Rev_Rechts_Off(void);
**         Neg        - void Rev_Rechts_Neg(void);
**         Get        - byte Rev_Rechts_Get(void);
**         Put        - void Rev_Rechts_Put(byte val);
**         SetRatio16 - void Rev_Rechts_SetRatio16(word ratio);
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
** @file Rev_Rechts.c
** @version 01.00
** @brief
**          This component implements a universal driver for a single LED.
*/         
/*!
**  @addtogroup Rev_Rechts_module Rev_Rechts module documentation
**  @{
*/         

/* MODULE Rev_Rechts. */

#include "Rev_Rechts.h"

/*
** ===================================================================
**     Method      :  Rev_Rechts_On (component LED)
**     Description :
**         This turns the LED on.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
** ===================================================================
**     Method      :  Rev_Rechts_Off (component LED)
**     Description :
**         This turns the LED off.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Rev_Rechts_Off(void)
{
  *** This method is implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  Rev_Rechts_Neg (component LED)
**     Description :
**         This negates/toggles the LED
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Rev_Rechts_Neg(void)
{
  *** This method is implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  Rev_Rechts_Get (component LED)
**     Description :
**         This returns logical 1 in case the LED is on, 0 otherwise.
**     Parameters  : None
**     Returns     :
**         ---             - Status of the LED (on or off)
** ===================================================================
*/
/*
byte Rev_Rechts_Get(void)
{
  *** This method is implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  Rev_Rechts_Init (component LED)
**     Description :
**         Performs the LED driver initialization.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Rev_Rechts_Init(void)
{
  *** This method is implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  Rev_Rechts_Put (component LED)
**     Description :
**         Turns the LED on or off.
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - value to define if the LED has to be on or
**                           off.
**     Returns     : Nothing
** ===================================================================
*/
/*
void Rev_Rechts_Put(byte val)
{
  *** This method is implemented as macro in the header file
}
*/

/*
** ===================================================================
**     Method      :  Rev_Rechts_Deinit (component LED)
**     Description :
**         Deinitializes the driver
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Rev_Rechts_Deinit(void)
{
}

/*
** ===================================================================
**     Method      :  Rev_Rechts_SetRatio16 (component LED)
**     Description :
**         Method to specify the duty cycle. If using a PWM pin, this
**         means the duty cycle is set. For On/off pins, values smaller
**         0x7FFF means off, while values greater means on.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ratio           - Ratio value, where 0 means 'off' and
**                           0xffff means 'on'
**     Returns     : Nothing
** ===================================================================
*/
void Rev_Rechts_SetRatio16(word ratio)
{
  /* on/off LED: binary on or off */
  if (ratio<(0xffff/2)) {
    Rev_Rechts_Off();
  } else {
    Rev_Rechts_On();
  }
}

/* END Rev_Rechts. */

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
