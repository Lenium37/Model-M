/* Including needed modules to compile this module/procedure */
#include <stdio.h>
#include "Cpu.h"
#include "Events.h"
#include "Servo.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "PwmLdd2.h"
#include "TU2.h"
#include "MotorLinks.h"
#include "PwmLdd3.h"
#include "LinksINT.h"
#include "ExtIntLdd2.h"
#include "TU3.h"
#include "FC321.h"
#include "RealTimeLdd1.h"
#include "TU4.h"
#include "SI.h"
#include "BitIoLdd8.h"
#include "Send_OK.h"
#include "LEDpin7.h"
#include "BitIoLdd7.h"
#include "RechtsINT.h"
#include "ExtIntLdd1.h"
#include "I2C1.h"
#include "MotorRechts.h"
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
#include <stdlib.h>
/* User includes (#include below this line is not maintained by Processor Expert) */
uint16_t value;
uint16_t value2;
uint16_t Pixel[128];
uint8_t prev_angle = 0;
uint16_t Programmtime;
int Curve = 0;
word Break_time = 0;
uint16_t Break_period = 0;
bool Start_line;
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
byte data[5];
word ret_rev;
uint16_t Angle = 0;
#define SLAVE 0x09

//Allgemeine Variablen
uint8_t pulse = 0;
uint8_t array_pos = 0;
uint8_t startup = 1;
int SerVal = 0;
extern uint8_t avgcounter;
uint16_t StartMotor = 50000;
//PID
uint32_t P_VAL = 0;

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
uint16_t Start_Counter = 16000;
uint16_t Mitte = 60750;
uint16_t Links = 61650;
uint16_t Rechts = 59860;
uint16_t Angle_reg = 60750;
uint32_t P_VAL_Lenkung = 60750;
uint16_t servo_value_regulated = 60750;
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
bool Serialline = TRUE;
bool Break_Curve = FALSE;
bool Break_Active = FALSE;
bool Start_up = TRUE;
bool DRIVE = TRUE;
bool Regler_Active = TRUE;
bool first_pulse = FALSE;
bool CLK_OFF = FALSE;
bool CLK_ON = FALSE;
int Break_Timer = 0;
uint16_t Winkel_prev_L = 0;
uint16_t Winkel_prev_R = 0;
uint16_t Lenkung_Regler = 0;
uint16_t Programmcounter = 0;
uint16_t Pulse_counter = 0;
uint32_t Speed_regulated = 0;
uint16_t Break_intens = 0;
uint16_t Start_point = 10000;
uint16_t linescan_counter = 0;
void cal_start_point()
{
	printf("Startpoint %d\n", Start_point);
	printf("P_VAL %d\n", P_VAL);
	if(first_pulse == TRUE)
	{
		Start_point = P_VAL+3000;
	//	if(Start_point > 16000)
		//{
			//Start_point = 16000;
		//}
		first_pulse = FALSE;
	}
	if(Start_point < 11000)
	{
		Start_point = 11000;
	}

}
double map(double x, double in_min, double in_max, double out_min, double out_max) {
	double Result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	if(out_max > out_min) {
		if(Result > out_max)
			return out_max;
	} else {
		if(Result < out_max)
			return out_max;
	}
	return Result;
}
long map_long(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t Regler_P(float Soll, float Ist, int Kp) {
if(Regler_Active == TRUE)
{
	//printf("Soll: %f\n", Soll);
	//printf("Ist: %f\n", Ist);
	//printf("P_VAL start: %d\n", P_VAL);


	float Speed_diff = Soll - Ist;
	if(Speed_diff < 0)
	{
		Kp = 1000;
	}
	if(P_VAL < Start_point)
		{
			P_VAL = Start_point;
		}
	if(P_VAL>65300)
	{
		P_VAL = 65300;
	}
	Kp = Kp/4;
	P_VAL += Speed_diff * Kp;
	//printf("P_VAL: %d\n",P_VAL);
	//printf("P_VAL: %d\n",Kp);
	//printf("P_VAL after calculation: %d\n", P_VAL);
	return P_VAL;
	}
}
void counter()
{
	Pulse_counter++;
	if(Pulse_counter > 500)
	{
		uint32_t Speed_regulated = 0;
		velocity_Rechts_avg = velocity_Rechts_avg/2;
		//P_VAL = P_VAL/2;
		Pulse_counter = 0;
	}
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
	default :
		Wert = 0;
		break;
	}
	return Wert;
}
int Cal_Angle(uint8_t Hundred, uint8_t Ten, uint8_t Zero) {

	return Hundred * 100 + Ten * 10 + Zero;
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
	//int count = 0;
	/*while (!flags_rev)
	{
		//printf("Angle: %d, %d\n", Angle,SerVal);
		count++;
		if (count > 30000) {
			flags_rev = TRUE;
			count = 0;
		}
		//Send('R',1);
	}*/
	//Send('B',1);
	//while (!errFlags_rev);
	if(flags_rev == TRUE)
	{
	//Send_OK_Off();
	//printf("receiving\n");
	/* Receive bytes in slave mode (read the input buffer) */
	err_rev = I2C1_RecvBlock(&data, 8, &ret_rev);
	//err_rev = 100;
	uint8_t Hundred = char_int(data[1]);
	uint8_t Ten = char_int(data[2]);
	uint8_t Zero = char_int(data[3]);
	prev_angle = Angle;
	if (data[0] != 'X' || data[1] != 'X' || data[2] != 'X' || data[3] != 'X' || data[4] != 'X') {
		Angle = Cal_Angle(Hundred, Ten, Zero);
	}
	if (data[0] == 'X' || data[1] == 'X' || data[2] == 'X' || data[3] == 'X' || data[4] == 'X') {
		Angle = prev_angle;
	}
	SerVal = map_long(Angle, 0, 450, 0, 819); //819 //3277 //max 62258 min 58981; 819 sind 45° , mitte 60619
	//printf("  Data: %c%c%c%c\n",data[0],data[1],data[2],data[3],data[4]);
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
	flags_rev = FALSE; /* Clear flags */
	errFlags_rev = FALSE;
	}

	Send_OK_On();
	//}
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
void LineKamera() {
	uint16_t ADC_Wert = 0;
	uint16_t Threshold = 0;
	uint16_t exposure_time_on = 2;
	uint16_t exposure_time_off = 2;
	if(CLK_ON == FALSE)
	{
		if (linescan_counter == 0 && Start_line == TRUE)              //Kommunikation gestartet?
		{
			//SI_On();                 //Einschalten von SI PTD7

			SI_PutVal(TRUE);
			WAIT1_Waitus(5);
			Start_line = TRUE;
		}
		CLK_On();//CLK_Neg();                 //CLK Signal auf High  PTE1
		if (Start_line == TRUE)             //Kommunikation gestartet?
		{
			WAIT1_Waitus(5);
			//SI_Off();     //SI auf LOW vor der n�chsten Fallendenflanke von CLK
			SI_PutVal(FALSE);
			Start_line = FALSE;
		}                   //1 = LOW pulse 2 = High Pulse = Eine Periode
		CLK_ON = TRUE;
	}
	if(linescan_counter == exposure_time_on)
	{
		CLK_OFF = TRUE;
	}
		if (CLK_OFF == TRUE)                     //Wenn CLK auf High lese ADC aus
			{
			(void) Potis_MeasureChan(TRUE, 2);
			(void) Potis_GetChanValue16(2, &ADC_Wert);
			Pixel[array_pos] = ADC_Wert; //Schreibe Aktuellen Analogwert in das Array
			array_pos++;                    //Position des Arrays inkrementieren
			CLK_Off();
			if (array_pos == 129)             //Alle 129 Werte ausgelesen
				{
				array_pos = 0;                 //Array position wieder auf null
				linescan_counter = 0;
				startup = 0;
				Start_line = TRUE;
				//Threshold = Threshold_cal();
				//Auswertung(128, Threshold);
			}
			CLK_OFF = FALSE;
		}
	//}
	linescan_counter++;
	if(linescan_counter == exposure_time_off+exposure_time_on)
	{
		linescan_counter = 0;
		CLK_ON = FALSE;

	}
}
void Break(uint16_t Rev_Speed, uint16_t Block_Time) {
	if(Break_Active == TRUE)
	{
			Regler_Active = FALSE;
			FC321_Enable();
			FC321_GetTimeMS(&Break_time);
			MotorRechts_SetRatio16(65300);
			MotorLinks_SetRatio16(65300);
			//MotorLinks_RevRatio16(65300);
			if(Break_time>Block_Time)
			{
				//printf("Break: %d \n",Break_time);
				//MotorRechts_Rev_SetRatio16(1);
				//MotorLinks_Rev_SetRatio16(1);
				FC321_Disable();
				FC321_Reset();
				Break_period = 0;
				Break_intens = 0;
				if(data[1] == 'X'||Start == FALSE)
				{
					EN_Off();
				}
				Regler_Active = TRUE;
				Break_Active = FALSE;
			}
	}
}
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

	CLK_Init();
	PE_low_level_init();
	FC321_Disable();
	FC321_Reset();
	uint16_t Kp_drive = 175;
	uint16_t add_rechts_speed = 1000;
	//RechtsClock_Enable();
	servo_value_regulated = Mitte;
	for (;;) {
		counter();
		cal_start_point();
		(void) Potis_MeasureChan(TRUE, 0);
		(void) Potis_GetChanValue16(0, &value);
		(void) Potis_MeasureChan(TRUE, 1);
		(void) Potis_GetChanValue16(1, &value2);
		if(Programmcounter % 15 == 0) // nur jedes 15. Mal receiven
			Rec();
		//LineKamera();
		//printf("Soll_Speed = %d \n",value);
		//counter(FALSE);
		/*if (value > StartMotor) {
			value = StartMotor;
		}*/
		//printf("Speed %d\n",Angle);

		uint16_t Maxspeed = value;
		uint16_t Slow = Maxspeed * 3 / 4;
		uint16_t Speed = map_long(Angle, 0, 450, Maxspeed, Slow);
		//printf("Speed %d\n",Speed);
		float Speed_Ms = (float) map(Speed,0,65535,0.0,2.0);
		//printf("Speed_Ms %f\n",Speed_Ms);
		//uint16_t LR_diff = map_long(Angle, 0, 450, 0, 8000);
		//uint16_t LR_diff_innen = map_long(Angle, 0, 450, 0, 4000);
		//if(LR_diff_innen < 2000)
			//LR_diff_innen = 0;
		//uint16_t Break_Time_Speed = map_long(Speed, 0, 65535, 50, 25);
        uint16_t AVG_Rechts_int = map(velocity_Rechts_avg,0.0,2.0,0,65535);
       // printf("Speed = %f",velocity_Rechts_avg);
		Speed_regulated = Regler_P(Speed_Ms,velocity_Rechts_avg,Kp_drive);
		Speed_regulated = map(Speed_regulated,0,65300,65300,0);
		//printf("velocity_Rechts_avg %f\n",velocity_Rechts_avg);
		//printf("Value = %0.2f\n",velocity_Rechts_avg);

		uint16_t LR_diff = 0;
		if(Angle > 400) {
			LR_diff = Speed_regulated / 5;
			if(Speed_regulated + LR_diff > 65300) {
				LR_diff = Speed_regulated / 8;
				if(Speed_regulated + LR_diff > 65300) {
					LR_diff = Speed_regulated / 10;
					if(Speed_regulated + LR_diff > 65300) {
						LR_diff = 0;
					}
				}
			}
		}

	    //printf("velocity_Rechts_avg: %f\n", velocity_Rechts_avg);
		//printf("speed_regulated: %d\n", Speed_regulated);
		//printf("LR_diff: %d\n", LR_diff);

		Break(Break_intens,Break_period);
		if(data[4] =="S")
			Kp_drive = 400;
		else
			Kp_drive = 175;
			 if(data[0] == 'S'&&Break_Active == FALSE) {
				Kp_drive = 400;
				Break_intens = 0;
				MotorRechts_SetRatio16(Speed_regulated - add_rechts_speed);
				MotorLinks_SetRatio16(Speed_regulated);
			}
			if (/*data[3] == 'C'&&*/Break_Active == FALSE) {
				Kp_drive = 175;
				if (velocity_Rechts_avg > Speed_Ms+1)
						{
						    Kp_drive = 0;
							Break_period = 25;
							Break_intens = 30000;
							//Break_Active = TRUE;
						}
				if (data[0] == 'L'&&Break_Active == FALSE) {

					MotorRechts_SetRatio16(Speed_regulated - LR_diff - add_rechts_speed);
					if(Speed_regulated - LR_diff < 3)
						LR_diff = -5;
					MotorLinks_SetRatio16(Speed_regulated  + LR_diff);
				}
				if(data[0] == 'R'&&Break_Active == FALSE) {

					MotorRechts_SetRatio16(Speed_regulated + LR_diff - add_rechts_speed);
					if(Speed_regulated - LR_diff < 3)
											LR_diff = -5;
					MotorLinks_SetRatio16(Speed_regulated  - LR_diff);
				}
			}

		if (TasterA_GetVal() == 1 && SW1_GetVal() == 0) {
			Start = TRUE;
		}
		if (TasterB_GetVal() == 1) {
			Start = FALSE;
		}
		if (Start == TRUE) {
			if(data[0] == 'L' || data[0] == 'R'|| data[0] == 'S')
			{
			Regler_Active = TRUE;
			LED1_On();
			EN_On();
			}
		}
		if (data[1] == 'X' || Start == FALSE) {

			Kp_drive = 0;
			Regler_Active = FALSE;
			//Break_Active = TRUE;
			Break_period = 1000;
			Break_intens = 40000;
			LED1_Off();
			EN_Off();
		}

		uint16_t servo_value = Mitte;

		if (data[0] == 'L') {
			servo_value = map_long(SerVal, 0, 819, Mitte, Links); //max 62258 min 58981; 819 sind 45° , mitte 60619
			//Winkel_prev_L = diff2;
		}
		else if (data[0] == 'R') {
			servo_value = map_long(SerVal, 0, 819, Mitte, Rechts);
			//Winkel_prev_R = diff;
		}

		if(abs(servo_value - servo_value_regulated) > 50) {
			//printf("servo_value too large, taking only a step\n");
			if(servo_value > servo_value_regulated)
				servo_value_regulated += 50;
			else
				servo_value_regulated -= 50;
		} else {
			//printf("jumping exactly to servo_value\n");
			servo_value_regulated = servo_value;
		}

		Servo_SetRatio16(servo_value_regulated);
		Servo_Enable();

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
				SerVal = map_long(value2, 0, 65535, 62258, 59800);
				Servo_SetRatio16(SerVal);
				Mitte = SerVal;
				Angle_reg = Mitte;
				P_VAL_Lenkung = Mitte;
				Links = Mitte + 819;
				Rechts = Mitte - 819;
			}
			LED2_Off();
		}
		//LineKamera();
		Programmcounter++;
		if (Programmcounter >= 150) {
			if (data[0] == '0' && data[1] == '0' && data[2] == '0' && data[3] == '0' && data[4] == '0') {
				data[0] = 'X';
				data[1] = 'X';
				data[2] = 'X';
				data[3] = 'X';
				data[4] = 'X';
				//Break_Active = 1;
				Break_Active = TRUE;
				Break_period = 1000;
				Break_intens = 60000;
				Regler_Active = FALSE;
				Speed_regulated = 1000;
				LED1_Off();
				EN_Off();
			}
			Programmcounter = 0;
		}
		//data[0] = '0';
		//data[1] = '0';
		//data[2] = '0';
		//data[3] = '0';
		//data[4] = '0';
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
