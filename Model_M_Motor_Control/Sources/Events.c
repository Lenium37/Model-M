/* ###################################################################
 **     Filename    : Events.c
 **     Project     : Test
 **     Processor   : MKL25Z128VLK4
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2019-11-05, 19:47, # CodeGen: 71
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Contents    :
 **         CI2C2_OnReadReq      - void CI2C2_OnReadReq(void);
 **         CI2C2_OnWriteReq     - void CI2C2_OnWriteReq(void);
 **         CI2C2_OnFreeTxBuf    - void CI2C2_OnFreeTxBuf(void);
 **         CI2C2_OnFullRxBuf    - void CI2C2_OnFullRxBuf(void);
 **         FC1_OnInterrupt      - void FC1_OnInterrupt(void);
 **         CI2C1_OnReceiveData  - void CI2C1_OnReceiveData(void);
 **         CI2C1_OnTransmitData - void CI2C1_OnTransmitData(void);
 **         CI2C1_OnRxChar       - void CI2C1_OnRxChar(void);
 **         CI2C1_OnTxChar       - void CI2C1_OnTxChar(void);
 **         Cpu_OnNMIINT         - void Cpu_OnNMIINT(void);
 **
 ** ###################################################################*/

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 

extern int Error;
float Umfang = 0.157; //Meter Umfang
float Abstand_Mag = 0.01383;
float velocity_Links;
float velocity_Rechts;
extern float velocity_Rechts_avg;
float Rechts_time = 0;
float velocity_Rechts_avg = 0.0;
volatile uint16_t Speed_time_Links = 0;
uint16_t Speed_time_Rechts = 0;

extern bool flags_rev;
extern bool errFlags_rev;
extern bool flags_send;

extern uint16_t Pulse_counter;

extern bool Serial;
bool Start_up_Rechts = TRUE;
bool Start_up_Links  = TRUE;
int count = 0;
volatile uint8_t Counter_OVF = 0;
uint8_t avg_counter = 0;
uint32_t TU3_CounterValue = 0;
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
void Cpu_OnNMIINT(void) {
	/* Write your code here ... */
}
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
void RechtsINT_OnInterrupt(void)
{
			Pulse_counter = 0;
			Speed_time_Rechts = TU3_GetCounterValue(TU3_Pointer);
			TU3_CounterValue = Speed_time_Rechts+(Counter_OVF*65535);
			Rechts_time = 0.000002*TU3_CounterValue;
			velocity_Rechts += Abstand_Mag/Rechts_time;
			avg_counter++;
			if(avg_counter > 10)
			{
				velocity_Rechts_avg = velocity_Rechts/10;
				velocity_Rechts = 0;
				avg_counter = 0;
			}
			Counter_OVF = 0;
			TU3_ResetCounter(TU3_Pointer);
	}
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
void FC1_OnInterrupt(void) {
	//Timer_OVF++;
}

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
void I2C1_OnReceiveData(void) {
	/* Write your code here ... */
}

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
void I2C1_OnTransmitData(void) {
	flags_send = TRUE;
	if (Serial == TRUE) {
		printf("Data_Send\n");
	}
}

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
void I2C1_OnRxChar(void) {
	/* Write your code here ... */
}

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
void I2C1_OnTxChar(void) {
	/* Write your code here ... */
}

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
void I2C1_OnFullRxBuf(void) {
	flags_rev = TRUE;

}

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
void I2C1_OnError(void) {
	errFlags_rev = TRUE;
}

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
void LinksINT_OnInterrupt(void)
{
	/*/PID_Active = TRUE;
			//FC1_Disable();
			uint8_t AVGMAX = 4;
			RechtsClock_Disable();  //Deaktivieren vom Timer um Zeit zu Speichern
			double AVG_Speed_Time = 0.0;
			counter(TRUE);
			LinksINT_Disable(); //Interrupt Deaktivieren
			if (Start_up_Links == TRUE)//Beim ersten auslösen des Interrupts nach dem Start den Timer Starten
			{
				//FC1_Enable();
				RechtsClock_Enable();
				Start_up_Links = FALSE;
			}
			if (count <= 1)	  //Ersten Impulse Zählen
					{
				count++;
			}
			if (count >= 2) //Beim Ersten Impulse auf den Zweiten warten erst dann Zeit Messen
			{
				RechtsClock_GetTimeMS(&Speed_time_Links);
				AVG_ARRAY_Links[avgcounter] = Speed_time_Links;			//Zeit in einem Array Speichern
				Speed_time_Links = 0;			//Ausgelesener Timerwert wieder wieder auf null
				avgcounter++;							//Array Zähler um eins erhöhen
				RechtsClock_Reset();
				RechtsClock_Enable();
			}
			if (avgcounter >= AVGMAX)//Wenn 4 Impulse gemessen wurden berechne den Mittelwert um Fehler zu vermindern
					{
				for (int i = 0; i < AVGMAX; i++)
				{
					AVG_Speed_Time = AVG_Speed_Time + AVG_ARRAY_Links[i];	//Alle Samples addieren
				}
				AVG_Speed_Time = AVG_Speed_Time / AVGMAX;				//Mittelwert bilden
				velocity_Links = Abstand_Mag / AVG_Speed_Time * 1000;	//Geschwindigkeit berechnen um auf m/s zu kommen muss die Geschwindigkeit * 1000 gerehnet werden
				avgcounter = 0;							//Array Zähler wieder auf null
			}

			LinksINT_Enable();									//Interrupt wieder Starten
			printf("velocity_Links = %d\n",velocity_Links);*/
}

/*
** ===================================================================
**     Event       :  TU3_OnCounterRestart (module Events)
**
**     Component   :  TU3 [TimerUnit_LDD]
*/
/*!
**     @brief
**         Called if counter overflow/underflow or counter is
**         reinitialized by modulo or compare register matching.
**         OnCounterRestart event and Timer unit must be enabled. See
**         [SetEventMask] and [GetEventMask] methods. This event is
**         available only if a [Interrupt] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TU3_OnCounterRestart(LDD_TUserData *UserDataPtr)
{
   Counter_OVF++;
}

/*
** ===================================================================
**     Event       :  MotorRechts_OnEnd (module Events)
**
**     Component   :  MotorRechts [PWM]
**     Description :
**         This event is called when the specified number of cycles has
**         been generated. (Only when the component is enabled -
**         <Enable> and the events are enabled - <EnableEvent>). The
**         event is available only when the <Interrupt service/event>
**         property is enabled and selected peripheral supports
**         appropriate interrupt.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


/* END Events */

#ifdef __cplusplus
} /* extern "C" */
#endif 

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
