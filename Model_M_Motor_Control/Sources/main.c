/* Including needed modules to compile this module/procedure */
#include <stdio.h>
#include "Cpu.h"
#include "Events.h"
#include "Servo.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "MotorLinks.h"
#include "PwmLdd2.h"
#include "TU2.h"
#include "LinksINT.h"
#include "ExtIntLdd2.h"
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
#include "RechtsINT.h"
#include "ExtIntLdd1.h"
#include "I2C1.h"
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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
uint16_t value;
uint16_t value2;
uint16_t Pixel[128];
uint8_t prev_angle = 0;
uint16_t Programmtime;
int Curve = 0;
int Break_time = 0;
uint16_t Break_period = 0;

//Routine Auswertung
uint16_t Pixel[128];
uint16_t Pixel_Auswertung[128];
uint8_t anzahl_null = 0;
uint8_t anzahl_eins = 0;
uint8_t erkennung_links = 1;
uint8_t erkennung_rechts = 1;
uint8_t Linke_Linie = 0;
uint8_t Rechte_Linie = 0;
uint8_t null_diff = 0;

//I2c
bool flags_rev;
bool errFlags_rev;
bool flags_send;
byte err_rev;
byte data[4];
word ret_rev;
uint8_t Angle = 0;
#define SLAVE 0x09

//Allgemeine Variablen
uint8_t pulse = 0;
uint8_t array_pos = 0;
uint8_t startup = 1;
int SerVal = 0;
extern uint8_t avgcounter;
uint16_t StartMotor = 50000;
//PID
uint16_t P_VAL = 0;

float Regel = 0;
float PID_p, PID_d, PID_i = 0;
double Period = 0;
float Speed_diff = 0;
float PID_total = 0;

uint8_t Timer_ovf = 0;

uint32_t Tick = 0;
bool Serial_Active = FALSE;
bool PID_Active = TRUE;
char Data;

//Routine Linien_zaehlen
uint8_t anzahl_linien = 0;
uint8_t pre_null = 0;
uint8_t anzahl_null_bar = 0;
uint8_t anzahl_eins_bar = 0;
uint16_t Pixel_Null[7];
uint8_t LinienBreite[10];

uint16_t Mitte = 60750;
uint16_t Links = 61650;
uint16_t Rechts = 59860;
float velocity_Links = 0.0;
float velocity_Rechts = 0.0;
float velocity_Rechts_avg;
int Error = 0;
uint8_t Magnenten = 8;
extern double AVG_ARRAY_Links[];
extern double AVG_ARRAY_Rechts[];
float RadUmfang = 157.08; //mm
uint16_t DreiLinien = 0;
uint16_t VierLinien = 0;
bool Start = FALSE;
bool Serial = FALSE;
bool Serialline = FALSE;
bool Break_Curve = FALSE;
byte Break_Active = 0;
bool DRIVE = TRUE;
int Break_Timer = 0;
uint16_t Winkel_prev_L = 0;
uint16_t Winkel_prev_R = 0;
uint16_t Lenkung_Regler = 0;
uint16_t Programmcounter = 0;
long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void counter(bool Stop) //Wenn kein Interrupt mehr nach 30 Ticks ausgelöst wird dann ist die Geschwindigkeit gleich null oder der Speedsensor hat ein Error
{

}
uint16_t Regler_P(float Soll, float Ist, int Kp) {
	float Speed_diff = Soll - Ist;
	return P_VAL += Speed_diff * Kp;
}

uint8_t char_int(char Zeichen) {
	uint8_t Wert = 0;
	switch (Zeichen) {
	case '0':
		Wert = 0;
		break;
	case '1':
		Wert = 1;
		break;
	case '2':
		Wert = 2;
		break;
	case '3':
		Wert = 3;
		break;
	case '4':
		Wert = 4;
		break;
	case '5':
		Wert = 5;
		break;
	case '6':
		Wert = 6;
		break;
	case '7':
		Wert = 7;
		break;
	case '8':
		Wert = 8;
		break;
	case '9':
		Wert = 9;
		break;
	}
	return Wert;
}
int Cal_Angle(uint8_t Ten, uint8_t Zero) {

	return Ten * 10 + Zero;
}
void Send(char Data, uint8_t Block_send) {
	byte err_send;
	word ret_send;
	/* Set slave address */
	(void) I2C1_SelectSlave(SLAVE);

	/* Clear "Complete" flag */
	flags_send = FALSE;

	/* Send bytes in master mode */
	err_send = I2C1_SendBlock(&Data, Block_send, &ret_send);
	if (Serial == TRUE) {
		if (err_send == ERR_OK) {
			printf("ERR_OK\n");
		}
		if (err_send == ERR_SPEED) {
			printf("ERR_SPEED\n");
		}
		if (err_send == ERR_DISABLED) {
			printf("ERR_DISABLED\n");
		}
		if (err_send == ERR_BUSY) {
			printf("ERR_BUSY\n");
		}
		if (err_send == ERR_BUSOFF) {
			printf("ERR_BUSOFF\n");
		}
		if (err_send == ERR_TXFULL) {
			printf("ERR_TXFULL\n");
		}
		if (err_send == ERR_ARBITR) {
			printf("ERR_ARBITR\n");
		}
	}
	/*
	 ERR_OK - OK
	 ERR_SPEED - This device does not work in the active speed mode
	 ERR_DISABLED - Device is disabled
	 ERR_BUSY - The slave device is busy, it does not respond by the acknowledge (only in master mode and when interrupt service is disabled)
	 ERR_BUSOFF - Clock timeout elapsed or device cannot transmit data
	 ERR_TXFULL - Transmitter is full. Some data has not been sent. (slave mode only)
	 ERR_ARBITR - Arbitration lost (only when interrupt service is disabled and in master mode)
	 */
	/* Wait for communication complete */
	while (!flags_send) {
		int count = 0;
		while (!flags_rev) /* Wait for full buffer */
		{
			count++;
			if (count > 3000) {
				flags_rev = TRUE; /* Clear flags */
			}
			//Send('R',1);
		}
	}
}
void Rec() {
	flags_rev = FALSE; /* Clear flags */
	errFlags_rev = FALSE;
	int count = 0;
	while (!flags_rev) /* Wait for full buffer */
	{
		Send_OK_On();
		//printf("Angle: %d, %d\n", Angle,SerVal);
		count++;
		if (count > 30000) {
			flags_rev = TRUE; /* Clear flags*/
			count = 0;
		}
		//Send('R',1);
	}
	//Send('B',1);
	//while (!errFlags_rev);
	Send_OK_Off();
	/* Receive bytes in slave mode (read the input buffer) */
	err_rev = I2C1_RecvBlock(&data, 8, &ret_rev);
	err_rev = 100;
	uint8_t Ten = char_int(data[1]);
	uint8_t Zero = char_int(data[2]);
	prev_angle = Angle;
	if (data[0] != 'X' || data[1] != 'X' || data[2] != 'X' || data[3] != 'X') {
		Angle = Cal_Angle(Ten, Zero);
	}
	if (data[0] == 'X' || data[1] == 'X' || data[2] == 'X' || data[3] == 'X') {
		Angle = prev_angle;
	}
	SerVal = map(Angle, 0, 45, 0, 819); //819 //3277 //max 62258 min 58981; 819 sind 45° , mitte 60619
	//printf("Data: %c%c%c\n",data[0],data[1],data[2],data[3]);
	//printf("Angle: %d, %d\n", Angle,SerVal);
	//if(data[1] == 'L'||data[1]=='R'||data[1]=='S'||data[2] == 'L'||data[2]=='R'||data[2]=='S')
	//{
	I2C1_ClearRxBuf();
	//}
	if (Serial == TRUE) {
		if (err_rev == ERR_OK) {
			printf("ERR_OK\n");
		}
		if (err_rev == ERR_SPEED) {
			printf("ERR_SPEED\n");
		}
		if (err_rev == ERR_DISABLED) {
			printf("ERR_DISABLED\n");
		}
		if (err_rev == ERR_BUSY) {
			printf("ERR_BUSY\n");
		}
		if (err_rev == ERR_BUSOFF) {
			printf("ERR_BUSOFF\n");
		}
		if (err_rev == ERR_RXEMPTY) {
			printf("ERR_RXEMPTY\n");
		}
		if (err_rev == ERR_OVERRUN) {
			printf("ERR_OVERRUN\n");
		}
		if (err_rev == ERR_ARBITR) {
			printf("ERR_ARBITR\n");
		}
		if (err_rev == ERR_NOTAVAIL) {
			printf("ERR_NOTAVAIL\n");
		}
	}
	/*
	 ERR_OK - OK
	 ERR_SPEED - This device does not work in the active speed mode
	 ERR_DISABLED - Device is disabled
	 ERR_BUSY - The slave device is busy, it does not respond by an acknowledge (only in master mode and when interrupt service is disabled)
	 ERR_BUSOFF - Clock timeout elapsed or device cannot receive data
	 ERR_RXEMPTY - The receive buffer didn't contain the requested number of data. Only available data (or no data) has been returned (slave mode only).
	 ERR_OVERRUN - Overrun error was detected from last character or block receiving (slave mode only)
	 ERR_ARBITR - Arbitration lost (only when interrupt service is disabled and in master mode)
	 ERR_NOTAVAIL - Method is not available in current mode - see the comment in the generated code.
	 */
}
int Linien_zeahlen(uint8_t array_laenge, uint8_t MinBreite, uint8_t MaxBreite) {
	uint8_t Linienbreite = 0;
	uint8_t Linienerkannt = 0;
	for (int i = 0; i < array_laenge + 1; i++) {
		if (i == array_laenge && pre_null == 1) {
			anzahl_linien++;
			Pixel_Null[anzahl_linien] = anzahl_null_bar;
			anzahl_null_bar = 0;
		}
		if (Pixel_Auswertung[i] == 1 && pre_null == 1) {
			anzahl_linien++;
			Pixel_Null[anzahl_linien] = anzahl_null_bar;
			anzahl_null_bar = 0;
			pre_null = 0;
		}
		if (Pixel_Auswertung[i] == 0) {
			anzahl_null_bar++;

			pre_null = 1;
		}
	}
	null_diff = 0;
	for (int i = 2; i < anzahl_linien; i++) {
		if (Pixel_Null[i] >= MinBreite && Pixel_Null[i] <= MaxBreite) {
			Linienerkannt++;
		}
		null_diff = null_diff + Pixel_Null[i];
	}
	if (Linienerkannt == 4) {
		VierLinien++;
		if (VierLinien >= 3) {
			LED4_On();
			LED3_Off();
			VierLinien = 0;
		}
	}
	if (Linienerkannt == 3) {
		DreiLinien++;
		if (DreiLinien >= 3) {
			LED4_Off();
			LED3_On();
			DreiLinien = 0;
		}
	}
	if (Linienerkannt > 4 || Linienerkannt < 3) {
		LED4_Off();
		LED3_Off();
	}
	if (Serialline == TRUE) {
		printf("Anzahl Linien: ");
		printf("%d\n", Linienerkannt);
	}
	anzahl_null_bar = 0;
	anzahl_linien = 0;
	Linienerkannt = 0;
}
int Auswertung(uint8_t array_laenge, uint16_t Black_Threshold) {
	for (int i = 0; i < array_laenge; i++) {
		if (Pixel[i] > Black_Threshold) {
			Pixel_Auswertung[i] = 1;
			anzahl_eins++;
			erkennung_rechts == 1;
			if (erkennung_links == 1) {
				Linke_Linie = anzahl_null;
				anzahl_null = 0;
				erkennung_links = 0;
			}
		} else {
			Pixel_Auswertung[i] = 0;
			anzahl_null++;
			if (erkennung_rechts == 1 && i == array_laenge - 1) {
				Rechte_Linie = anzahl_null;
				erkennung_rechts = 0;
			}
		}
		if (Serialline == TRUE) {
			printf("%d", Pixel_Auswertung[i]);

		}
	}
	printf("\n");
	Rechte_Linie = Rechte_Linie - null_diff;
	erkennung_links = 1;
	erkennung_rechts = 1;
	Linien_zeahlen(array_laenge, 4, 7);
}
uint16_t Threshold_cal() {
	uint16_t Threshold = 0;
	uint16_t PixelVal = 0;
	uint16_t MinVal = 65535;
	uint16_t MaxVal = 0;
	for (int i = 1; i < 126; i++) {
		PixelVal = Pixel[i];
		//printf("%d\n",PixelVal);
		if (MinVal < PixelVal) {
			MinVal = MinVal;
		} else {
			MinVal = PixelVal;
		}
		if (MaxVal > PixelVal) {
			MaxVal = MaxVal;
		} else {
			MaxVal = PixelVal;
		}

	}
	Threshold = (MaxVal - MinVal) / 2;
	//printf("Max: %d\n", MaxVal);
	//printf("Min: %d\n", MinVal);
	return Threshold;
}
uint16_t Lenkung(uint16_t Winkel_prev, uint16_t Winkel) {
	uint16_t Winkeldiff;
	return Winkeldiff = Winkel - Winkel_prev;
}
void LineKamera() {
	uint8_t GetVal;
	bool Start;
	uint16_t ADC_Wert = 0;
	uint16_t Threshold = 0;
	for (int i = 0; i < 128 * 2; i++) {
		if (i == 0)              //Kommunikation gestartet?
				{
			//SI_On();                 //Einschalten von SI PTD7
			SI_PutVal(TRUE);
			Start = TRUE;
		}
		CLK_Neg();                 //CLK Signal auf High  PTE1
		WAIT1_Waitus(1);
		if (i == 0 || Start == TRUE)             //Kommunikation gestartet?
		{
			//SI_Off();     //SI auf LOW vor der n�chsten Fallendenflanke von CLK
			SI_PutVal(FALSE);
			Start = FALSE;
		}
		pulse++;                   //1 = LOW pulse 2 = High Pulse = Eine Periode

		if (pulse == 2)                     //Wenn CLK auf High lese ADC aus
				{
			(void) Potis_MeasureChan(TRUE, 2);
			(void) Potis_GetChanValue16(2, &ADC_Wert);
			Pixel[array_pos] = ADC_Wert; //Schreibe Aktuellen Analogwert in das Array
			array_pos++;                    //Position des Arrays inkrementieren
			pulse = 0;                      //Periode Vorbei
			if (array_pos == 128)             //Alle 129 Werte ausgelesen
					{
				array_pos = 0;                 //Array position wieder auf null
				startup = 0;
			}
		}
	}
	Threshold = Threshold_cal();
	Auswertung(128, Threshold);
}
void Break(uint16_t Rev_Speed, uint16_t Block_Time) {

			MotorRechts_SetRatio16(0);
			MotorLinks_SetRatio16(0);
			MotorRechts_Rev_SetRatio16(Rev_Speed);
			MotorLinks_Rev_SetRatio16(Rev_Speed);
		    WAIT1_Waitms(Block_Time);
		    MotorRechts_Rev_SetRatio16(0);
		    MotorLinks_Rev_SetRatio16(0);
}
int main(void) {
	CLK_Init();
	PE_low_level_init();
	//RechtsClock_Enable();

	for (;;) {

		(void) Potis_MeasureChan(TRUE, 0);
		(void) Potis_GetChanValue16(0, &value);
		(void) Potis_MeasureChan(TRUE, 1);
		(void) Potis_GetChanValue16(1, &value2);
		Rec();
		//LineKamera();
		if (Serial == TRUE) {
			printf("IST_Rechts = %0.2f,IST_Links = %0.2f Soll_Speed = %d m/s\n",
					velocity_Links, velocity_Rechts, value);
		}
		//printf("Soll_Speed = %d \n",value);
		counter(FALSE);
		if (value > StartMotor) {
			value = StartMotor;
		}
		uint16_t Maxspeed = StartMotor - value;
		uint16_t Slow = StartMotor - Maxspeed * 1 / 2;
		Maxspeed = StartMotor - Maxspeed;
		uint16_t Speed = map(Angle, 0, 45, Maxspeed, Slow);
		uint16_t LR_diff = map(Angle, 0, 45, 0, 8000);
		uint16_t LR_diff_innen = map(Angle, 0, 45, 0, 4000);
		Speed = map(Speed, 0, 65535, 65535, 0);
		uint16_t Break_Time_Speed = map(Speed, 0, 65535, 50, 25);


		if (Angle > 15&&velocity_Rechts_avg > 1.5)
		{
			Break(Speed + 10000,300);
		}

			if (data[3] == 'S') {
				MotorRechts_SetRatio16(Speed);
				MotorLinks_SetRatio16(Speed);
			}
			if (data[3] == 'C') {
				if (data[0] == 'L') {
					MotorRechts_SetRatio16(Speed - LR_diff_innen);
					MotorLinks_SetRatio16(Speed + LR_diff);
				}
				if (data[0] == 'R') {
					MotorRechts_SetRatio16(Speed - LR_diff_innen);
					MotorLinks_SetRatio16(Speed + LR_diff);
				}
			}
		MotorLinks_Enable();
		MotorRechts_Enable();

		if (TasterA_GetVal() == 1 && SW1_GetVal() == 0) {
			Start = TRUE;
		}
		if (TasterB_GetVal() == 1) {
			Start = FALSE;
		}
		if (data[1] == 'D' && Start == TRUE && Start == TRUE) {
			LED1_On();
			EN_On();
		}
		if (data[1] == 'X' || Start == FALSE) {
			//Break_Active = 1;
			Break(Speed,1000);
			LED1_Off();
			EN_Off();
		}

		if (data[0] == 'L') {
			int diff2 = map(SerVal, 0, 819, Mitte, Links); //max 62258 min 58981; 819 sind 45° , mitte 60619
			Servo_SetRatio16(diff2);
			Servo_Enable();
			//Winkel_prev_L = diff2;
		}
		if (data[0] == 'R') {
			int diff = map(SerVal, 0, 819, Mitte, Rechts);
			Servo_SetRatio16(diff);
			Servo_Enable();
			//Winkel_prev_R = diff;
		}
		if (data[0] == 'S') {
			Servo_SetRatio16(Mitte); //3276 //62306 1639
		}
		if (SW1_GetVal() == 1 && SW2_GetVal() == 1 && SW3_GetVal() == 1
				&& SW4_GetVal() == 1) {
			while (TasterA_GetVal() == 1) {
				LED2_On();
				(void) Potis_MeasureChan(TRUE, 1);
				(void) Potis_GetChanValue16(1, &value2);
				uint16_t SerVal;
				SerVal = map(value2, 0, 65535, 62258, 59800);
				Servo_SetRatio16(SerVal);
				Mitte = SerVal;
				Links = Mitte + 819;
				Rechts = Mitte - 819;
			}
			LED2_Off();
		}
		LineKamera();
		Programmcounter++;
		if (Programmcounter >= 150) {
			if (data[0] == '0' && data[1] == '0' && data[2] == '0'
					&& data[3] == '0') {
				data[0] = 'X';
				data[1] = 'X';
				data[2] = 'X';
				data[3] = 'X';
				//Break_Active = 1;
				Break(Speed,1000);
				LED1_Off();
				EN_Off();
			}
			Programmcounter = 0;
		}
		data[0] = '0';
		data[1] = '0';
		data[2] = '0';
		data[3] = '0';
		/*RechtsClock_GetTimeMS(&Programmtime);
		 printf("Zeit = %d ms \n",Programmtime);
		 RechtsClock_Reset();
		 RechtsClock_Enable();*/
	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
}
