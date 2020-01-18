/* ###################################################################
**     Filename    : Events.h
**     Project     : Test
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-11-03, 23:08, # CodeGen: 22
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         AS1_OnBlockReceived  - void AS1_OnBlockReceived(LDD_TUserData *UserDataPtr);
**         AS1_OnBlockSent      - void AS1_OnBlockSent(LDD_TUserData *UserDataPtr);
**         CI2C1_OnReceiveData  - void CI2C1_OnReceiveData(void);
**         CI2C1_OnTransmitData - void CI2C1_OnTransmitData(void);
**         CI2C1_OnRxChar       - void CI2C1_OnRxChar(void);
**         CI2C1_OnTxChar       - void CI2C1_OnTxChar(void);
**         Cpu_OnNMIINT         - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Servo.h"
#include "PwmLdd1.h"
#include "MotorLinks.h"
#include "PwmLdd2.h"
#include "MotorRechts.h"
#include "PwmLdd3.h"
#include "EN.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "Potis.h"
#include "AdcLdd1.h"
#include "SW1.h"
#include "BitsIoLdd1.h"
#include "SW2.h"
#include "BitsIoLdd2.h"
#include "SW3.h"
#include "BitsIoLdd3.h"
#include "SW4.h"
#include "BitsIoLdd4.h"
#include "LED1.h"
#include "LEDpin2.h"
#include "BitIoLdd2.h"
#include "LED2.h"
#include "LEDpin3.h"
#include "BitIoLdd3.h"
#include "LED3.h"
#include "LEDpin4.h"
#include "BitIoLdd4.h"
#include "LED4.h"
#include "LEDpin5.h"
#include "BitIoLdd5.h"
#include "TasterA.h"
#include "BitsIoLdd5.h"
#include "TasterB.h"
#include "BitsIoLdd6.h"
#include "CLK.h"
#include "LEDpin6.h"
#include "BitIoLdd6.h"
#include "WAIT1.h"
#include "IntI2cLdd1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "TU1.h"
#include "TU2.h"
#include "LinksINT.h"
#include "ExtIntLdd2.h"
#include "Links_Clock.h"
#include "RealTimeLdd2.h"
#include "TU3.h"
#include "SI.h"
#include "BitIoLdd8.h"
#include "Send_OK.h"
#include "LEDpin7.h"
#include "BitIoLdd7.h"
#include "MotorLinks_Rev.h"
#include "PwmLdd4.h"
#include "MotorRechts_Rev.h"
#include "PwmLdd6.h"
#include "RechtsClock.h"
#include "RealTimeLdd1.h"
#include "RechtsINT.h"
#include "ExtIntLdd1.h"
#include "I2C1.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


/*
** ===================================================================
**     Event       :  FC1_OnInterrupt (module Events)
**
**     Component   :  FC1 [FreeCntr]
*/
/*!
**     @brief
**         This event is called when a compare matches the counter
**         value (if compare or reload is selected as a interrupt
**         source) or a counter overflows (for free-running devices).
**         It is valid only when the component is enabled - <"Enable">
**         and the events are enabled - <"EnableEvent">. The event is
**         available only if <Interrupt service/event> is enabled.
*/
/* ===================================================================*/

/*
** ===================================================================
**     Event       :  RechtsINT_OnInterrupt (module Events)
**
**     Component   :  RechtsINT [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  FC1_OnInterrupt (module Events)
**
**     Component   :  FC1 [FreeCntr]
*/
/*!
**     @brief
**         This event is called when a compare matches the counter
**         value (if compare or reload is selected as a interrupt
**         source) or a counter overflows (for free-running devices).
**         It is valid only when the component is enabled - <"Enable">
**         and the events are enabled - <"EnableEvent">. The event is
**         available only if <Interrupt service/event> is enabled.
*/
/* ===================================================================*/
void FC1_OnInterrupt(void);

void RechtsINT_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  RechtsINT_OnInterrupt (module Events)
**
**     Component   :  RechtsINT [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  I2C1_OnReceiveData (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the reception of the
**         data successfully. This event is not available for the SLAVE
**         mode and if both RecvChar and RecvBlock are disabled. This
**         event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2C1_OnReceiveData(void);

/*
** ===================================================================
**     Event       :  I2C1_OnTransmitData (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the transmission of
**         the data successfully. This event is not available for the
**         SLAVE mode and if both SendChar and SendBlock are disabled.
**         This event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2C1_OnTransmitData(void);

/*
** ===================================================================
**     Event       :  I2C1_OnRxChar (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         Called when a correct character is received. This event is
**         not available for the MASTER mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2C1_OnRxChar(void);

/*
** ===================================================================
**     Event       :  I2C1_OnTxChar (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         Called when a correct character is sent (placed to the
**         transmitter). This event is not available for the MASTER
**         mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2C1_OnTxChar(void);

/*
** ===================================================================
**     Event       :  I2C1_OnFullRxBuf (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         Called when the input buffer is full, i.e. after reception
**         of the last character that was successfully placed into
**         input buffer. This event is not available for the MASTER
**         mode. 
**         If OnRxChar event also generated, this event is called after
**         OnRxChar.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2C1_OnFullRxBuf(void);

/*
** ===================================================================
**     Event       :  I2C1_OnError (module Events)
**
**     Component   :  I2C1 [InternalI2C]
**     Description :
**         This event is called when input data buffer has overrun.
**         This event is not available for the MASTER mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2C1_OnError(void);

void LinksINT_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  LinksINT_OnInterrupt (module Events)
**
**     Component   :  LinksINT [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
