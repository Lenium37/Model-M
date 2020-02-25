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
#include "Rev_Rechts.h"
#include "LEDpin8.h"
#include "BitIoLdd9.h"
#include "Rev_Links.h"
#include "LEDpin9.h"
#include "BitIoLdd10.h"
#include "CI2C1.h"
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
#include "AS1.h"
#include "ASerialLdd1.h"
#include "CsIO1.h"
#include "IO1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include <stdlib.h>
/* User includes (#include below this line is not maintained by Processor Expert) */
#define TX_CHAR     1    /* Flag for OnTxChar event */
#define BUFFER_FREE 2    /* Flag for OnFreeTxBuf event */
#define TX_EMPTY    4    /* Flag for OnTxEmpty event */
#define SLAVE  0x09
byte flags;
byte err;
byte send_data[5];
word ret;
word Sent;
uint8_t Hundred = 0;
uint8_t Ten = 0;
uint8_t Zero = 0;

uint16_t value;
uint16_t value2;
uint16_t Pixel[128];
uint8_t prev_angle = 0;

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
bool flags_send;

byte err_rev;
byte data[5];

uint16_t Angle = 0;

//Allgemeine Variablen

uint8_t array_pos = 0;
uint8_t startup = 1;
int SerVal = 0;

//PID
uint32_t P_VAL_rechts = 0;
uint32_t P_VAL_links = 0;
//Routine Linien_zaehlen
uint8_t anzahl_linien = 0;
uint8_t pre_null = 0;
uint8_t anzahl_null_bar = 0;
uint16_t Pixel_Null[7];

//Lenkung
uint16_t Mitte = 60750;
uint16_t Links = 61650;
uint16_t Rechts = 59860;
uint16_t Angle_reg = 60750;
uint32_t P_VAL_Lenkung = 60750;
uint16_t servo_value_regulated = 60750;

float velocity_Rechts_avg;
float velocity_links_avg;

uint16_t DreiLinien = 0;
uint16_t VierLinien = 0;
bool Start = FALSE;
bool Serial = FALSE;
bool Serialline = TRUE;
bool Break_Active = FALSE;
bool Regler_Active = TRUE;
bool first_pulse = FALSE;
bool CLK_OFF = FALSE;
bool CLK_ON = FALSE;
bool flag_buffer_empty = FALSE;

uint16_t Programmcounter = 0;
uint16_t Pulse_counter = 0;
uint32_t Speed_regulated_rechts = 0;
uint32_t Speed_regulated_links = 0;
uint16_t Break_intens = 0;
uint16_t Start_point = 10000;
uint16_t linescan_counter = 0;
bool prev_S = FALSE;
float Speed_Ms = 0;

uint8_t kp_Teiler = 20;
struct {
	char Direction;
	char Hundert;
	char Zener;
	char Einer;

	char straight_curve;
} message;
struct {
	byte Speed_left;
	byte Speed_Right;
	byte Speed_right_ovf;
	byte dummy;
	char Terminator;
} send_message;
void cal_start_point() {
	//printf("Startpoint %d\n", Start_point);
	//printf("P_VAL %d\n", P_VAL);
	if (first_pulse == TRUE) {
		Start_point = P_VAL_rechts + 3000;
		//	if(Start_point > 16000)
		//{
		//Start_point = 16000;
		//}
		first_pulse = FALSE;
	}
	if (Start_point < 11000) {
		Start_point = 11000;
	}

}
double map(double x, double in_min, double in_max, double out_min,
		double out_max) {
	double Result = (x - in_min) * (out_max - out_min) / (in_max - in_min)
			+ out_min;
	if (out_max > out_min) {
		if (Result > out_max)
			return out_max;
	} else {
		if (Result < out_max)
			return out_max;
	}
	return Result;
}
long map_long(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t Regler_P_rechts(float Soll, float Ist, int Kp) {
	if (Regler_Active == TRUE) {
		//printf("Soll: %f\n", Soll);
		//printf("Ist: %f\n", Ist);
		//printf("P_VAL start: %d\n", P_VAL);

		float Speed_diff = Soll - Ist;
		if (Speed_diff < 0.5) {
			//Kp = 1000;
		}
		if (P_VAL_rechts < Start_point) {
			P_VAL_rechts = Start_point;
		}
		if (P_VAL_rechts > 65300) {
			P_VAL_rechts = 65300;
		}
		Kp = Kp / kp_Teiler;
		P_VAL_rechts += Speed_diff * Kp;
		//printf("P_VAL: %d\n",P_VAL);
		//printf("P_VAL: %d\n",Kp);
		//printf("P_VAL after calculation: %d\n", P_VAL);
		return P_VAL_rechts;
	}
}
uint16_t Regler_P_links(float Soll, float Ist, int Kp) {
	if (Regler_Active == TRUE) {
		//printf("Soll: %f\n", Soll);
		//printf("Ist: %f\n", Ist);
		//printf("P_VAL start: %d\n", P_VAL);

		float Speed_diff = Soll - Ist;
		if (Speed_diff < 0.5) {
			//Kp = 1000;
		}
		if (P_VAL_links < Start_point) {
			P_VAL_links = Start_point;
		}
		if (P_VAL_links > 65300) {
			P_VAL_links = 65300;
		}
		Kp = Kp / kp_Teiler;
		P_VAL_links += Speed_diff * Kp;
		//printf("P_VAL: %d\n",P_VAL);
		//printf("P_VAL: %d\n",Kp);
		//printf("P_VAL after calculation: %d\n", P_VAL);
		return P_VAL_links;
	}
}
void counter() {
	Pulse_counter++;
	if (Pulse_counter > 500) {

		velocity_Rechts_avg = velocity_Rechts_avg / 2;
		velocity_links_avg = velocity_links_avg / 2;
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
	default:
		Wert = 0;
		break;
	}
	return Wert;
}
int Cal_Angle(uint8_t Hundred, uint8_t Ten, uint8_t Zero) {

	return Hundred * 100 + Ten * 10 + Zero;
}
void send() {
	/* Set slave address */
	int count = 0;
	(void) CI2C1_SelectSlave(SLAVE);

	/* Send bytes in master mode */

	err = CI2C1_SendBlock(&send_data, 5, &ret);
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
	while (!flags_send) {
		count++;
		if (count == 3000) {
			flags_send = 1;
			count = 0;
		}
	}
}
void Rec() {
	int count = 0;

	Send_OK_Off();
	//printf("receiving\n");
	/* Receive bytes in slave mode (read the input buffer) */

	(void) CI2C1_SelectSlave(SLAVE);

	/* Clear "Complete" flag */
	flags = 0;

	/* Receive bytes in master mode */
	err = CI2C1_RecvBlock(&data, 5, &ret);

	/* Wait for communication complete */
	while (!flags) {
		count++;
		if (count == 3000) {
			flags = 1;
			count = 0;
		}
	}

	//err_rev = 100;

	prev_angle = Angle;
	if (data[0] != 'X' || data[1] != 'X' || data[2] != 'X' || data[3] != 'X'
			|| data[4] != 'X') {
		Angle = Cal_Angle(Hundred, Ten, Zero);
	}
	if (data[0] == 'X' || data[1] == 'X' || data[2] == 'X' || data[3] == 'X'
			|| data[4] == 'X') {
		Angle = prev_angle;
	}
	//819 //3277 //max 62258 min 58981; 819 sind 45° , mitte 60619
	//printf("  Data: %c%c%c%c%c\n",data[0],data[1],data[2],data[3],data[4]);
	//word RX = CI2C1_GetCharsInRxBuf();
	//printf("Angle: %d, %d\n", Angle,SerVal);
	//if(data[1] == 'L'||data[1]=='R'||data[1]=='S'||data[2] == 'L'||data[2]=='R'||data[2]=='S')
	//{
	//I2C1_ClearRxBuf();
	//}
	send();
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

	//send();
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
		//printf("Anzahl Linien: ");
		//printf("%d\n", Linienerkannt);
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
			//printf("%d", Pixel_Auswertung[i]);

		}
	}
	//printf("\n");
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
	if (CLK_ON == FALSE) {
		if (linescan_counter == 0 && Start_line == TRUE) //Kommunikation gestartet?
		{
			//SI_On();                 //Einschalten von SI PTD7

			SI_PutVal(TRUE);
			WAIT1_Waitus(5);
			Start_line = TRUE;
		}
		CLK_On();      //CLK_Neg();                 //CLK Signal auf High  PTE1
		if (Start_line == TRUE)             //Kommunikation gestartet?
		{
			WAIT1_Waitus(5);
			//SI_Off();     //SI auf LOW vor der n�chsten Fallendenflanke von CLK
			SI_PutVal(FALSE);
			Start_line = FALSE;
		}                   //1 = LOW pulse 2 = High Pulse = Eine Periode
		CLK_ON = TRUE;
	}
	if (linescan_counter == exposure_time_on) {
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
	if (linescan_counter == exposure_time_off + exposure_time_on) {
		linescan_counter = 0;
		CLK_ON = FALSE;

	}
}
void Break(uint16_t Rev_Speed, uint16_t Block_Time) {
	if (Break_Active == TRUE) {
		Regler_Active = FALSE;
		FC321_Enable();
		FC321_GetTimeMS(&Break_time);
		Rev_Rechts_On();
		Rev_Links_On();
		MotorRechts_SetRatio16(Rev_Speed);
		MotorLinks_SetRatio16(Rev_Speed);
		if (Break_time > Block_Time) {
			P_VAL_rechts = Start_point;
			P_VAL_links = Start_point;
			Rev_Links_Off();
			Rev_Rechts_Off();
			MotorRechts_SetRatio16(Start_point);
			MotorLinks_SetRatio16(Start_point);
			FC321_Disable();
			FC321_Reset();
			Break_period = 0;
			Break_intens = 0;
			if (data[1] == 'X' || Start == FALSE) {
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
	send_message.Terminator = '&';
	send_message.dummy = 10;
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	CLK_Init();
	PE_low_level_init();
	FC321_Disable();
	FC321_Reset();
	Rev_Rechts_Off();
	Rev_Links_Off();
	float S_multi_rechts = 1.0;
	float S_multi_links = 1.0;
	uint16_t Kp_drive_rechts = 175;
	uint16_t Kp_drive_links = 175;
	uint16_t add_rechts_speed = 1000;
	//RechtsClock_Enable();
	servo_value_regulated = Mitte;
	for (;;) {
		counter();
		cal_start_point();

		if (Programmcounter % 15 == 0)
			; // nur jedes 15. Mal receiven
		//Rec();

		//uint16_t AVG_Rechts_int = map(velocity_Rechts_avg,0.0,2.0,0,65535);
		float Speed_Ms_links = Speed_Ms * S_multi_links;
		float Speed_Ms_rechts = Speed_Ms * S_multi_rechts;
		Speed_regulated_rechts = Regler_P_rechts(Speed_Ms_rechts,
				velocity_Rechts_avg, Kp_drive_rechts);
		Speed_regulated_links = Regler_P_links(Speed_Ms_links,
				velocity_Rechts_avg, Kp_drive_links);
		//
		//printf("Speed_rechts: %0.2f Soll_rechts: %0.2f\n",velocity_Rechts_avg,Speed_Ms_rechts);
		//printf("Speed_rechts: %0.2f\n",velocity_Rechts_avg);
		//printf("\n");
		Speed_regulated_rechts = map(Speed_regulated_rechts, 0, 65300, 65300,0);
		Speed_regulated_links = map(Speed_regulated_links, 0, 65300, 65300, 0);

		send_message.Speed_Right = velocity_Rechts_avg * 100;
		send_message.Speed_left = velocity_Rechts_avg * 100;
		if(velocity_Rechts_avg * 100 > 255)
		{
			send_message.Speed_right_ovf = 1;
		}else
		{
			send_message.Speed_right_ovf = 0;
		}
		if(flag_buffer_empty == TRUE)
		{
			AS1_SendBlock((byte*) &send_message, sizeof(send_message), &Sent);
			//printf("data %d %d\n", send_message.Speed_Right,send_message.Speed_left);
			flag_buffer_empty = FALSE;
		}

		/*uint16_t LR_diff = 0;
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
		 }*/

		//MotorRechts_SetRatio16(value);
		//MotorLinks_SetRatio16(value);
		Break(Break_intens, Break_period);

		Hundred = char_int(message.Hundert);
		Ten = char_int(message.Zener);
		Zero = char_int(message.Einer);

		Angle = Cal_Angle(Hundred, Ten, Zero);
		SerVal = map_long(Angle, 0, 450, 0, 819);

		//printf("data %c%c%c%c%c , Angle: %d\n",message.Direction,message.Hundert,message.Zener,message.Einer,message.straight_curve,Angle);
		if (message.straight_curve == 'S' && Break_Active == FALSE) {
			//Kp_drive = 175;
			S_multi_links = 1.3;
			S_multi_rechts = 1.3;
			Kp_drive_rechts = 300;
			Kp_drive_links = 300;
			prev_S = TRUE;
			Break_intens = 0;
			Rev_Links_Off();
			Rev_Rechts_Off();
			MotorRechts_SetRatio16(Speed_regulated_rechts);
			MotorLinks_SetRatio16(Speed_regulated_links);
		} else {
			if (message.Direction == 'S' && Break_Active == FALSE) {
				Kp_drive_rechts = 250;
				Kp_drive_links = 250;
				MotorRechts_SetRatio16(Speed_regulated_rechts);
				MotorLinks_SetRatio16(Speed_regulated_links);
			}
			if (message.straight_curve == 'C' && Break_Active == FALSE) {
				//S_multi_rechts = 1.0;
				//S_multi_links = 1.0;
				Kp_drive_rechts = 250;
				Kp_drive_links = 250;
				if (prev_S == TRUE && velocity_Rechts_avg > 0.5) {
					//Kp_drive = 0;
					LED1_Off();
					Break_period = 100;
					Break_intens = 30000;
					Break_Active = TRUE;
					prev_S = FALSE;
				}
				if (message.Direction == 'L' && Break_Active == FALSE) {
					S_multi_rechts = 1.1;
					S_multi_links = 0;
					MotorRechts_SetRatio16(Speed_regulated_rechts);
					MotorLinks_SetRatio16(Speed_regulated_links);
				}
				if (message.Direction == 'R' && Break_Active == FALSE) {
					S_multi_rechts = 0;
					S_multi_links = 1.1;
					MotorRechts_SetRatio16(Speed_regulated_rechts);
					MotorLinks_SetRatio16(Speed_regulated_links);
				}
			}
		}

		if (TasterA_GetVal() == 1 && SW1_GetVal() == 0) {
			(void) Potis_MeasureChan(TRUE, 0);
			(void) Potis_GetChanValue16(0, &value);
			uint16_t Maxspeed = value;
			uint16_t Slow = Maxspeed * 3 / 4;
			uint16_t Speed = map_long(Angle, 0, 450, Maxspeed, Slow);
			Speed_Ms = (float) map(Speed, 0, 65535, 0.0, 3.5);
			Start = TRUE;
		}
		if (TasterB_GetVal() == 1) {
			Start = FALSE;
		}
		if (Start == TRUE) {
			if (message.Direction == 'L' || message.Direction == 'R'
					|| message.Direction == 'S') {
				Regler_Active = TRUE;
				LED1_On();
				EN_On();
			}
		}
		if (message.Einer == 'X' || Start == FALSE) {

			Kp_drive_links = 0;
			Kp_drive_rechts = 0;
			Regler_Active = FALSE;
			Break_Active = TRUE;
			Break_period = 1000;
			Break_intens = 20000;

			LED1_Off();
			//EN_Off();
		}

		uint16_t servo_value = Mitte;

		if (message.Direction == 'L') {
			servo_value = map_long(SerVal, 0, 819, Mitte, Links); //max 62258 min 58981; 819 sind 45° , mitte 60619
			//Winkel_prev_L = diff2;
		} else if (message.Direction == 'R') {
			servo_value = map_long(SerVal, 0, 819, Mitte, Rechts);
			//Winkel_prev_R = diff;
		}

		if (abs(servo_value - servo_value_regulated) > 50) {
			//printf("servo_value too large, taking only a step\n");
			if (servo_value > servo_value_regulated)
				servo_value_regulated += 50;
			else
				servo_value_regulated -= 50;
		} else {
			//printf("jumping exactly to servo_value\n");
			servo_value_regulated = servo_value;
		}

		Servo_SetRatio16(servo_value_regulated);
		Servo_Enable();

		if (message.Direction == 'S') {
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
			if (data[0] == '0' && data[1] == '0' && data[2] == '0'
					&& data[3] == '0' && data[4] == '0') {
				data[0] = 'X';
				data[1] = 'X';
				data[2] = 'X';
				data[3] = 'X';
				data[4] = 'X';
				//Break_Active = 1;
				Break_Active = TRUE;
				Break_period = 1000;
				Break_intens = 15000;
				Regler_Active = FALSE;
				Speed_regulated_links = 1000;
				Speed_regulated_rechts = 1000;
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
