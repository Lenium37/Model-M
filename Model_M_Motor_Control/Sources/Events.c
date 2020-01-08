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
extern float velocity;
uint16_t Speed_time = 0;
double Speed = 0.0;
double Tick_Time = 0.0;
double AVG_ARRAY[4];
extern bool PID_Active;
extern int counter();
/* User includes (#include below this line is not maintained by Processor Expert) */
//I2C
extern bool flags_rev;
extern bool errFlags_rev;
extern bool flags_send;

extern bool Serial;
bool Start_up = TRUE;
int count = 0;
uint8_t Timer_OVF = 0;
uint8_t avgcounter = 0;
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
 **     Event       :  EInt1_OnInterrupt (module Events)
 **
 **     Component   :  EInt1 [ExtInt]
 **     Description :
 **         This event is called when an active signal edge/level has
 **         occurred.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void EInt1_OnInterrupt(void) {
	PID_Active = TRUE;
	FC1_Disable(); //Deaktivieren vom Timer um Zeit zu Speichern
	double AVG_Speed_Time = 0.0;
	counter(TRUE);
	EInt1_Disable(); //Interrupt Deaktivieren
	if (Start_up == TRUE)//Beim ersten auslösen des Interrupts nach dem Start den Timer Starten
	{
		FC1_Enable();
		Start_up = FALSE;
	}
	if (count <= 1)	  //Ersten Impulse Zählen
			{
		count++;
	}
	if (count >= 2) //Beim Ersten Impulse auf den Zweiten warten erst dann Zeit Messen
			{
		FC1_GetCounterValue(&Speed_time); //Timer Wert holen

		if (Timer_OVF != 0) //Wenn der Timer einen Overflow hat anzahl diser Multiplizieren
				{
			//Speed = Speed_time*Timer_OVF*2.666/1000; //Timerwert mal Overflow mal Schrittzeit und durch 1000 um auf milisekuden zu kommen
			if (Serial == TRUE) {
				printf("OVF = %d\n", Timer_OVF);		  //Timer OVF print
			}
		} else//Wenn Kein Overflow dann den Faktor nicht mit einbeziehen sonst ist das Ergebnis immer gleich 0
		{
			Speed = Speed_time * 2.666 / 1000;
		}
		AVG_ARRAY[avgcounter] = Speed;			//Zeit in einem Array Speichern
		Timer_OVF = 0;						//Timer_OVF wieder auf null netzen
		Speed_time = 0;			//Ausgelesener Timerwert wieder wieder auf null
		avgcounter++;							//Array Zähler um eins erhöhen
		FC1_Reset();								//Timer zurücksetzen
		FC1_Enable();								//Erstdann wieder aktivieren
	}
	if (avgcounter >= 4)//Wenn 12 Impulse gemessen wurden berechne den Mittelwert um Fehler zu vermindern
			{
		for (int i = 0; i < 4; i++) {
			AVG_Speed_Time = AVG_Speed_Time + AVG_ARRAY[i];	//Alle Samples addieren
		}
		AVG_Speed_Time = AVG_Speed_Time / 4;				//Mittelwert bilden
		velocity = Abstand_Mag / AVG_Speed_Time * 1000;	//Geschwindigkeit berechnen um auf m/s zu kommen muss die Geschwindigkeit * 1000 gerehnet werden um auf m/s zu kommen
		avgcounter = 0;							//Array Zähler wieder auf null
	}

	EInt1_Enable();									//Interrupt wieder Starten
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
	Timer_OVF++;
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
