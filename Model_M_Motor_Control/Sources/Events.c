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
 **         CI2C1_OnReceiveZener  - void CI2C1_OnReceiveZener(void);
 **         CI2C1_OnTransmitZener - void CI2C1_OnTransmitZener(void);
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
//float Abstand_Mag = 0.01383;
float Abstand_Mag = 0.0117;
float velocity_Links;
float velocity_Rechts;
extern float velocity_Rechts_avg;
extern float velocity_links_avg;
extern bool first_pulse;
float Rechts_time = 0;
float Links_time = 0;
float velocity_Rechts_avg = 0.0;
volatile uint16_t Speed_time_Links = 0;
uint16_t Speed_time_Rechts = 0;



extern bool flags_send;
extern byte flags;
extern bool flag_buffer_empty;
bool first_pulse_active = TRUE;

extern uint16_t Pulse_counter;


extern volatile int64_t Counter_OVF;
uint8_t avg_counter_rechts = 0;
uint8_t avg_counter_links = 0;

uint64_t TU3_CounterValue_links = 0;
uint64_t TU3_CounterValue_rechts = 0;
int8_t first_pulse_rechts = 0;
int8_t first_pulse_links = 0;
float Rechst_time_prev = 0;
float Links_time_prev = 0;
float LPF_Beta = 0.5;

extern struct {
  char Direction;
  char Hundert;
  char Zener;
  char Einer;
  char straight_curve;
  char align;
} message;
extern struct {
  char Direction_buffer;
  char Hundert_buffer;
  char Zener_buffer;
  char Einer_buffer;
  char straight_curve_buffer;
  char align_buffer;
} message_buffer;
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
			TU3_CounterValue_rechts = Speed_time_Rechts+(Counter_OVF*65535);
			Rechts_time = 0.0000000417*TU3_CounterValue_rechts;

			first_pulse_rechts++;
			if(first_pulse_rechts == 2)
			{
				float Rechts_time_real = Rechts_time - Rechst_time_prev;

				velocity_Rechts += Abstand_Mag/Rechts_time_real;
				avg_counter_rechts++;
				first_pulse_rechts = 1;
			}
			if(avg_counter_rechts > 10 && first_pulse_active == TRUE)
			{
				first_pulse = TRUE;
				first_pulse_active = FALSE;
			}

			if(avg_counter_rechts > 0)
			{
				 velocity_Rechts_avg = velocity_Rechts_avg - (LPF_Beta * (velocity_Rechts_avg - velocity_Rechts));
				//printf("Speed_rechts: %f\n\n",velocity_Rechts_avg);
				if(velocity_Rechts_avg > 5.5)
				{
					velocity_Rechts_avg = 0;
				}
				velocity_Rechts = 0;
				avg_counter_rechts = 0;
			}
			//Counter_OVF = 0;
			Rechst_time_prev = Rechts_time;
			//TU3_ResetCounter(TU3_Pointer);
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
 **     Event       :  I2C1_OnTransmitZener (module Events)
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
				Pulse_counter = 0;
				Speed_time_Links = TU3_GetCounterValue(TU3_Pointer);
				TU3_CounterValue_links = Speed_time_Links+(Counter_OVF*65535);
				Links_time = 0.0000000417*TU3_CounterValue_links;
				//printf("Links \n");
				first_pulse_links++;
				if(first_pulse_links == 2)
				{
					float Links_time_real = Links_time - Links_time_prev;

					velocity_Links += Abstand_Mag/Links_time_real;
					//printf("SPEED: %f \n",velocity_Rechts);
					avg_counter_links++;
					first_pulse_links = 1;
				}
				if(avg_counter_links > 10 && first_pulse_active == TRUE)
				{
					first_pulse = TRUE;
					first_pulse_active = FALSE;
				}

				if(avg_counter_links > 0)
				{
					velocity_links_avg = velocity_links_avg - (LPF_Beta * (velocity_links_avg - velocity_Links));
					//printf("Speed_links: %f\n\n",velocity_links_avg);
					velocity_Links = 0;
					avg_counter_links = 0;
				}
				//Counter_OVF = 0;
				Links_time_prev = Links_time;
				//TU3_ResetCounter(TU3_Pointer);
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
**         UserZenerPtr     - Pointer to the user or
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


/*
** ===================================================================
**     Event       :  I2C1_OnReceiveZener (module Events)
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

/*
** ===================================================================
**     Event       :  CI2C1_OnReceiveZener (module Events)
**
**     Component   :  CI2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the reception of the
**         data successfully. This event is not available for the SLAVE
**         mode and if both RecvChar and RecvBlock are disabled. This
**         event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CI2C1_OnReceiveZener(void)
{
  flags = 1;
}

/*
** ===================================================================
**     Event       :  CI2C1_OnTransmitZener (module Events)
**
**     Component   :  CI2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the transmission of
**         the data successfully. This event is not available for the
**         SLAVE mode and if both SendChar and SendBlock are disabled.
**         This event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CI2C1_OnTransmitZener(void)
{
	flags_send = 1;
}

/*
** ===================================================================
**     Event       :  AS1_OnError (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnRxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnTxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  AS1_OnFullRxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnFullRxBuf(void)
{
	word Received;
	byte err;
	err = AS1_RecvBlock((byte*)&message_buffer, sizeof(message_buffer), &Received);
	AS1_ClearRxBuf();
	flag_buffer_empty = TRUE;

	message.Direction = message_buffer.Direction_buffer;
	message.Einer =	message_buffer.Einer_buffer;
	message.Zener =	message_buffer.Zener_buffer;
	message.Hundert = message_buffer.Hundert_buffer;
	message.straight_curve = message_buffer.straight_curve_buffer;
	message.align = message_buffer.align_buffer;

	if(message.align != 'A')
				{
					if(message.Direction == 'A')
					{
						message.Direction = message_buffer.Hundert_buffer; 		// AR150C // R150CA
						message.Hundert = message_buffer.Zener_buffer;
						message.Zener =	message_buffer.Einer_buffer;
						message.Einer =	message_buffer.straight_curve_buffer;
						message.straight_curve = message_buffer.align_buffer;
						message.align = message_buffer.Direction_buffer;

					}else if(message.Einer == 'A')
					{
						message.Direction = message_buffer.Zener_buffer; 		// CAR150 // R150CA
						message.Hundert = message_buffer.Einer_buffer;
						message.Zener =	message_buffer.straight_curve_buffer;
						message.Einer =	message_buffer.align_buffer;
						message.straight_curve = message_buffer.Direction_buffer;
						message.align = message_buffer.Hundert_buffer;

					}else if(message.Zener == 'A')
					{
						message.Direction = message_buffer.Einer_buffer; 		// 0CAR15 // R150CA
						message.Hundert = message_buffer.straight_curve_buffer;
						message.Zener =	message_buffer.align_buffer;
						message.Einer =	message_buffer.Direction_buffer;
						message.straight_curve = message_buffer.Hundert_buffer;
						message.align = message_buffer.Zener_buffer;

					}else if(message.Hundert == 'A')
					{
						message.Direction = message_buffer.straight_curve_buffer; 		// 50CAR1 // R150CA
						message.Hundert = message_buffer.align_buffer;
						message.Zener =	message_buffer.Direction_buffer;
						message.Einer =	message_buffer.Hundert_buffer;
						message.straight_curve = message_buffer.Zener_buffer;
						message.align = message_buffer.Einer_buffer;

					}else if(message.straight_curve == 'A')
					{
						message.Direction = message_buffer.align_buffer; 		// 150CAR // R150CA
						message.Hundert = message_buffer.Direction_buffer;
						message.Zener =	message_buffer.Hundert_buffer;
						message.Einer =	message_buffer.Zener_buffer;
						message.straight_curve = message_buffer.Einer_buffer;
						message.align = message_buffer.straight_curve_buffer;

					}
				}

}

/*
** ===================================================================
**     Event       :  AS1_OnFreeTxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS1_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  CI2C1_OnReceiveData (module Events)
**
**     Component   :  CI2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the reception of the
**         data successfully. This event is not available for the SLAVE
**         mode and if both RecvChar and RecvBlock are disabled. This
**         event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CI2C1_OnReceiveData(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  CI2C1_OnTransmitData (module Events)
**
**     Component   :  CI2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the transmission of
**         the data successfully. This event is not available for the
**         SLAVE mode and if both SendChar and SendBlock are disabled.
**         This event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CI2C1_OnTransmitData(void)
{
  /* Write your code here ... */
}

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
