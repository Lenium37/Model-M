#include <Pixy2.h>
#include <Wire.h> // I2Cs
#include <EEPROM.h>

#define I2C_ADDRESS_OF_SLAVE 8
#define I2C_READY_PIN 3
#define PIN_BT 5
#define PIN_RELAY 6
#define PIN_LED_1 7
#define PIN_LED_2 8
#define PIN_LED_3 9
#define PIN_LIGHT_SENSOR 14
#define PIN_BATTERY_VOLTAGE 15
#define DEBUG_GRAY_VALUES true
#define TRIGGER_PIN_LEFT 21
#define ECHO_PIN_LEFT 20
#define TRIGGER_PIN_RIGHT 4
#define ECHO_PIN_RIGHT 2
#define ARRAY_EEPROMM_SIZE 12

Pixy2 pixy;

uint16_t LEFT_DISTANCE_THRESHOLD = 10;
uint16_t RIGHT_DISTANCE_THRESHOLD = 10;
uint8_t  ULTRASONIC_SAMPELS = 10;
uint8_t  THRESHOLD_GRAY = 100;
uint8_t  MIN_TRACK_WIDTH = 20;
uint8_t  OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER = 27;
uint8_t  OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER = 30;
uint8_t  THRESHOLD_DIFFERENCE = 40;
int data_eepromm[ARRAY_EEPROMM_SIZE];

bool debug_active = false;
bool current_lamp_status = 0;
uint8_t black_or_white_at_25[63];
uint8_t black_or_white_at_75[63];
uint8_t black_or_white_at_125[63];
uint8_t black_or_white_at_175[63];
uint8_t rgb_25[63];
uint8_t rgb_75[63];
uint8_t rgb_125[63];
uint8_t rgb_175[63];
uint8_t differences_75[63];
uint8_t differences_175[63];
int line_25_until_border = 0;
int line_75_until_border = 0;
int line_125_until_border = 0;
int line_175_until_border = 0;
bool currently_in_curve = false;
String current_angle_string;
float voltage_of_battery = 10;
int battery_low_counter = 0;
int index_of_left_line = 0;
int index_of_right_line = 62;
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

void debug(String s) {
  if (digitalRead(PIN_BT) == HIGH)
  {
    Serial1.print(s);
    delay(3);
  }
  // if changing Serial number don't forget to change it in setup() as well!
}
void Serial_receive()
{
  if (digitalRead(PIN_BT) == HIGH)
  {
    if (Serial1.available() > 0) {
      // Lies das eingehende Byte:
      String incomingData = Serial1.readStringUntil('&');

      if (start_stream == true)
      {
        int p1 = incomingData.indexOf("$");
        int p2 = incomingData.indexOf("$$", p1);
        int p3 = incomingData.indexOf("$$$", p2);
        int p4 = incomingData.indexOf("$$$$", p3);
        int p5 = incomingData.indexOf("%", p4);
        int p6 = incomingData.indexOf("/", p5);
        int p7 = incomingData.indexOf("//", p6);
        int p8 = incomingData.indexOf("///", p7);
        int p9 = incomingData.indexOf("////", p8);
        int p10 = incomingData.indexOf("§", p8);
        int p11 = incomingData.indexOf("§§", p8);
        int p12 = incomingData.indexOf("§§§", p8);
       // int p13 = incomingData.indexOf("+", p9);

        String Brightness_data = incomingData.substring(0, p1);
        String OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER_data = incomingData.substring(p1 + 1, p2);
        String OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER_data = incomingData.substring(p2 + 2, p3);
        String THRESHOLD_GRAY_data = incomingData.substring(p3 + 3, p4);
        String MIN_TRACK_WIDTH_data = incomingData.substring(p4 + 4, p5);
        String Red_data = incomingData.substring(p5 + 1, p6);
        String Blue_data = incomingData.substring(p6 + 1, p7);
        String Green_data = incomingData.substring(p7 + 2, p8);
        String Led_Brightness_data = incomingData.substring(p8 + 3, p9);
        String LEFT_DISTANCE_THRESHOLD_data = incomingData.substring(p9 + 4, p10);
        String RIGHT_DISTANCE_THRESHOLD_data = incomingData.substring(p10 + 2, p11);
        String ULTRASONIC_SAMPELS_data = incomingData.substring(p11 + 4, p12);
       // String BOOL_BIN_data = incomingData.substring(p12+6, p13);

        int Led_Brightness = 21 - Led_Brightness_data.toInt();
      data_eepromm[0] = Led_Brightness;
      int Brightness = Brightness_data.toInt();
      data_eepromm[1] = Brightness;
      int Red = 255 - Red_data.toInt() / Led_Brightness;
      data_eepromm[2] = Red;
      int Blue = 255 - Blue_data.toInt() / Led_Brightness;
      data_eepromm[3] = Blue;
      int Green = 255 - Green_data.toInt() / Led_Brightness;
      data_eepromm[4] = Green;

      OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER = OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER_data.toInt();

      data_eepromm[5] = OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;

      OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER = OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER_data.toInt();

      data_eepromm[6] = OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER;

      THRESHOLD_GRAY = THRESHOLD_GRAY_data.toInt();
      data_eepromm[7] = THRESHOLD_GRAY;

      MIN_TRACK_WIDTH = MIN_TRACK_WIDTH_data.toInt();
      data_eepromm[8] = THRESHOLD_GRAY;

      LEFT_DISTANCE_THRESHOLD = LEFT_DISTANCE_THRESHOLD_data.toInt();
      data_eepromm[9] = LEFT_DISTANCE_THRESHOLD;

      RIGHT_DISTANCE_THRESHOLD = RIGHT_DISTANCE_THRESHOLD_data.toInt();
      data_eepromm[10] = RIGHT_DISTANCE_THRESHOLD;

      ULTRASONIC_SAMPELS = ULTRASONIC_SAMPELS_data.toInt();
      data_eepromm[11] = ULTRASONIC_SAMPELS;

      for (int i = 0; i < ARRAY_EEPROMM_SIZE; i++)
      {
        EEPROM_write_read(i, data_eepromm[i], true);
      }

      set_LED(Red, Green, Blue);
      pixy.setCameraBrightness(Brightness);
      }
    }
  }
}
void write_i2c(String s) {
  debug(s + "\n");
  debug("distanceCmLeft: " + String(distanceCmLeft) + "\n");
  debug("distanceCmRight: " + String(distanceCmRight) + "\n");
  if (distanceCmRight < RIGHT_DISTANCE_THRESHOLD && distanceCmLeft < LEFT_DISTANCE_THRESHOLD /*&& distanceCmRight != 1000 && distanceCmLeft != 1000*/ /*|| BOOL_BIN == 1*/)
   { 
    s = "XXXXX";
    debug("XXXXX\n");
    //Serial.println(s);
   }
  char buffer[8];
  s.toCharArray(buffer, 8);
  //while (digitalRead(I2C_READY_PIN) == LOW);
  Wire.beginTransmission(I2C_ADDRESS_OF_SLAVE);
  Wire.write(buffer);
  Wire.endTransmission();
}
uint8_t EEPROM_write_read(int address, uint8_t data, bool read_write)
{
  int data_read = 0;
  if (read_write)
  {
    EEPROM.write(address, data);
    EEPROM.write(200, 230);

  } else if (!read_write)
  {
    return data_read = EEPROM.read(address);
  }
}
void generate_steer_angle_string(int steer_angle) {

  if((steer_angle >= 0 && steer_angle < 225) || (steer_angle <= 0 && steer_angle > -225)) {
    if(maybe_on_straight) {
      if(millis() - begin_of_steering_straight > 300) {
        currently_in_curve = false;
      }
    } else {
      begin_of_steering_straight = millis();
      maybe_on_straight = true;
    }
    steer_angle = steer_angle / 2;
  } else {
    maybe_on_straight = false;
    currently_in_curve = true;
  }

  if (steer_angle < 0) { // steer left
    //if(last_steering_direction == "right")
    //steer_angle = steer_angle / 2;

    if (steer_angle > -10) {
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
  }
  else if (steer_angle > 0) { // steer right
    //if(last_steering_direction == "left")
    //steer_angle = steer_angle / 2;

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
  } else { // straight
    current_angle_string = "S000S";
  }
}
/*volatile double durationMicroSec_RIGHT_isr = (micros() - durationMicroSec_RIGHT)/1000;
   Serial.println(durationMicroSec_RIGHT_isr);
   distanceCmRight = durationMicroSec_RIGHT_isr/58;*/
void Ultrasonic_LEFT_ISR()
{
  LEFT_counter++;
  if (LEFT_counter == 1)
  {
    durationMicroSec_LEFT_prev = micros();
  }
  if (LEFT_counter >= 2)
  {
    durationMicroSec_LEFT = micros();
    left_active = false;
    LEFT_counter = 0;
  }
}
void Ultrasonic_RIGHT_ISR()
{
  RIGHT_counter++;
  if (RIGHT_counter == 1)
  {
    durationMicroSec_RIGHT_prev = micros();
  }
  if (RIGHT_counter >= 2)
  {
    durationMicroSec_RIGHT = micros();
    left_active = true;
    RIGHT_counter = 0;
  }
}
void load_eeprom()
{
  if (EEPROM.read(200) == 230)
  {
    for (int i = 0; i < ARRAY_EEPROMM_SIZE; i++)
    {
      data_eepromm[i] = EEPROM_write_read(i, 0, false);
    }
    int Led_Brightness = data_eepromm[0];
    int Brightness = data_eepromm[1];
    int Red = data_eepromm[2];
    int Blue = data_eepromm[3];
    int Green = data_eepromm[4];

    OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER = data_eepromm[5];


    OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER = data_eepromm[6];


    THRESHOLD_GRAY = data_eepromm[7];


    MIN_TRACK_WIDTH = data_eepromm[8];


    LEFT_DISTANCE_THRESHOLD = data_eepromm[9] ;


    RIGHT_DISTANCE_THRESHOLD = data_eepromm[10];


    ULTRASONIC_SAMPELS = data_eepromm[11];

    set_LED(Red, Green, Blue);
    pixy.setCameraBrightness(Brightness);

  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  pinMode(PIN_BT, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(TRIGGER_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
  pinMode(TRIGGER_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);
  pinMode(PIN_LIGHT_SENSOR, INPUT);
  attachInterrupt(ECHO_PIN_RIGHT, Ultrasonic_RIGHT_ISR, CHANGE);
  attachInterrupt(ECHO_PIN_LEFT, Ultrasonic_LEFT_ISR, CHANGE);
  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug("Voltage");
  debug(String(voltage_of_battery) + "\n");
  if (voltage_of_battery < 6.25) {
    debug("voltage of battery too low on startup");
    digitalWrite(PIN_RELAY, LOW);
  } else {
    debug("turned on relay");
    digitalWrite(PIN_RELAY, HIGH);
  }
  load_eeprom();
  pixy.init();
  //Serial.println(pixy.changeProg("video"));
  current_lamp_status = 1;
  pixy.setLamp(current_lamp_status, current_lamp_status);
  //Serial.print("Turned on lamps...\n");

  Wire.begin(); // join i2c bus as master (address optional as master)
  //Wire.begin(8);                // join i2c bus with address #8
  //Wire.onReceive(receiveEvent); // register event

  pinMode(I2C_READY_PIN, INPUT);
  //attachInterrupt(I2C_READY_PIN, send_interpolated_string, RISING);
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);

  pinMode(PIN_LED_1, OUTPUT);
  //digitalWrite(PIN_LED_1, LOW);
  pinMode(PIN_LED_2, OUTPUT);
  //digitalWrite(PIN_LED_2, LOW);
  pinMode(PIN_LED_3, OUTPUT);
  //digitalWrite(PIN_LED_3, LOW);

  //Serial1.begin(38400); // Default communication rate of the Bluetooth module
  Serial1.begin(460800);
}

int return_map(int value_to_map, int in_min, int in_max, int out_min, int out_max) {
  return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int calculate_pull_towards_ideallinie_in_degrees(int distance_from_ideallinie) {
  //int angle = return_map(distance_from_ideallinie, 0, 20, 0, 450);
  debug("distance_from_ideallinie: " + String(distance_from_ideallinie) + "\n");
  int angle = return_map(distance_from_ideallinie, 0, 70, 0, 1350);
  if (angle > 450)
    angle = 450;

  if (distance_from_ideallinie <= 5)
    angle = angle / 2;

  return angle;
}

uint8_t count_lines(uint8_t array[63]) {
  uint8_t number_of_lines = 0;
  bool currently_in_line = false;
  int index_of_last_line = -MIN_TRACK_WIDTH - 5;

  for (int i = 0; i < 63; i++) {
    if (!currently_in_line && array[i] == 0 && i > index_of_last_line + MIN_TRACK_WIDTH) {
      number_of_lines++;
      currently_in_line = true;
    }
    if ( (array[i] == 1 || array[i] == 255) && currently_in_line) {
      currently_in_line = false;
      index_of_last_line = i;
    }

  }
  return number_of_lines;
}

void update_indexes_of_lines(uint8_t array[63]) {
  bool left_line_found = false;
  bool right_line_found = false;
  bool first_one_found = false;

  for (int i = 32; i >= 0; i--) {
    if (array[i] == 1 || array[i] == 255)
      first_one_found = true;
    if (array[i] == 0 && first_one_found) {
      index_of_left_line = i;
      left_line_found = true;
      break;
    }
  }
  if (!left_line_found) {
    for (int i = 32; i >= 0; i--) {
      if (array[i] == 0) {
        index_of_left_line = i;
        left_line_found = true;
        break;
      }
    }
  }

  first_one_found = false;

  for (int i = 32; i < 63; i++) {
    if (array[i] == 1 || array[i] == 255)
      first_one_found = true;
    if (array[i] == 0 && first_one_found) {
      index_of_right_line = i;
      right_line_found = true;
      break;
    }
  }
  if (!right_line_found) {
    for (int i = 32; i < 63; i++) {
      if (array[i] == 0) {
        index_of_right_line = i;
        right_line_found = true;
        break;
      }
    }
  }
}

void connect_line_edges(uint8_t array[], uint8_t size) {
  bool line_edge_found = false;
  uint8_t line_edge_index = 0;
  uint8_t line_width_counter = 0;


  for(int i = 0; i < size; i++) {
    if(array[i] < THRESHOLD_DIFFERENCE && !line_edge_found)
      array[i] = 255;
    else {
      if(array[i] > THRESHOLD_DIFFERENCE && !line_edge_found) {
        array[i] = 0;
        line_edge_found = true;
        line_width_counter = 0;
        line_edge_index = i;
      }

      if(line_edge_found && ((array[i] > THRESHOLD_DIFFERENCE && array[i] < 255)) && i > line_edge_index) {
        for(int j = line_edge_index; j < i; j++)
          array[j] = array[line_edge_index];
        array[i] = 0;
      }
    }

    if(line_edge_found) {
      line_width_counter++;
    }

    if(line_width_counter > 4)
      line_edge_found = false;

  }
}

void detect_if_left_or_right_line(uint8_t array[63]) {
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

}
void set_LED(uint8_t R, uint8_t G, uint8_t B)
{
  analogWrite(PIN_LED_2, R);
  analogWrite(PIN_LED_3, G);
  analogWrite(PIN_LED_1, B);
}
void Ultrasonic_RIGHT()
{

  if (left_active == true)
  {
    digitalWrite(TRIGGER_PIN_LEFT, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIGGER_PIN_LEFT, LOW);
  }
  distanceCmRight = abs(durationMicroSec_RIGHT - durationMicroSec_RIGHT_prev) / 2.0 * 0.0343;
  //if (distanceCmRight == 0 || distanceCmRight > 400) {
  if (distanceCmRight < 1 || distanceCmRight > 400) {
    distanceCmRight = 1000;
  }
}
void Ultrasonic_LEFT()
{

  if (left_active == false)
  {
    digitalWrite(TRIGGER_PIN_RIGHT, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIGGER_PIN_RIGHT, LOW);
  }
  distanceCmLeft = abs(durationMicroSec_LEFT - durationMicroSec_LEFT_prev) / 2.0 * 0.0343;
  //if (distanceCmLeft == 0 || distanceCmLeft > 400) {
  if (distanceCmLeft < 1 || distanceCmLeft > 400) {
    distanceCmLeft = 1000;
  }
}
void loop() {

  debug("light sensor value: " + String(analogRead(PIN_LIGHT_SENSOR)) + "\n");

  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug("Voltage ");
  debug(String(voltage_of_battery) + "\n");
  if (voltage_of_battery < 6.25) {
    battery_low_counter++;
    if (battery_low_counter > 500) {
      debug("voltage of battery too low");
      digitalWrite(PIN_RELAY, LOW);
      return;
    }
  }
  Ultrasonic_RIGHT();
  // put your main code here, to run repeatedly:
  uint8_t r, g, b;
  uint8_t gray;
  //debug(String(millis()) + "\n");

  line_25_until_border = 0;
  line_75_until_border = 0;
  line_125_until_border = 0;
  line_175_until_border = 0;
  //int look_towards_right_or_left = 0;


  for (int i = 0, j = 0; i < 315; i = i + 5, j++) {


    if (digitalRead(PIN_BT) == HIGH) {
      pixy.video.getRGB(i, 25, &r, &g, &b, false);
      gray = 0.299 * r + 0.587 * g + 0.114 * b;
      if (DEBUG_GRAY_VALUES)
        rgb_25[j] = gray;
      if (gray > THRESHOLD_GRAY)
        black_or_white_at_25[j] = 1;
      else
        black_or_white_at_25[j] = 0;

      //debug(String(gray) + "\n");
    }


    pixy.video.getRGB(i, 75, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if (DEBUG_GRAY_VALUES) {
      rgb_75[j] = gray;

      if(j >= 1) {
        differences_75[j - 1] = abs(rgb_75[j - 1] - rgb_75[j]);
      }
    }
    if (gray > THRESHOLD_GRAY)
      black_or_white_at_75[j] = 1;
    else
      black_or_white_at_75[j] = 0;

    //debug(String(gray) + "\n");


    if (digitalRead(PIN_BT) == HIGH) {
      pixy.video.getRGB(i, 125, &r, &g, &b, false);
      gray = 0.299 * r + 0.587 * g + 0.114 * b;
      if (DEBUG_GRAY_VALUES)
        rgb_125[j] = gray;
      if (gray > THRESHOLD_GRAY)
        black_or_white_at_125[j] = 1;
      else
        black_or_white_at_125[j] = 0;

    }


    pixy.video.getRGB(i, 175, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if (DEBUG_GRAY_VALUES)
      rgb_175[j] = gray;
    if (gray > THRESHOLD_GRAY)
      black_or_white_at_175[j] = 1;
    else
      black_or_white_at_175[j] = 0;
  }
  differences_75[62] = 255;
  differences_175[62] = 255;
  connect_line_edges(differences_75, 62);
  connect_line_edges(differences_175, 62);


  uint8_t number_of_lines = count_lines(differences_75);
  debug("number of lines: " + String(number_of_lines) + "\n");
  if (number_of_lines == 2) {
    currently_seeing_both_lines = true;
    currently_seeing_only_left_line = false;
    currently_seeing_only_right_line = false;
    update_indexes_of_lines(differences_75);
    debug("index_of_left_line: " + String(index_of_left_line) + "\n");
    debug("index_of_right_line: " + String(index_of_right_line) + "\n");
  } else if (number_of_lines == 1) {
    detect_if_left_or_right_line(differences_75);
  } else if (number_of_lines == 0) {
    currently_seeing_both_lines = false;
    currently_seeing_only_left_line = false;
    currently_seeing_only_right_line = false;
  }

  int center = 0;
  if (currently_seeing_both_lines) {
    debug("currently seeing both lines!\n");
    center = (index_of_left_line + index_of_right_line) / 2;
  }
  if (currently_seeing_only_left_line) {
    debug("currently seeing only left line\n");
    debug("index_of_left_line: " + String(index_of_left_line) + "\n");
    center = index_of_left_line + OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;
    index_of_right_line = index_of_left_line + 2 * OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;
    /*if (index_of_left_line > 55)
      index_of_right_line = 66;
    else
      index_of_right_line = 62;*/
  }
  if (currently_seeing_only_right_line) {
    debug("currently seeing only right line\n");
    debug("index_of_right_line: " + String(index_of_right_line) + "\n");
    center = index_of_right_line - OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;
    index_of_left_line = index_of_right_line - 2 * OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;
    /*if (index_of_right_line < 7)
      index_of_left_line = -5;
    else
      index_of_left_line = 0;*/
  }



  if (number_of_lines > 0) {
    int steer_angle = calculate_pull_towards_ideallinie_in_degrees(abs(center - 32));
    if (center >= 32) {
      debug("steer right: " + String(steer_angle) + "\n");
    }
    else {
      debug("steer left: " + String(steer_angle) + "\n");
      steer_angle = steer_angle * -1;
    }


    //if((last_steer_angle > 400 && steer_angle < 100 && steer_angle > 0) || (last_steer_angle < -400 && steer_angle > -100 && steer_angle < 0))
    //steer_angle = steer_angle * 4;


    if (steer_angle > 450)
      steer_angle = 450;
    else if (steer_angle < -450)
      steer_angle = -450;


    generate_steer_angle_string(steer_angle);

    write_i2c(current_angle_string);
    last_steer_angle = steer_angle;
  } else {
    debug("did not find a line at 75, now looking at 175\n");
    /*for (int i = 0; i < 63; i++) {
      debug(String(black_or_white_at_175[i]));
    } debug("\n");*/

    uint8_t number_of_lines_175 = count_lines(differences_175);
    debug("number of lines: " + String(number_of_lines_175) + "\n");
    if (number_of_lines_175 == 2) {
      currently_seeing_both_lines = true;
      currently_seeing_only_left_line = false;
      currently_seeing_only_right_line = false;
      update_indexes_of_lines(differences_175);
      debug("index_of_left_line: " + String(index_of_left_line) + "\n");
      debug("index_of_right_line: " + String(index_of_right_line) + "\n");
    } else if (number_of_lines_175 == 1) {
      detect_if_left_or_right_line(differences_175);
    } else if (number_of_lines_175 == 0) {
      currently_seeing_both_lines = false;
      currently_seeing_only_left_line = false;
      currently_seeing_only_right_line = false;
    }

    int center = 0;
    if (currently_seeing_both_lines) {
      debug("currently seeing both lines!\n");
      center = (index_of_left_line + index_of_right_line) / 2;
    }
    if (currently_seeing_only_left_line) {
      debug("currently seeing only left line\n");
      debug("index_of_left_line: " + String(index_of_left_line) + "\n");
      center = index_of_left_line + OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER;
      index_of_right_line = index_of_left_line + 2 * OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER;
    }
    if (currently_seeing_only_right_line) {
      debug("currently seeing only right line\n");
      debug("index_of_right_line: " + String(index_of_right_line) + "\n");
      center = index_of_right_line - OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER;
      index_of_left_line = index_of_right_line - 2 * OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER;
    }

    if (number_of_lines_175 > 0) {
      int steer_angle_175 = calculate_pull_towards_ideallinie_in_degrees(abs(center - 32));
      if (center >= 32) {
        debug("steer right: " + String(steer_angle_175) + "\n");
      }
      else {
        debug("steer left: " + String(steer_angle_175) + "\n");
        steer_angle_175 = steer_angle_175 * -1;
      }

      steer_angle_175 = steer_angle_175 * 2;
      //if((last_steer_angle > 400 && steer_angle_175 < 100 && steer_angle_175 > 0) || (last_steer_angle < -400 && steer_angle_175 > -100 && steer_angle_175 < 0))
      //steer_angle_175 = steer_angle_175 * 4;

      if (steer_angle_175 > 450)
        steer_angle_175 = 450;
      else if (steer_angle_175 < -450)
        steer_angle_175 = -450;

      generate_steer_angle_string(steer_angle_175);

      write_i2c(current_angle_string);
      last_steer_angle = steer_angle_175;
    }
    else { // write last steer angle
      //debug("seeing no line, sending last angle string\n");
      write_i2c(current_angle_string);
    }
  }
  if (DEBUG_GRAY_VALUES)
  {
    String s_25;
    String s_75;
    String s_125;
    String s_175;
    String US_data;
    //debug("\n");


    for (int i = 0; i < 63; i++) {
      //debug(String(black_or_white_at_75[i]));
      
      s_25 += String(rgb_25[i]) + " ";
      //if(i == 62)
        //s_25 += "255 ";
      //else
        //s_25 += String(differences_75[i]) + " ";
      s_75 += String(rgb_75[i]) + " ";
      s_125 += String(rgb_125[i]) + " ";
      s_175 += String(rgb_175[i]) + " ";
    }
    US_data += String(abs(distanceCmLeft), DEC);
    US_data += " ";
    US_data += String(abs(distanceCmRight), DEC);
    US_data += " ";
    String mainString = (" # " + s_25 + " $ " + s_75 + " $$ " + s_125 + " $$$ " + s_175 + " $$$$ " + US_data + " / " + "\n" + "&");
    debug(mainString);
    /*debug(" # ");
      debug(s_25);
      debug(" $ ");
      debug(s_75);
      debug(" $$ ");
      debug(s_125);
      debug(" $$$ ");
      debug(s_175);
      debug(" $$$$ ");
      debug(US_data);
      debug(" / ");
      debug("\n");
      debug("&");*/

  }
 
  Serial.println(millis());
  Ultrasonic_LEFT();
  Serial_receive();
}
