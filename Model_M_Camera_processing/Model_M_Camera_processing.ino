#include <Pixy2.h>
#include <EEPROM.h>
#include <FastLED.h>

<<<<<<< Updated upstream
<<<<<<< Updated upstream
#define NUM_LEDS 12
=======
#define NUM_LEDS 47
>>>>>>> Stashed changes
=======
#define NUM_LEDS 47
>>>>>>> Stashed changes
#define DATA_PIN 8

CRGB leds[NUM_LEDS];

#define I2C_ADDRESS_OF_SLAVE 8
#define I2C_READY_PIN 3
#define PIN_BT 5
#define PIN_RELAY 6
//#define PIN_LED_1 7
//#define PIN_LED_2 8
//#define PIN_LED_3 9
#define PIN_LIGHT_SENSOR 14
#define PIN_BATTERY_VOLTAGE 15
#define DEBUG_GRAY_VALUES true
#define TRIGGER_PIN_LEFT 21
#define ECHO_PIN_LEFT 20
#define TRIGGER_PIN_RIGHT 3
#define ECHO_PIN_RIGHT 4
<<<<<<< Updated upstream
<<<<<<< Updated upstream
#define ARRAY_EEPROMM_SIZE 13
=======
#define ARRAY_EEPROMM_SIZE 18
>>>>>>> Stashed changes
=======
#define ARRAY_EEPROMM_SIZE 18
>>>>>>> Stashed changes
#define NUMBER_OF_ULTRASONICS_TRIGGERS_FOR_STOP 5
#define DURATION_OF_DODGE 350
#define RGB_THRESHOLD_FOR_SYMBOLS 75

Pixy2 pixy;

uint16_t LEFT_DISTANCE_THRESHOLD = 10;
uint16_t RIGHT_DISTANCE_THRESHOLD = 10;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
uint8_t SOLL_SPEED = 100;
uint8_t SLOW_SPEED = SOLL_SPEED / 2;
uint8_t THRESHOLD_GRAY = 100;
uint8_t OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER = 23;
uint8_t OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER = 29;
uint8_t OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER = 35;
uint8_t MIN_TRACK_WIDTH = OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER * 1.7;
uint8_t THRESHOLD_DIFFERENCE = 40;
=======
=======
>>>>>>> Stashed changes
uint16_t SOLL_SPEED = 100;
uint16_t SLOW_SPEED = SOLL_SPEED / 2;
uint16_t THRESHOLD_GRAY = 100;
uint16_t OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER = 23;
uint16_t OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER = 29;
uint16_t OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER = 35;
uint16_t MIN_TRACK_WIDTH = OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER * 1.7;
uint16_t THRESHOLD_DIFFERENCE = 40;

int MEDIAN_FILTER_global = 0;
float NOISE_FILTER_global = 0;
int THERSHOLD_PREV_global = 0;
float LENK_FILTER_global = 0;

<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
int data_eepromm[ARRAY_EEPROMM_SIZE];

bool debug_active = false;
bool current_lamp_status = 0;
uint16_t pin_counter = 0;
uint8_t black_or_white_at_25[63];
uint8_t black_or_white_at_75[63];
uint8_t black_or_white_at_120[63];
uint8_t black_or_white_at_165[63];
int Led_Brightness = 0;
uint8_t rgb_25[63];
uint8_t rgb_75[63];
uint8_t rgb_120[63];
uint8_t rgb_165[63];
uint8_t differences_75[63];
uint8_t differences_120[63];
uint8_t differences_165[63];
bool currently_in_curve = false;
String current_angle_string;
float voltage_of_battery = 10;
int battery_low_counter = 0;
int index_of_left_line = 0;
int index_of_right_line = 62;
uint8_t indexes_of_lines[64];
int last_steer_angle = 0;
double distanceCmRight = 0;
double distanceCmLeft = 0;
volatile long durationMicroSec_LEFT = 0;
volatile long durationMicroSec_RIGHT = 0;
volatile long durationMicroSec_LEFT_prev;
volatile long durationMicroSec_RIGHT_prev;
uint8_t LEFT_counter = 0;
uint8_t RIGHT_counter = 0;
uint8_t UL_COUNTER = 0;
uint8_t BOOL_BIN = 0;
bool currently_seeing_both_lines = false;
bool currently_seeing_only_left_line = false;
bool currently_seeing_only_right_line = false;
volatile bool left_active = false;
bool start_stream = true;
unsigned long begin_of_steering_straight = 0;
bool maybe_on_straight = false;
bool flag_serial4 = true;
uint8_t Start_Stop = 1;
float Speed_left = 0.0;
float Speed_right = 0.0;
uint8_t counter_ultrasonics_triggered = 0;
uint8_t counter_ultrasonics_triggered_left = 0;
uint8_t counter_ultrasonics_triggered_right = 0;
int number_of_lines = 0;
int track_center = 30;
bool dodge_to_the_left = false;
bool dodge_to_the_right = false;
unsigned long timer_of_dodge_to_the_left = 0;
unsigned long timer_of_dodge_to_the_right = 0;
int current_line = 0;
byte Serial_monitor_stream = 1;
String mainString;
String add_debug_string;
uint8_t string_counter = 0;
uint8_t string_max_add = 5;
int last_finish_line = 0;
uint8_t mode = 0;  // 0 = timed race, 1 = slow/fast zone, 2 = obstacle avoidance, 3 = emergency brake, 4 = eight
unsigned long last_symbol_time = 0;
String last_symbol = "nothing";
bool triggert_right = true;
bool triggert_left = true;
void debug(String s, String s1) {

  if (!digitalRead(PIN_BT) == HIGH) {
    if (Serial_monitor_stream == 1) {
      Serial1.print(s);
      //delay(2);
    }
    string_counter = 0;
    if (Serial_monitor_stream == 2 && s1 != "") {
      //Serial.println(s1);
      //Serial1.print(s1);
      //Serial.println("Stop");
      //delay(20);
    }
    string_counter++;
  }
  // if changing Serial number don't forget to change it in setup() as well!
}
void KL25z_data(String s) {
  //Serial.println("Start_Stop: " + String(Start_Stop));
<<<<<<< Updated upstream
<<<<<<< Updated upstream
  //Serial.println(s);
=======
  // Serial.println(s);
>>>>>>> Stashed changes
=======
  // Serial.println(s);
>>>>>>> Stashed changes
  if (Start_Stop == 1) {
    Serial4.print(s + "A");
    flag_serial4 = false;
  }
  if (Start_Stop == 2) {
    Serial4.print("XXXXXX");
    Serial4.print("XXXXXX");
    Start_Stop = 3;
  }
  // if changing Serial number don't forget to change it in setup() as well!
}
void Serial_KL25z_receive() {
  if (Serial4.available() > 0) {
    // Lies das eingehende Byte:
    byte buffer[4];
    Serial4.readBytesUntil('&', buffer, 4);
    if (buffer[2] == 1) {
      Speed_right = buffer[1];
      Speed_right = (Speed_right + 255) / 100;
    } else {
      Speed_right = buffer[1];
      Speed_right = Speed_right / 100;
    }
  }
}

String prepare_speed_data(int speed) {
  String speed_s = "V000V";
  if (speed < 10) {
    speed_s = "V00" + String(abs(speed)) + "V";
  } else if (speed < 100) {
    speed_s = "V0" + String(abs(speed)) + "V";
  } else if (speed >= 100) {
    speed_s = "V" + String(abs(speed)) + "V";
  }
  debug("speed data for speed " + String(speed) + " is " + speed_s + "\n", "");
  return speed_s;
}
void Serial_receive() {
  if (!digitalRead(PIN_BT)) {
    if (Serial1.available() > 1) {
      // Lies das eingehende Byte:
      String incomingData = Serial1.readStringUntil('&');
      incomingData.trim();

      if (incomingData == "#3!") {
        Serial_monitor_stream = 1;
        Serial.println(Serial_monitor_stream);
        //set_LED(0, 255, 255);
      } else if (incomingData == "#4!") {
        Serial_monitor_stream = 2;
        Serial.println(Serial_monitor_stream);
        //set_LED(255, 0, 255);
      }
      if (incomingData.length() <= 9 && incomingData.length() >= 4) {
        String mode_s = incomingData.substring(6, 7);
        mode = mode_s.toInt();
        //Serial.println(mode);
        switch (mode) {
          case 0:
            set_LED(255, 255, 255);
            break;
          case 1:
            set_LED(255, 0, 255);
            break;
          case 2:
            set_LED(255, 255, 0);
            break;
          case 3:
            set_LED(0, 0, 255);
            break;
          case 4:
            set_LED(255, 0, 0);
            break;
          case 5:
            set_LED(0, 255, 0);
            break;
          default:
            break;
        }
      }
      if (start_stream == true && incomingData != "Stop" && incomingData != "Start" && incomingData.length() > 15) {
        //Serial.println(incomingData);
        int p1 = incomingData.indexOf("!");
        int p2 = incomingData.indexOf("[", p1);
        int p3 = incomingData.indexOf("$", p2);
        int p4 = incomingData.indexOf("%", p3);
        int p5 = incomingData.indexOf("/", p4);
        int p6 = incomingData.indexOf("(", p5);
        int p7 = incomingData.indexOf(")", p6);
        int p8 = incomingData.indexOf("=", p7);
        int p9 = incomingData.indexOf("?", p8);
        int p10 = incomingData.indexOf("#", p9);
        int p11 = incomingData.indexOf("+", p10);
        int p12 = incomingData.indexOf("*", p11);
        int p13 = incomingData.indexOf("-", p12);
        int p14 = incomingData.indexOf(";", p13);
        int p15 = incomingData.indexOf("]", p14);
        int p16 = incomingData.indexOf(":", p15);
        int p17 = incomingData.indexOf("'", p16);
        int p18 = incomingData.indexOf(",", p17);

        String Brightness_data = incomingData.substring(0, p1);
        String OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER_data = incomingData.substring(p1 + 1, p2);
        String OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER_data = incomingData.substring(p2 + 1, p3);
        String THRESHOLD_GRAY_data = incomingData.substring(p3 + 1, p4);
        String MIN_TRACK_WIDTH_data = incomingData.substring(p4 + 1, p5);
        String Red_data = incomingData.substring(p5 + 1, p6);
        String Blue_data = incomingData.substring(p6 + 1, p7);
        String Green_data = incomingData.substring(p7 + 1, p8);
        String Led_Brightness_data = incomingData.substring(p8 + 1, p9);
        String LEFT_DISTANCE_THRESHOLD_data = incomingData.substring(p9 + 1, p10);
        String RIGHT_DISTANCE_THRESHOLD_data = incomingData.substring(p10 + 1, p11);
        String Speed_data = incomingData.substring(p11 + 1, p12);
        String OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER_data = incomingData.substring(p12 + 1, p13);
        String Start_Stop_data = incomingData.substring(p13 + 1, p14);
        String MEDIAN_FILTER_data = incomingData.substring(p14 + 1, p15);
        String NOISE_FILTER_data = incomingData.substring(p15 + 1, p16);
        String THERSHOLD_PREV_data = incomingData.substring(p16 + 1, p17);
        String LENK_FILTER_data = incomingData.substring(p17 + 1, p18);

        Speed_data.replace(",", ".");
        float Speed_data_float = Speed_data.toFloat();
        int Speed_data_int = Speed_data_float * 100;

        Led_Brightness = Led_Brightness_data.toInt();
        data_eepromm[0] = Led_Brightness;
        int Brightness = Brightness_data.toInt();
        data_eepromm[1] = Brightness;
        int Red = Red_data.toInt() / Led_Brightness;
        data_eepromm[2] = Red;
        int Blue = Blue_data.toInt() / Led_Brightness;
        data_eepromm[3] = Blue;
        int Green = Green_data.toInt() / Led_Brightness;
        data_eepromm[4] = Green;

        OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER = OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER_data.toInt();

        data_eepromm[5] = OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;

        OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER = OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER_data.toInt();

        data_eepromm[6] = OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER;

        OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER = OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER_data.toInt();

        data_eepromm[12] = OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER;

        THRESHOLD_GRAY = THRESHOLD_GRAY_data.toInt();
        THRESHOLD_DIFFERENCE = THRESHOLD_GRAY;
        data_eepromm[7] = THRESHOLD_GRAY;

        MIN_TRACK_WIDTH = MIN_TRACK_WIDTH_data.toInt();
        data_eepromm[8] = MIN_TRACK_WIDTH;

        LEFT_DISTANCE_THRESHOLD = LEFT_DISTANCE_THRESHOLD_data.toInt();
        data_eepromm[9] = LEFT_DISTANCE_THRESHOLD;

        RIGHT_DISTANCE_THRESHOLD = RIGHT_DISTANCE_THRESHOLD_data.toInt();
        data_eepromm[10] = RIGHT_DISTANCE_THRESHOLD;

        SOLL_SPEED = Speed_data_int;
        SLOW_SPEED = SOLL_SPEED / 2;
        data_eepromm[11] = SOLL_SPEED;

        Start_Stop = Start_Stop_data.toInt();

        int MEDIAN_FILTER = MEDIAN_FILTER_data.toInt();
        float NOISE_FILTER = NOISE_FILTER_data.toFloat();
        int THERSHOLD_PREV = THERSHOLD_PREV_data.toInt();
        float LENK_FILTER = LENK_FILTER_data.toFloat();
        int LENK_FILTER_int = LENK_FILTER * 100;
        int NOISE_FILTER_int = NOISE_FILTER * 100;

        MEDIAN_FILTER_global = data_eepromm[13];
        NOISE_FILTER_global = float(data_eepromm[14]) / 100;
        THERSHOLD_PREV_global = data_eepromm[15];
        LENK_FILTER_global = LENK_FILTER;

        //setMedianFilterSize(MEDIAN_FILTER_global);

        data_eepromm[13] = MEDIAN_FILTER;
        data_eepromm[14] = NOISE_FILTER_int;
        data_eepromm[15] = THERSHOLD_PREV;
        data_eepromm[16] = LENK_FILTER_int;
        data_eepromm[17] = 0;

        //Serial.println(MEDIAN_FILTER);
        //Serial.println(NOISE_FILTER);
        //Serial.println(THERSHOLD_PREV);
        //Serial.println(LENK_FILTER);

        //Serial.println(incomingData.length());
        for (int i = 0; i < ARRAY_EEPROMM_SIZE; i++) {
          EEPROM_write_read(i, data_eepromm[i], true);
          //Serial.print(" ");
          //Serial.print(data_eepromm[i]);
        }
        set_LED(Red, Green, Blue);
        //Serial.println(Start_Stop);
        Speed_data = prepare_speed_data(SOLL_SPEED);
        KL25z_data(Speed_data);
        pixy.setCameraBrightness(Brightness);
      }
      incomingData = "";
    }
  }
}
uint8_t EEPROM_write_read(int address, uint8_t data, bool read_write) {
  int data_read = 0;
  if (read_write) {
    EEPROM.write(address + 10, data);
    if (EEPROM.read(200) != 230) {
      EEPROM.write(200, 230);
    }

  } else if (!read_write) {
    return data_read = EEPROM.read(address + 10);
  }
  return 0;
}
int prev_millis = 0;
bool seen_symbol = false;

CRGB schweif[5] = {
  CRGB(5, 0, 0),
  CRGB(25, 0, 0),
  CRGB(50, 0, 0),
  CRGB(107, 0, 0),
  CRGB(255, 0, 0)
};

CRGB schweif_inv[5] = {
  CRGB(255, 0, 0),
  CRGB(107, 0, 0),
  CRGB(50, 0, 0),
  CRGB(25, 0, 0),
  CRGB(5, 0, 0)
};

void generate_steer_angle_string(int steer_angle) {

  if ((steer_angle >= 0 && steer_angle < 225) || (steer_angle <= 0 && steer_angle > -225)) {
    if (maybe_on_straight) {
      if (millis() - begin_of_steering_straight > 50) {
        currently_in_curve = false;
      }
    } else {
      begin_of_steering_straight = millis();
      maybe_on_straight = true;
    }
    steer_angle = steer_angle * 0.75;
  } else {
    maybe_on_straight = false;
    currently_in_curve = true;
  }

<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
=======
>>>>>>> Stashed changes
  /*if (seen_symbol == true) {
    steer_angle = 0;
    seen_symbol = false;
    prev_millis = millis();
  }

  if (millis() - prev_millis < 100) {
    steer_angle = 0;
  }*/


<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
  if (steer_angle < 0) {  // steer left
    //if(last_steering_direction == "right")
    //steer_angle = steer_angle / 2;

    if (steer_angle > -10) {
      steer_angle = steer_angle * 1.25;
      if (steer_angle > 450)  //550
        steer_angle = 450;    //550

      if (currently_in_curve)
        current_angle_string = "L00" + String(abs(steer_angle)) + "C";
      else
        current_angle_string = "L00" + String(abs(steer_angle)) + "S";
    } else if (steer_angle > -100) {
      if (currently_in_curve)
        current_angle_string = "L0" + String(abs(steer_angle)) + "C";
      else
        current_angle_string = "L0" + String(abs(steer_angle)) + "S";
    } else {
      if (currently_in_curve)
        current_angle_string = "L" + String(abs(steer_angle)) + "C";
      else
        current_angle_string = "L" + String(abs(steer_angle)) + "S";
    }
  } else if (steer_angle > 0) {  // steer right
    //if(last_steering_direction == "left")
    //steer_angle = steer_angle / 2;
    steer_angle = steer_angle * 1.25;
    if (steer_angle > 450)  //550
      steer_angle = 450;    //550

    if (steer_angle < 10) {
      if (currently_in_curve)
        current_angle_string = "R00" + String(steer_angle) + "C";
      else
        current_angle_string = "R00" + String(steer_angle) + "S";
    } else if (steer_angle < 100) {
      if (currently_in_curve)
        current_angle_string = "R0" + String(steer_angle) + "C";
      else
        current_angle_string = "R0" + String(steer_angle) + "S";
    } else {
      if (currently_in_curve)
        current_angle_string = "R" + String(steer_angle) + "C";
      else
        current_angle_string = "R" + String(steer_angle) + "S";
    }
  } else {  // straight
    current_angle_string = "S000S";
  }
}
void Ultrasonic_LEFT_ISR() {
  durationMicroSec_LEFT = micros();
  //
  distanceCmLeft = abs(durationMicroSec_LEFT - durationMicroSec_LEFT_prev - 500) / 2.0 * 0.0343;
  //Serial.println(distanceCmLeft);
  triggert_left = true;
}
void Ultrasonic_RIGHT_ISR() {
  durationMicroSec_RIGHT = micros();
  //Serial.println(distanceCmRight);
  distanceCmRight = abs(durationMicroSec_RIGHT - durationMicroSec_RIGHT_prev - 500) / 2.0 * 0.0343;
  triggert_right = true;
}
void load_eeprom() {
  if (EEPROM.read(200) == 230) {
    for (int i = 0; i < ARRAY_EEPROMM_SIZE; i++) {
      data_eepromm[i] = EEPROM_write_read(i, 0, false);
      //Serial.print(" ");
      //Serial.print(data_eepromm[i]);
    }
    Led_Brightness = data_eepromm[0];
    int Brightness = data_eepromm[1];
    int Red = data_eepromm[2];
    int Blue = data_eepromm[3];
    int Green = data_eepromm[4];

    OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER = data_eepromm[5];
    OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER = data_eepromm[6];
    OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER = data_eepromm[12];
    THRESHOLD_GRAY = data_eepromm[7];
    THRESHOLD_DIFFERENCE = THRESHOLD_GRAY;
    MIN_TRACK_WIDTH = data_eepromm[8];
    LEFT_DISTANCE_THRESHOLD = data_eepromm[9];
    SOLL_SPEED = data_eepromm[11];
    SLOW_SPEED = SOLL_SPEED / 2;

    MEDIAN_FILTER_global = data_eepromm[13];
    NOISE_FILTER_global = (float(data_eepromm[14])) / 100;
    THERSHOLD_PREV_global = data_eepromm[15];
    LENK_FILTER_global = (float(data_eepromm[16])) / 100;

    // Serial.println(MEDIAN_FILTER_global);
    // Serial.println(NOISE_FILTER_global);
    // Serial.println(THERSHOLD_PREV_global);
    // Serial.println(LENK_FILTER_global);

    setMedianFilterSize(MEDIAN_FILTER_global);

    set_LED(Red, Green, Blue);
    pixy.setCameraBrightness(Brightness);
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    Serial.println("");
=======
    //Serial.println("");
>>>>>>> Stashed changes
=======
    //Serial.println("");
>>>>>>> Stashed changes
  }
}
void setup() {
  // put your setup code here, to run once:
  //pinMode(PIN_LED_1, OUTPUT);
  //digitalWrite(PIN_LED_1, LOW);
  //pinMode(PIN_LED_2, OUTPUT);
  //digitalWrite(PIN_LED_2, LOW);
  //pinMode(PIN_LED_3, OUTPUT);
<<<<<<< Updated upstream
<<<<<<< Updated upstream

  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
=======
=======
>>>>>>> Stashed changes
  initializeBuffers();
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
  //digitalWrite(PIN_LED_3, LOW);
  //digitalWrite(PIN_LED_1, HIGH);
  //digitalWrite(PIN_LED_2, HIGH);
  //digitalWrite(PIN_LED_3, HIGH);
  Serial.begin(500000);
  pinMode(PIN_BT, INPUT_PULLUP);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(TRIGGER_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
  pinMode(TRIGGER_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);
  pinMode(PIN_LIGHT_SENSOR, INPUT);

  attachInterrupt(ECHO_PIN_RIGHT, Ultrasonic_RIGHT_ISR, FALLING);
  attachInterrupt(ECHO_PIN_LEFT, Ultrasonic_LEFT_ISR, FALLING);
  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug("Voltage", "");
  debug(String(voltage_of_battery) + "\n", "");
  if (voltage_of_battery < 6.25) {
    debug("voltage of battery too low on startup", "");
    digitalWrite(PIN_RELAY, LOW);
  } else {
    debug("turned on relay", "");
    digitalWrite(PIN_RELAY, HIGH);
  }
  Serial.println("pixy");
  int counter_nightrider = 0;
  int LED_POS = 0;
  bool invert = false;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
  while (1) {

    EVERY_N_MILLIS(50) {

      if (invert) {
        for (int i = 0; i < 12; i++) {
          if (i == LED_POS)
            leds[i] = CRGB(255, 0, 0);
          else
            leds[i] = CRGB(0, 0, 0);
        }
      } else
        for (int i = 0; i < 12; i++) {
          if (i == LED_POS)
            leds[11 - i] = CRGB(255, 0, 0);
          else
            leds[11 - i] = CRGB(0, 0, 0);
        }

      LED_POS++;
      if (LED_POS >= NUM_LEDS) {
        LED_POS = 0;
        invert = !invert;
      }

      FastLED.show();
      counter_nightrider++;
      if(counter_nightrider >= 200)
        break;
    }
  }
  pixy.init();

=======
  int invert_leds = 0;
  while (pixy.init() != 0)
    ;

=======
  int invert_leds = 0;
  while (pixy.init() != 0)
    ;

>>>>>>> Stashed changes
  while (1) {

    EVERY_N_MILLIS(20) {


      if (!invert) {
        if (LED_POS < 25) {
          for (int y = 0; y < 5; y++) {
            leds[LED_POS + y] = schweif[y];
            leds[(19-LED_POS) + 24 + y] = schweif_inv[y];
          }
        }
        if (LED_POS >= 0){
          leds[LED_POS - 1] = CRGB(0, 0, 0);
          leds[(19-LED_POS) + 24 + 5] = CRGB(0, 0, 0);
        }
      } else {
        if (LED_POS < 20) {
          for (int y = 0; y < 5; y++) {
            leds[LED_POS + y] = schweif_inv[y];
            leds[(19-LED_POS) + 24 + y] = schweif[y];
          }
        }

        if (LED_POS < 25){
          leds[LED_POS + 5] = CRGB(0, 0, 0);
          //Serial.println(LED_POS + 25 + 5);
          leds[(19-LED_POS) + 28 - 5] = CRGB(0, 0, 0);
        }
        
      }

      if(!invert)
        LED_POS++;
      else
        LED_POS--;

      //Serial.println(LED_POS);

      if (LED_POS > 19) {
        LED_POS = 19;
        invert = !invert;
      }
      if (LED_POS == 0) {
        LED_POS = 0;
        invert = !invert;
      }
      FastLED.show();
      //   if (invert) {

      //   } else
      //     for (int i = 0; i < NUM_LEDS; i++) {
      //       if (i == LED_POS)
      //         leds[(NUM_LEDS - 1) - i] = CRGB(255, 0, 0);
      //       else
      //         leds[(NUM_LEDS - 1) - i] = CRGB(0, 0, 0);
      //     }

      //   LED_POS++;
      //


        counter_nightrider++;
        if (counter_nightrider >= 400)
          break;
    }
  }
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

  Serial.println(pixy.changeProg("video"));
  current_lamp_status = 1;
  //pixy.setLamp(current_lamp_status, current_lamp_status);
  //Serial.print("Turned on lamps...\n");
  pinMode(I2C_READY_PIN, INPUT);
  //attachInterrupt(I2C_READY_PIN, send_interpolated_string, RISING);
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);


  load_eeprom();
  //Serial1.begin(38400); // Default communication rate of the Bluetooth module
  Serial1.begin(500000);

  Serial4.begin(115200);
}

int return_map(int value_to_map, int in_min, int in_max, int out_min, int out_max) {
  return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int calculate_pull_towards_ideallinie_in_degrees(int distance_from_ideallinie) {
  //int angle = return_map(distance_from_ideallinie, 0, 20, 0, 450);
  debug("distance_from_ideallinie: " + String(distance_from_ideallinie) + "\n", "");
  int angle = return_map(distance_from_ideallinie, 0, 65, 0, 1350);

  if (distance_from_ideallinie <= 5)
    angle = angle * 0.75;

  if (angle > 450)
    angle = 450;


  return angle;
}

int track_center_max_dif = 10;
int prev_track_center = 0;
int prev_left_line = 0;
int prev_index_75 = 0;


int detect_lines(uint8_t array[63], uint8_t which_line, uint8_t rgb[63]) {
  number_of_lines = 0;
  uint8_t track_width_at_current_line = 0;
  if (which_line == 75)
    track_width_at_current_line = OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER * 2;
  else if (which_line == 120)
    track_width_at_current_line = OFFSET_FROM_ONE_LINE_AT_120_TO_CENTER * 2;
  else
    track_width_at_current_line = OFFSET_FROM_ONE_LINE_AT_165_TO_CENTER * 2;

  debug("THRESHOLD_GRAY: " + String(THRESHOLD_GRAY) + "\n", "");
  debug("track_width_at_current_line: " + String(track_width_at_current_line) + "\n", "");

  /*for(int i = 0; i < 63; i++) {
    debug(String(array[i]) + "\n");
    }*/

  bool left_one_found = false;
  bool right_one_found = false;


  /*for (int i = track_center; i >= 0; i--) {
    if (i >= 0 && i < 63) {
      int index_of_left_line_buf[2];
      int index_toggle = 0;
      if (array[i] >= THRESHOLD_GRAY && array[constrain(i - 1, 0, 62)] < THERSHOLD_PREV_global && array[constrain(i + 1, 0, 62)] < THERSHOLD_PREV_global) {
        index_of_left_line = i;
        //index_of_left_line_buf[index_toggle] = i;

        //index_toggle = index_toggle + 1;

        //if (index_toggle == 2)
        //  index_toggle = 0;
        //if(index_of_left_line_buf[0] - index_of_left_line_buf[1])
        debug("found left line at: " + String(i) + "\n", "");
        //Serial.println("found left line at: " + String(i));
        left_one_found = true;
        number_of_lines++;
        break;
      }
    }
  }*/
  for (int i = 0; i < 63; i++) {
    if (i >= 0 && i < 63) {
      int count_white = 0;
      if (array[i] >= THRESHOLD_GRAY) /*&& array[constrain(i - 1, 0, 62)] < THERSHOLD_PREV_global && array[constrain(i + 1, 0, 62)] < THERSHOLD_PREV_global)*/ {

        index_of_left_line = i;
        for (int y = index_of_left_line; y < index_of_left_line + 20; y++) {
          if (y > 62)
            break;
          if (rgb[y] > THERSHOLD_PREV_global) {
            count_white = count_white + 1;

          } else
            count_white = count_white - 1;
        }
        if (count_white >= 15) {
          debug("found left line at: " + String(i) + "\n", "");
          //Serial.println("found left line at: " + String(i));

          left_one_found = true;
          number_of_lines++;
          if (which_line == 75)
            prev_index_75 = index_of_left_line;
          if (which_line == 120 && prev_index_75 < 25)
            left_one_found = false;
          break;
        } else
          count_white = 0;
      }
    }
  }
  Serial.println("");

  // for (int i = track_center; i < 63; i++) {
  //   if (i >= 0 && i < 63) {
  //     if (array[i] >= THRESHOLD_GRAY /*&& abs(i - index_of_left_line) > MIN_TRACK_WIDTH / 2 && array[constrain(i - 1, 0, 62)] <= THERSHOLD_PREV_global && array[constrain(i + 1, 0, 62)] <= THERSHOLD_PREV_global*/) {
  //       //if(array[i] == 0) {
  //       index_of_right_line = i;


  //       debug("found right line at: " + String(i) + "\n", "");
  //       right_one_found = true;
  //       number_of_lines++;
  //       break;
  //     }
  //   }
  // }
  int track_center_buf = 0;

  if (!left_one_found)
    track_center_buf = 0;
  else
    track_center_buf = track_center;

  for (int i = track_center_buf; i < 63; i++) {
    if (i >= 0 && i < 63) {
      int count_white = 0;
      if (array[i] >= THRESHOLD_GRAY /*&& abs(i - index_of_left_line) > MIN_TRACK_WIDTH / 2 && array[constrain(i - 1, 0, 62)] <= THERSHOLD_PREV_global && array[constrain(i + 1, 0, 62)] <= THERSHOLD_PREV_global*/) {
        //if(array[i] == 0) {
        index_of_right_line = i;
        for (int y = index_of_right_line; y > index_of_right_line - 20; y--) {
          if (y < 0)
            break;
          if (rgb[y] > THERSHOLD_PREV_global) {
            count_white = count_white + 1;

          } else
            count_white = count_white - 1;
        }
        if (count_white >= 15) {

          debug("found right line at: " + String(i) + "\n", "");
          right_one_found = true;
          number_of_lines++;
          break;
        }
      }
    }
  }


  if (left_one_found && right_one_found) {
    debug("MIN_TRACK_WIDTH: " + String(MIN_TRACK_WIDTH) + "\n", "");
    debug("abs{index_of_right_line - index_of_left_line}: " + String(abs(index_of_right_line - index_of_left_line)) + "\n", "");
    //debug("abs{index_of_right_line - index_of_left_line}: \n", "");
    if (abs(index_of_right_line - index_of_left_line) < MIN_TRACK_WIDTH && (mode == 0 || mode == 1)) {
      if (millis() - last_symbol_time > 1000) {
        int rgb_average = 0;
        if (which_line == 75) {
          for (int i = 0; i < 63; i++)
            rgb_average += rgb_75[i];
        } else if (which_line == 120) {
          for (int i = 0; i < 63; i++)
            rgb_average += rgb_120[i];
        } else if (which_line == 165) {
          for (int i = 0; i < 63; i++)
            rgb_average += rgb_165[i];
        }
        rgb_average /= 63;
        debug("RGB average on line " + String(which_line) + " is: " + String(rgb_average) + "\n", "");
        if (rgb_average > RGB_THRESHOLD_FOR_SYMBOLS) {
          last_symbol_time = millis();
          debug("+++++ FOUND SYMBOL ON TRACK +++++\n", "");
          int num_lines = count_lines(array);
          seen_symbol = true;
          debug("number of lines: " + String(num_lines) + "\n", "");
          if (mode == 0) {
            if (num_lines >= 2) {
              if (millis() - last_finish_line > 5000 && millis() - last_finish_line > 4000) {
                set_LED(0, 255, 255);
                delay(250);
                Start_Stop = 2;
              }
              last_finish_line = millis();
              // debug("seeing finish line\n", "");
            }
          } else if (mode == 1) {
            if ((last_symbol == "slow" || num_lines >= 5) && last_symbol != "fast") {
              //if((last_symbol == "slow" || num_lines >= 5)) {
              debug("seeing faster line\n", "");
              set_LED(255, 0, 255);
              String speed = prepare_speed_data(SOLL_SPEED);
              //debug(speed + "\n", "");
              KL25z_data(speed);
              last_symbol = "fast";
            } else if (last_symbol == "fast" || (num_lines < 5 && num_lines >= 3)) {
              debug("seeing slower line\n", "");
              set_LED(255, 255, 0);
              String speed = prepare_speed_data(SLOW_SPEED);
              //debug(speed + "\n", "");
              KL25z_data(speed);
              last_symbol = "slow";
            }
          }
        }
        // TODO: continue with last steering angle
        index_of_left_line = 0;
        index_of_right_line = 63;
        track_center = (index_of_left_line + index_of_right_line) / 2;
      }
    } else {
      track_center = (index_of_left_line + index_of_right_line) / 2;
    }
  } else if (left_one_found && !right_one_found) {
    if (dodge_to_the_right && index_of_left_line > track_width_at_current_line / 2) {
      index_of_right_line = index_of_left_line;
      index_of_left_line = index_of_right_line - track_width_at_current_line;
      track_center = (index_of_left_line + index_of_right_line) / 2;
      left_one_found = !left_one_found;
      right_one_found = !right_one_found;
      debug("detected left line but dodging to the right. so this has to be the right line!\n", "");
    } else {
      index_of_right_line = index_of_left_line + track_width_at_current_line;
      track_center = (index_of_left_line + index_of_right_line) / 2;
    }
  } else if (!left_one_found && right_one_found) {
    if (dodge_to_the_left && index_of_right_line < track_width_at_current_line / 2) {
      index_of_left_line = index_of_right_line;
      index_of_right_line = index_of_left_line + track_width_at_current_line;
      track_center = (index_of_left_line + index_of_right_line) / 2;
      left_one_found = !left_one_found;
      right_one_found = !right_one_found;
      debug("detected right line but dodging to the left. so this has to be the left line!\n", "");
    } else {
      index_of_left_line = index_of_right_line - track_width_at_current_line;
      track_center = (index_of_left_line + index_of_right_line) / 2;
    }
  }

  /*if (abs(track_center - prev_track_center) > track_center_max_dif) {
    track_center = prev_track_center;
    debug("Trackcenter dif to big: " + String(abs(track_center - prev_track_center)) + "\n", "");
    debug("Continue with old Trackcenter: " + String(track_center) + "\n", "");
  }*/

  //prev_track_center = track_center;
  debug("found " + String(number_of_lines) + " lines\n", "");
  debug("found left line: " + String(left_one_found) + "\n", "");
  debug("found right line: " + String(right_one_found) + "\n", "");
  debug("left line at: " + String(index_of_left_line) + "\n", "");
  debug("right line at: " + String(index_of_right_line) + "\n", "");
  debug("center at: " + String(track_center) + "\n", "");


  if (dodge_to_the_left) {
    index_of_left_line -= (track_width_at_current_line / 4);
    index_of_right_line -= (track_width_at_current_line / 4);
    track_center = (index_of_left_line + index_of_right_line) / 2;
    debug("dodging to the left\n", "");
    debug("new center at: " + String(track_center) + "\n", "");
  }
  if (dodge_to_the_right) {
    index_of_left_line += (track_width_at_current_line / 4);
    index_of_right_line += (track_width_at_current_line / 4);
    track_center = (index_of_left_line + index_of_right_line) / 2;
    debug("dodging to the right\n", "");
    debug("new center at: " + String(track_center) + "\n", "");
  }
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
  return track_center;
>>>>>>> Stashed changes
=======
  return track_center;
>>>>>>> Stashed changes
}

uint8_t count_lines(uint8_t array[63]) {
  uint8_t number_of_lines = 0;
  bool currently_in_line = false;
  int index_of_last_line = -MIN_TRACK_WIDTH - 5;
  int MIN_DISTANCE_SYMBOL_LINES = 2;

  for (int i = 1; i < 63; i++) {
    /*if (!currently_in_line && array[i] == 0 && i > index_of_last_line + MIN_DISTANCE_SYMBOL_LINES) {
      indexes_of_lines[number_of_lines] = index_of_last_line;
      number_of_lines++;
      currently_in_line = true;
      }
      if ( (array[i] == 1 || array[i] == 255) && currently_in_line) {
      currently_in_line = false;
      index_of_last_line = i;
      }*/
    if (abs(array[i] - array[i - 1]) > THRESHOLD_GRAY) {
      if (array[i] > array[i - 1]) {
        debug("line edge at " + String(i) + "\n", "");
        i += 2;
        number_of_lines++;
      } else if (array[i] < array[i - 1]) {
        debug("line edge at " + String(i) + "\n", "");
        i += 2;
        number_of_lines++;
      }
      debug("counted line at " + String(i) + "\n", "");
    }
  }

  if (number_of_lines % 2 == 1)
    number_of_lines++;

  number_of_lines = number_of_lines / 2;

  return number_of_lines;
}

void connect_line_edges(uint8_t array[], uint8_t size) {
  bool line_edge_found = false;
  uint8_t line_edge_index = 0;
  uint8_t line_width_counter = 0;


  for (int i = 0; i < size; i++) {
    if (array[i] < THRESHOLD_DIFFERENCE && !line_edge_found)
      array[i] = 0;
    else {
      if (array[i] > THRESHOLD_DIFFERENCE && !line_edge_found) {
        array[i] = 0;
        line_edge_found = true;
        line_width_counter = 0;
        line_edge_index = i;
      }

      if (line_edge_found && ((array[i] > THRESHOLD_DIFFERENCE && array[i] < 255)) && i > line_edge_index) {
        for (int j = line_edge_index; j < i; j++)
          array[j] = array[line_edge_index];
        array[i] = 0;
      }
    }

    if (line_edge_found) {
      line_width_counter++;
    }

    if (line_width_counter > 4)
      line_edge_found = false;
  }
}

/*void detect_if_left_or_right_line(uint8_t array[63]) {
  if (index_of_left_line == index_of_right_line) {
    index_of_left_line -= 1;
    index_of_right_line += 1;
  }
  int index_of_line = 0;
  for (int i = 0; i < 63; i++) {
    if (array[i] == 0) {
      index_of_line = i;
      if (i > 32)
        break;
    }
  }
  int distance_from_left_index = abs(index_of_left_line - index_of_line);
  int distance_from_right_index = abs(index_of_right_line - index_of_line);
  if (distance_from_right_index <= distance_from_left_index) {
    index_of_right_line = index_of_line;
    currently_seeing_both_lines = false;
    currently_seeing_only_left_line = false;
    currently_seeing_only_right_line = true;
  } else {
    index_of_left_line = index_of_line;
    currently_seeing_both_lines = false;
    currently_seeing_only_left_line = true;
    currently_seeing_only_right_line = false;
  }

  }*/

void set_LED(uint8_t R, uint8_t G, uint8_t B) {
  //analogWrite(PIN_LED_2, R);
  //analogWrite(PIN_LED_3, G);
  //analogWrite(PIN_LED_1, B);

<<<<<<< Updated upstream
<<<<<<< Updated upstream
  for (int i = 0; i < 12; i++)
=======
  for (int i = 0; i < NUM_LEDS; i++)
>>>>>>> Stashed changes
=======
  for (int i = 0; i < NUM_LEDS; i++)
>>>>>>> Stashed changes
    leds[i] = CRGB(R, G, B);

  FastLED.show();
}
unsigned long micros_prev_right = 0;
bool triggered_right = false;  // Zustand des Triggers

void Ultrasonic_RIGHT() {
  unsigned long current_time = micros();  // micros() nur einmal aufrufen

  if (triggert_right && !triggered_right) {
    digitalWrite(TRIGGER_PIN_RIGHT, HIGH);
    micros_prev_right = current_time;  // Zeitmessung starten
    triggert_right = false;
    triggered_right = true;  // Trigger ist jetzt aktiv
  }

  if (triggered_right && (current_time - micros_prev_right > 100)) {
    digitalWrite(TRIGGER_PIN_RIGHT, LOW);
    durationMicroSec_RIGHT_prev = micros();
    triggered_right = false;  // Trigger deaktivieren
  }

  /*if (distanceCmRight < 1 || distanceCmRight > 400) {
    distanceCmRight = 1000; // Fehlerwert setzen
  }*/
}

unsigned long micros_prev_left = 0;
bool triggered_left = false;  // Zustand des Triggers

void Ultrasonic_LEFT() {
  unsigned long current_time = micros();  // micros() nur einmal aufrufen

  if (triggert_left && !triggered_left) {
    digitalWrite(TRIGGER_PIN_LEFT, HIGH);
    micros_prev_left = current_time;  // Zeitmessung starten
    triggert_left = false;
    triggered_left = true;  // Trigger ist jetzt aktiv
  }

  if (triggered_left && (current_time - micros_prev_left > 100)) {
    digitalWrite(TRIGGER_PIN_LEFT, LOW);
    durationMicroSec_LEFT_prev = micros();
    triggered_left = false;  // Trigger deaktivieren
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
=======
  }

  /*if (distanceCmLeft < 1 || distanceCmLeft > 400) {
    distanceCmLeft = 1000; // Fehlerwert setzen
  }*/
}

float output_75[63];
float output_120[63];
float output_165[63];
float buffer[10];  // Temporärer statischer Puffer

void applyBilateralFilterToArrayStatic(uint8_t* input, float* output, int length, int windowSize, float spatialSigma, float intensitySigma) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < windowSize; j++) {
      int index = i + j - windowSize / 2;
      if (index < 0) index = 0;
      if (index >= length) index = length - 1;
      buffer[j] = (float)input[index];
    }
    output[i] = applyBilateralFilter(buffer, windowSize, spatialSigma, intensitySigma);
>>>>>>> Stashed changes
  }

  /*if (distanceCmLeft < 1 || distanceCmLeft > 400) {
    distanceCmLeft = 1000; // Fehlerwert setzen
  }*/
}

<<<<<<< Updated upstream
float output_75[63];
float output_120[63];
float output_165[63];
float buffer[10];  // Temporärer statischer Puffer

void applyBilateralFilterToArrayStatic(uint8_t* input, float* output, int length, int windowSize, float spatialSigma, float intensitySigma) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < windowSize; j++) {
      int index = i + j - windowSize / 2;
      if (index < 0) index = 0;
      if (index >= length) index = length - 1;
      buffer[j] = (float)input[index];
    }
    output[i] = applyBilateralFilter(buffer, windowSize, spatialSigma, intensitySigma);
>>>>>>> Stashed changes
  }

  /*if (distanceCmLeft < 1 || distanceCmLeft > 400) {
    distanceCmLeft = 1000; // Fehlerwert setzen
  }*/
}

<<<<<<< Updated upstream
int no_echo_time = 1000;
int no_echo_time_prev = 0;

=======
=======
>>>>>>> Stashed changes

float applyBilateralFilter(float* buffer, int size, float spatialSigma, float intensitySigma) {
  float filteredValue = 0.0;
  float weightSum = 0.0;
  float centerValue = buffer[size / 2];  // Zentraler Pixelwert

  for (int i = 0; i < size; i++) {
    float spatialWeight = exp(-pow(i - size / 2, 2) / (2 * spatialSigma * spatialSigma));
    float intensityWeight = exp(-pow(buffer[i] - centerValue, 2) / (2 * intensitySigma * intensitySigma));
    float weight = spatialWeight * intensityWeight;

    filteredValue += buffer[i] * weight;
    weightSum += weight;
  }

  return filteredValue / weightSum;
}

uint8_t output_sobel_75[63];
uint8_t output_sobel_120[63];
uint8_t output_sobel_165[63];

void sobelFilter(uint8_t* input, uint8_t* output_sobel, int length, int Gain) {
  // Speicher für das Ausgabe-Array allokieren
  for (int i = 1; i < length - 1; i++) {
    // Gradienten berechnen (Differenz der Nachbarn)
    int gradient = abs(input[i + 1] - input[i - 1]);

    // Werte auf den gültigen Bereich von uint8_t begrenzen
    output_sobel[i] = (uint8_t)constrain(gradient * Gain, 0, 255);
  }

  // Ränder setzen
  output_sobel[0] = 0;
  output_sobel[length - 1] = 0;
}

uint8_t* laplaceFilter(uint8_t* input, int length) {
  // Speicher für das Ausgabe-Array allokieren
  uint8_t* output = new uint8_t[length];

  // Filter anwenden
  for (int i = 1; i < length - 1; i++) {
    // Berechnung des Laplace-Filters
    int value = -input[i - 1] + 2 * input[i] - input[i + 1];

    // Wert auf gültigen Bereich (0-255) begrenzen
    output[i] = (uint8_t)constrain(value, 0, 255);
  }

  // Ränder setzen
  output[0] = 0;
  output[length - 1] = 0;

  return output;  // Gefiltertes Array zurückgeben
}


int no_echo_time = 1000;
int no_echo_time_prev = 0;

int medianFilterSize = 5;  // Initiale Größe des Medianfilters
float* buffer_25 = nullptr;
float* buffer_75 = nullptr;
float* buffer_120 = nullptr;
float* buffer_165 = nullptr;

// Speicher für Puffer initialisieren
void initializeBuffers() {
  delete[] buffer_25;
  delete[] buffer_75;
  delete[] buffer_120;
  delete[] buffer_165;

  buffer_25 = new float[medianFilterSize]();
  buffer_75 = new float[medianFilterSize]();
  buffer_120 = new float[medianFilterSize]();
  buffer_165 = new float[medianFilterSize]();
}

// Puffer aktualisieren
void updateBuffer(float* buffer, float newValue) {
  for (int i = medianFilterSize - 1; i > 0; i--) {
    buffer[i] = buffer[i - 1];
  }
  buffer[0] = newValue;  // Neuer Wert hinzufügen
}
bool median_onoff = false;
// Medianfilter anwenden
float applyMedianFilter(float* buffer) {
  float* sorted = new float[medianFilterSize];
  memcpy(sorted, buffer, sizeof(float) * medianFilterSize);

  // Array sortieren (Bubble Sort für kleine Arrays)
  for (int i = 0; i < medianFilterSize - 1; i++) {
    for (int j = 0; j < medianFilterSize - i - 1; j++) {
      if (sorted[j] > sorted[j + 1]) {
        float temp = sorted[j];
        sorted[j] = sorted[j + 1];
        sorted[j + 1] = temp;
      }
    }
  }

  // Median zurückgeben (mittlerer Wert im sortierten Array)
  float median = sorted[medianFilterSize / 2];
  delete[] sorted;  // Dynamisch zugewiesenen Speicher freigeben
  return median;
}

// Medianfiltergröße zur Laufzeit ändern
void setMedianFilterSize(int newSize) {
  if (newSize < 3 || newSize % 2 == 0) {
    Serial.println("Error: Filter size must be an odd number greater than or equal to 3.");
    median_onoff = false;
    return;
  } else
    median_onoff = true;



  medianFilterSize = newSize;
  initializeBuffers();  // Speicher neu initialisieren
  Serial.print("Median filter size updated to: ");
  Serial.println(medianFilterSize);
}

float prev_tfilter_val_75 = 0;
float prev_tfilter_val_120 = 0;
float prev_tfilter_val_165 = 0;

float low_pass_filter(float filterValue, float gefiltertestSignal, int eingangsSignal) {
  return gefiltertestSignal = eingangsSignal * (1 - filterValue) + gefiltertestSignal * filterValue;
}

float prev_hfilter_val_75 = 0;
float prev_hfilter_val_120 = 0;
float prev_hfilter_val_165 = 0;

float high_pass_filter(float filterValue, float gefiltertestSignal, int eingangsSignal, int Ordnung) {
  float filterHelper = 0;
  for (int i = 0; i < Ordnung; i++) {
    if (i == 0) {
      filterHelper = eingangsSignal * filterValue + filterHelper * (1 - filterValue);
      gefiltertestSignal = eingangsSignal - filterHelper;
    } else {
      filterHelper = gefiltertestSignal * filterValue + filterHelper * (1 - filterValue);
      gefiltertestSignal = gefiltertestSignal - filterHelper;
    }
  }
  return gefiltertestSignal;
}
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

void loop() {
  //debug(String(SOLL_SPEED) + "\n", "");
  /*if(millis() - no_echo_time_prev >= no_echo_time)
    {
      triggert_left =  true;
      triggert_right = true;
      no_echo_time_prev = millis();
    }*/
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
=======
>>>>>>> Stashed changes
  // if (Serial.available()) {
  //   String input = Serial.readStringUntil('\n');  // Lese Eingabe bis zum Zeilenumbruch
  //   input.trim();  // Entferne Leerzeichen und Zeilenumbrüche
  //   if (input.indexOf(',') != -1) {
  //     // Extrahiere lowCutoff und highCutoff
  //     String lowStr = input.substring(0, input.indexOf(','));
  //     String highStr = input.substring(input.indexOf(',') + 1);

  //     // Konvertiere zu float
  //     float lowCutoff = lowStr.toFloat();
  //     float highCutoff = highStr.toFloat();

  //     // Berechne FIR-Koeffizienten
  //     calculateFIRCoefficients(filterCoeffs, filterSize, lowCutoff, highCutoff, 56000000);
  //   }
  // }
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
  Ultrasonic_RIGHT();
  Ultrasonic_LEFT();
  debug("distanceCMLeft: ", "");
  debug(String(distanceCmLeft) + "\n", "");

  debug("distanceCMRight: ", "");
  debug(String(distanceCmRight) + "\n", "");

  Serial_receive();


  debug("light sensor value: " + String(analogRead(PIN_LIGHT_SENSOR)) + "\n", "");

  //Serial4.print("S000S");
  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug("Voltage ", "");
  debug(String(voltage_of_battery) + "\n", "");
  if (voltage_of_battery < 6.25) {
    battery_low_counter++;
    if (battery_low_counter > 500) {
      debug("voltage of battery too low", "");
      digitalWrite(PIN_RELAY, LOW);
      return;
    }
  }

  // put your main code here, to run repeatedly:
  uint8_t r, g, b;
  uint8_t gray;
  //debug(String(millis()) + "\n");

  //int look_towards_right_or_left = 0;



  float Gain = 1;
  //const float noiseThreshold = NOISE_FILTER_global;  // Schwelle für Rauschunterdrückung
  float previousValue_120 = 0, previousValue_165 = 0, previousValue_75 = 0;



  for (int i = 0, j = 0; i < 315; i += 5, j++) {
    // if (!digitalRead(PIN_BT)) {
    // Für Zeile 120
    float filteredGray = 0;
    pixy.video.getRGB(i, 120, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;

    rgb_120[j] = gray * Gain;

    // Für Zeile 165
    pixy.video.getRGB(i, 165, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    updateBuffer(buffer_165, gray);


    rgb_165[j] = gray * Gain;


    // Für Zeile 75
    pixy.video.getRGB(i, 75, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    if (DEBUG_GRAY_VALUES) {
      rgb_75[j] = gray;

      if (j >= 1) {
        differences_75[j - 1] = abs(rgb_75[j - 1] - rgb_75[j]);
      }
    }
    if (gray > THRESHOLD_GRAY)
      black_or_white_at_75[j] = 1;
    else
      black_or_white_at_75[j] = 0;

    //debug(String(gray) + "\n");
=======
    rgb_75[j] = gray * Gain;
>>>>>>> Stashed changes
=======
    rgb_75[j] = gray * Gain;
>>>>>>> Stashed changes
  }
  //NOISE_FILTER_global = NOISE_FILTER_global *2;
  applyBilateralFilterToArrayStatic(rgb_75, output_75, 63, 10, NOISE_FILTER_global, NOISE_FILTER_global);
  applyBilateralFilterToArrayStatic(rgb_120, output_120, 63, 10, NOISE_FILTER_global, NOISE_FILTER_global);
  applyBilateralFilterToArrayStatic(rgb_165, output_165, 63, 10, NOISE_FILTER_global, NOISE_FILTER_global);

  for (int i = 0; i < 63; i++) {
    rgb_75[i] = (uint8_t)constrain(output_75[i], 0, 255);
    rgb_120[i] = (uint8_t)constrain(output_120[i], 0, 255);
    rgb_165[i] = (uint8_t)constrain(output_165[i], 0, 255);
  }


  sobelFilter(rgb_75, output_sobel_75, 63, MEDIAN_FILTER_global);
  sobelFilter(rgb_120, output_sobel_120, 63, MEDIAN_FILTER_global);
  sobelFilter(rgb_165, output_sobel_165, 63, MEDIAN_FILTER_global);

  for (int i = 0; i < 63; i++) {
    rgb_75[i] = 255 - (uint8_t)constrain(output_sobel_75[i], 0, 255);    // Werte begrenzen und konvertieren
    rgb_120[i] = 255 - (uint8_t)constrain(output_sobel_120[i], 0, 255);  // Werte begrenzen und konvertieren
    rgb_165[i] = 255 - (uint8_t)constrain(output_sobel_165[i], 0, 255);  // Werte begrenzen und konvertieren
  }

  applyBilateralFilterToArrayStatic(rgb_75, output_75, 63, 10, 5, 5);
  applyBilateralFilterToArrayStatic(rgb_120, output_120, 63, 10, NOISE_FILTER_global, NOISE_FILTER_global);
  applyBilateralFilterToArrayStatic(rgb_165, output_165, 63, 10, NOISE_FILTER_global, NOISE_FILTER_global);

  for (int i = 0; i < 63; i++) {
    rgb_75[i] = (uint8_t)constrain(output_75[i], 0, 255);
    rgb_120[i] = (uint8_t)constrain(output_120[i], 0, 255);
    rgb_165[i] = (uint8_t)constrain(output_165[i], 0, 255);
  }


  for (int i = 0, j = 0; i < 315; i += 5, j++) {


    if (j >= 1) {
      differences_75[j - 1] = abs(rgb_75[j - 1] - rgb_75[j]);
      //Serial.print(differences_75[j - 1]);
      //Serial.print(" ");
    }

    if (j >= 1) {
      differences_120[j - 1] = abs(rgb_120[j - 1] - rgb_120[j]);
    }

    if (j >= 1) {
      differences_165[j - 1] = abs(rgb_165[j - 1] - rgb_165[j]);
    }
  }
  //Serial.println();

  differences_75[62] = 0;
  //int track_center_165 = detect_lines(differences_165, 165);
  //int track_center_120 = detect_lines(differences_120, 120);
  int track_center_75 = detect_lines(differences_75, 75, rgb_75);

  //Serial.println(track_center_75 - track_center_120);
  //Serial.println(track_center_75 - track_center_165);
  current_line = 75;



  if (number_of_lines > 0) {
    int steer_angle = calculate_pull_towards_ideallinie_in_degrees(abs(track_center_75 - 30));
    //prev_tfilter_val_75 = low_pass_filter(LENK_FILTER_global, prev_tfilter_val_75, steer_angle);
    //steer_angle = (int)prev_tfilter_val_75;
    //Serial.println(steer_angle);
    //differences_120[62] = 0;
    //int track_center_75_to_120 = detect_lines(differences_120, 120);
    //Serial.println(track_center_75_to_120 - track_center_75);
    if (track_center_75 >= 30) {
      debug("steer right: " + String(steer_angle) + "\n", "");
    } else {
      debug("steer left: " + String(steer_angle) + "\n", "");
      steer_angle = steer_angle * -1;
    }

    if (steer_angle > 450)
      steer_angle = 450;
    else if (steer_angle < -450)
      steer_angle = -450;

    generate_steer_angle_string(steer_angle);

    //write_i2c(current_angle_string);
    KL25z_data(current_angle_string);
    last_steer_angle = steer_angle;
  } else {
    debug("did not find a line at 75, now looking at 120\n", "");

<<<<<<< Updated upstream
<<<<<<< Updated upstream
    for (int i = 0, j = 0; i < 315; i = i + 5, j++) {

      pixy.video.getRGB(i, 120, &r, &g, &b, false);
      gray = 0.299 * r + 0.587 * g + 0.114 * b;
      if (DEBUG_GRAY_VALUES) {
        rgb_120[j] = gray;

        if (j >= 1) {
          differences_120[j - 1] = abs(rgb_120[j - 1] - rgb_120[j]);
        }
      }
      if (gray > THRESHOLD_GRAY)
        black_or_white_at_120[j] = 1;
      else
        black_or_white_at_120[j] = 0;
    }
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
    differences_120[62] = 0;
    //connect_line_edges(differences_120, 62);


    detect_lines(differences_120, 120, rgb_120);
    current_line = 120;

    if (number_of_lines > 0) {
      int steer_angle_120 = calculate_pull_towards_ideallinie_in_degrees(abs(track_center - 30));
      prev_tfilter_val_120 = low_pass_filter(LENK_FILTER_global, prev_tfilter_val_120, steer_angle_120);
      steer_angle_120 = (int)prev_tfilter_val_120;
      if (track_center >= 30) {
        debug("steer right: " + String(steer_angle_120) + "\n", "");
      } else {
        debug("steer left: " + String(steer_angle_120) + "\n", "");
        steer_angle_120 = steer_angle_120 * -1;
      }

      steer_angle_120 = steer_angle_120 * 1.5;
      //if((last_steer_angle > 400 && steer_angle_120 < 100 && steer_angle_120 > 0) || (last_steer_angle < -400 && steer_angle_120 > -100 && steer_angle_120 < 0))
      //steer_angle_120 = steer_angle_120 * 4;

      if (steer_angle_120 > 0 && steer_angle_120 < 100)
        steer_angle_120 = 100;
      else if (steer_angle_120 < 0 && steer_angle_120 > -100)
        steer_angle_120 = -100;

      if (steer_angle_120 > 450)
        steer_angle_120 = 450;
      else if (steer_angle_120 < -450)
        steer_angle_120 = -450;

      generate_steer_angle_string(steer_angle_120);

      //write_i2c(current_angle_string);
      KL25z_data(current_angle_string);
      last_steer_angle = steer_angle_120;
    } else {
      debug("did not find a line at 120, now looking at 165\n", "");

<<<<<<< Updated upstream
<<<<<<< Updated upstream
      for (int i = 0, j = 0; i < 315; i = i + 5, j++) {

        pixy.video.getRGB(i, 165, &r, &g, &b, false);
        gray = 0.299 * r + 0.587 * g + 0.114 * b;
        if (DEBUG_GRAY_VALUES) {
          rgb_165[j] = gray;

          if (j >= 1) {
            differences_165[j - 1] = abs(rgb_165[j - 1] - rgb_165[j]);
          }
        }
        if (gray > THRESHOLD_GRAY)
          black_or_white_at_165[j] = 1;
        else
          black_or_white_at_165[j] = 0;
      }
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
      differences_165[62] = 0;
      //connect_line_edges(differences_165, 62);


      detect_lines(differences_165, 165, rgb_165);
      current_line = 165;

      if (number_of_lines > 0) {
        int steer_angle_165 = calculate_pull_towards_ideallinie_in_degrees(abs(track_center - 30));
        prev_tfilter_val_165 = low_pass_filter(LENK_FILTER_global, prev_tfilter_val_165, steer_angle_165);
        steer_angle_165 = (int)prev_tfilter_val_165;
        if (track_center >= 30) {
          debug("steer right: " + String(steer_angle_165) + "\n", "");
        } else {
          debug("steer left: " + String(steer_angle_165) + "\n", "");
          steer_angle_165 = steer_angle_165 * -1;
        }

        steer_angle_165 = steer_angle_165 * 3.5;
        //if((last_steer_angle > 400 && steer_angle_165 < 100 && steer_angle_165 > 0) || (last_steer_angle < -400 && steer_angle_165 > -100 && steer_angle_165 < 0))
        //steer_angle_165 = steer_angle_165 * 4;

        if (steer_angle_165 > 0 && steer_angle_165 < 100)
          steer_angle_165 = 100;
        else if (steer_angle_165 < 0 && steer_angle_165 > -100)
          steer_angle_165 = -100;

        if (steer_angle_165 > 450)
          steer_angle_165 = 450;
        else if (steer_angle_165 < -450)
          steer_angle_165 = -450;

        generate_steer_angle_string(steer_angle_165);

        //write_i2c(current_angle_string);
        KL25z_data(current_angle_string);
        last_steer_angle = steer_angle_165;
      } else {  // write last steer angle
        debug("seeing no line, sending last angle string\n", "");
        current_line = 0;
        index_of_left_line = 999;
        index_of_right_line = 999;
        //write_i2c(current_angle_string);
        generate_steer_angle_string(last_steer_angle);
        KL25z_data(current_angle_string);
        last_steer_angle = last_steer_angle * 0.9;
      }
    }
  }

  debug(current_angle_string, "");

  if (!digitalRead(PIN_BT)) {
    String s_25;
    String s_75;
    String s_120;
    String s_165;
    String US_data;
    String Speed;
    String s_current_line;
    String s_index_of_left_line;
    String s_index_of_right_line;
    //debug("\n");


    for (int i = 0; i < 63; i++) {
      //debug(String(black_or_white_at_75[i]));

      s_25 += String(rgb_25[i]) + " ";
      s_75 += String(rgb_75[i]) + " ";
      s_120 += String(rgb_120[i]) + " ";
      s_165 += String(rgb_165[i]) + " ";
    }
    s_current_line = String(current_line);
    s_index_of_left_line = String(index_of_left_line);
    s_index_of_right_line = String(index_of_right_line);
    String line_indexes = s_current_line + " " + s_index_of_left_line + " " + s_index_of_right_line;
    Speed = String(Speed_right);
    US_data += String(round(abs(distanceCmLeft)), DEC);
    US_data += " ";
    US_data += String(round(abs(distanceCmRight)), DEC);
    US_data += " ";
    mainString = (" # " + s_25 + " $ " + s_75 + " ! " + s_120 + " % " + s_165 + " / " + US_data + " ( " + Speed + " ; " + line_indexes + " ) " + "\n" + "&");
    debug(mainString, mainString);
  }
  if (mode == 3) {
    // check ultrasonic values, if something is close: break
    if (distanceCmLeft <= LEFT_DISTANCE_THRESHOLD && distanceCmRight <= RIGHT_DISTANCE_THRESHOLD)
      counter_ultrasonics_triggered++;
    else
      counter_ultrasonics_triggered = 0;
    if (counter_ultrasonics_triggered >= NUMBER_OF_ULTRASONICS_TRIGGERS_FOR_STOP) {
      set_LED(0, 255, 255);
      Start_Stop = 2;
      counter_ultrasonics_triggered = 0;
    }
    /* if (distanceCmLeft <= LEFT_DISTANCE_THRESHOLD) //&& distanceCmRight <= RIGHT_DISTANCE_THRESHOLD)
       set_LED(0, 255, 255);
       Start_Stop = 2;*/
  }
  if (mode == 2) {
    if (distanceCmLeft <= LEFT_DISTANCE_THRESHOLD && distanceCmRight > RIGHT_DISTANCE_THRESHOLD)
      counter_ultrasonics_triggered_right++;
    else
      counter_ultrasonics_triggered_right = 0;

    if (counter_ultrasonics_triggered_right >= NUMBER_OF_ULTRASONICS_TRIGGERS_FOR_STOP) {
      dodge_to_the_left = false;
      dodge_to_the_right = true;
      timer_of_dodge_to_the_right = millis();
      debug("DODGE TO THE RIGHT!\n", "");
      counter_ultrasonics_triggered_right = 0;
    }

    if (distanceCmLeft > LEFT_DISTANCE_THRESHOLD && distanceCmRight <= RIGHT_DISTANCE_THRESHOLD)
      counter_ultrasonics_triggered_left++;
    else
      counter_ultrasonics_triggered_left = 0;
    if (counter_ultrasonics_triggered_left >= NUMBER_OF_ULTRASONICS_TRIGGERS_FOR_STOP) {
      dodge_to_the_left = true;
      dodge_to_the_right = false;
      timer_of_dodge_to_the_left = millis();
      debug("DODGE TO THE LEFT!\n", "");
      counter_ultrasonics_triggered_left = 0;
    }
  }

  if (dodge_to_the_left && millis() - timer_of_dodge_to_the_left > DURATION_OF_DODGE) {
    dodge_to_the_left = false;
    dodge_to_the_right = false;
  }
  if (dodge_to_the_right && millis() - timer_of_dodge_to_the_right > DURATION_OF_DODGE) {
    dodge_to_the_left = false;
    dodge_to_the_right = false;
  }

  debug("dodge_to_the_left: " + String(dodge_to_the_left) + "\n", "");
  debug("dodge_to_the_right: " + String(dodge_to_the_right) + "\n", "");

  Serial_KL25z_receive();
}
