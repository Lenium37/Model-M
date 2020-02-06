#include <Pixy2.h>
#include <Wire.h> // I2Cs

#define THRESHOLD_GRAY 150
#define THRESHOLD_DIFFERENCE 120
#define I2C_ADDRESS_OF_SLAVE 8
#define I2C_READY_PIN 3
#define PIN_RELAY 6
#define PIN_LED_1 7
#define PIN_LED_2 8
#define PIN_LED_3 9
#define PIN_BATTERY_VOLTAGE 15
#define OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER 25
#define OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER 30
#define MIN_TRACK_WIDTH 30
#define DEBUG_GRAY_VALUES false
#define DEBUG_PRINTS true


Pixy2 pixy;
bool current_lamp_status = 0;
uint8_t black_or_white_at_25[63];
uint8_t black_or_white_at_75[63];
uint8_t black_or_white_at_125[63];
uint8_t black_or_white_at_175[63];
uint8_t rgb_25[63];
uint8_t rgb_75[63];
uint8_t rgb_125[63];
uint8_t rgb_175[63];
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

bool currently_seeing_both_lines = false;
bool currently_seeing_only_left_line = false;
bool currently_seeing_only_right_line = false;

void debug(String s) {
  if(DEBUG_PRINTS) {
    Serial1.print(s);
    // if changing Serial number don't forget to change it in setup() as well!
  }
}

void write_i2c(String s) {
  debug(s + "\n");
  char buffer[5];
  s.toCharArray(buffer, 5);
  while(digitalRead(I2C_READY_PIN) == LOW);
  Wire.beginTransmission(I2C_ADDRESS_OF_SLAVE);
  Wire.write(buffer);
  Wire.endTransmission();
}

void generate_steer_angle_string(int steer_angle) {
  if(steer_angle < 0) { // steer left
    //if(last_steering_direction == "right")
      //steer_angle = steer_angle / 2;
      
    if(steer_angle > -10) {
      if(currently_in_curve)
        current_angle_string = "L00"+String(abs(steer_angle))+"C";
      else
        current_angle_string = "L00"+String(abs(steer_angle))+"S";
    } else if(steer_angle > -100) {
      if(currently_in_curve)
        current_angle_string = "L0"+String(abs(steer_angle))+"C";
      else
        current_angle_string = "L0"+String(abs(steer_angle))+"S";
    } else {
      if(currently_in_curve)
        current_angle_string = "L"+String(abs(steer_angle))+"C";
      else
        current_angle_string = "L"+String(abs(steer_angle))+"S";
    }
  }
  else if(steer_angle > 0) { // steer right      
    //if(last_steering_direction == "left")
      //steer_angle = steer_angle / 2;
      
    if(steer_angle < 10) {
      if(currently_in_curve)
        current_angle_string = "R00"+String(steer_angle)+"C";
      else
        current_angle_string = "R00"+String(steer_angle)+"S";
    } else if(steer_angle < 100) {
      if(currently_in_curve)
        current_angle_string = "R0"+String(steer_angle)+"C";
      else
        current_angle_string = "R0"+String(steer_angle)+"S";
    } else {
      if(currently_in_curve)
        current_angle_string = "R"+String(steer_angle)+"C";
      else
        current_angle_string = "R"+String(steer_angle)+"S";
    }
  } else { // straight
    current_angle_string = "S000S";
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  
  pinMode(PIN_RELAY, OUTPUT);
  
  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  //debug(String(voltage_of_battery)+"\n");
  if(voltage_of_battery < 6.25) {
    //debug("voltage of battery too low on startup");
    //digitalWrite(PIN_RELAY, LOW);
  } else {
    //debug("turned on relay");
    digitalWrite(PIN_RELAY, HIGH);
  }

  pixy.init();
  Serial.println(pixy.changeProg("video"));
  current_lamp_status = 0;
  pixy.setLamp(current_lamp_status, current_lamp_status);
  Serial.print("Turned on lamps...\n");

  Wire.begin(); // join i2c bus as master (address optional as master)
  //Wire.begin(8);                // join i2c bus with address #8
  //Wire.onReceive(receiveEvent); // register event

  pinMode(I2C_READY_PIN, INPUT);
  //attachInterrupt(I2C_READY_PIN, send_interpolated_string, RISING);
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);
  
  pinMode(PIN_LED_1, OUTPUT);
  digitalWrite(PIN_LED_1, LOW);
  pinMode(PIN_LED_2, OUTPUT);
  digitalWrite(PIN_LED_2, LOW);
  pinMode(PIN_LED_3, OUTPUT);
  digitalWrite(PIN_LED_3, LOW);
  
  Serial1.begin(38400); // Default communication rate of the Bluetooth module
}

int return_map(int value_to_map, int in_min, int in_max, int out_min, int out_max) {
  return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int calculate_pull_towards_ideallinie_in_degrees(int distance_from_ideallinie) {
  //int angle = return_map(distance_from_ideallinie, 0, 20, 0, 450);
  debug("distance_from_ideallinie: " + String(distance_from_ideallinie) + "\n");
  int angle = return_map(distance_from_ideallinie, 0, 70, 0, 1350);
  if(angle > 450)
    angle = 450;

  if(distance_from_ideallinie <= 5)
    angle = angle / 2;

  return angle;
}

uint8_t count_lines(uint8_t array[63], bool start_at_left_line, bool end_at_right_line) {
  uint8_t number_of_lines = 0;
  bool currently_in_line = false;
  int index_of_last_line = -MIN_TRACK_WIDTH - 5;
  int start = 0;
  if(start_at_left_line)
    start = index_of_left_line - 3;
  if(start < 0)
    start = 0;
  int end = 63;
  if(end_at_right_line)
    end = index_of_right_line + 4;
  if(end > 63)
    end = 63;

  //for(int i = start; i < end; i++) {
  for(int i = 0; i < 63; i++) {
    if(!currently_in_line && array[i] == 0 && i > index_of_last_line + MIN_TRACK_WIDTH) {
      number_of_lines++;
      currently_in_line = true;
    }
    if(array[i] == 1 && currently_in_line) {
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

  for(int i = 32; i >= 0; i--) {
    if(array[i] == 1)
      first_one_found = true;
    if(array[i] == 0 && first_one_found) {
      index_of_left_line = i;
      left_line_found = true;
      break;
    }
  }
  if(!left_line_found) {
    for(int i = 32; i >= 0; i--) {
      if(array[i] == 0) {
        index_of_left_line = i;
        left_line_found = true;
        break;
      }
    } 
  }

  first_one_found = false;

  for(int i = 32; i < 63; i++) {
    if(array[i] == 1)
      first_one_found = true;
    if(array[i] == 0 && first_one_found) {
      index_of_right_line = i;
      right_line_found = true;
      break;
    }
  }
  if(!right_line_found) {
    for(int i = 32; i < 63; i++) {
      if(array[i] == 0) {
        index_of_right_line = i;
        right_line_found = true;
        break;
      }
    } 
  }
}

void detect_if_left_or_right_line(uint8_t array[63]) {
  if(index_of_left_line == index_of_right_line) {
    index_of_left_line -= 1;
    index_of_right_line += 1;
    debug("index of left = index of right");
  }

  bool line_found = false;
  int index_of_line = 0;
  for(int i = 0; i < 63; i++) {
    if(array[i] == 0) {
      line_found = true;
      if(i > 32) {
        index_of_line = i;
        break;
      }
    }
    if(line_found && array[i] == 1) {
      index_of_line = i;
      break;
    }
  }


  /*if(last_steer_angle > 0) {
    index_of_left_line += 10;
    index_of_right_line += 10;
  } else if(last_steer_angle < 0) {
    index_of_left_line -= 10;
    index_of_right_line -= 10;
  }*/


  int distance_from_left_index = abs(index_of_left_line - index_of_line);
  int distance_from_right_index = abs(index_of_right_line - index_of_line);
  if(distance_from_right_index <= distance_from_left_index) {
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

void loop() {


  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug(String(voltage_of_battery) + "\n");
  if(voltage_of_battery < 6.25) {
    battery_low_counter++;
    if(battery_low_counter > 500) {
      //debug("voltage of battery too low");
      //digitalWrite(PIN_RELAY, LOW);
      //return;
    }
  }
  
  // put your main code here, to run repeatedly:
  uint8_t r, g, b;
  uint8_t gray;
  debug(String(millis()) + "\n");

  line_25_until_border = 0;
  line_75_until_border = 0;
  line_125_until_border = 0;
  line_175_until_border = 0;
  int look_towards_right_or_left = 0;

  
  for(int i = 0, j = 0; i < 315; i = i + 5, j++) {
    /*
    pixy.video.getRGB(i, 25, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if(DEBUG_GRAY_VALUES)
      rgb_25[j] = gray;
    if(gray > THRESHOLD_GRAY)
      black_or_white_at_25[j] = 1;
    else
      black_or_white_at_25[j] = 0;*/

    //debug(String(gray) + "\n");


    pixy.video.getRGB(i, 75, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    rgb_75[j] = gray;
    if(gray > THRESHOLD_GRAY)
      black_or_white_at_75[j] = 1;
    else
      black_or_white_at_75[j] = 0;
    
    //debug(String(gray) + "\n");

    /*pixy.video.getRGB(i, 125, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if(DEBUG_GRAY_VALUES)
      rgb_125[j] = gray;
    if(gray > THRESHOLD_GRAY)
      black_or_white_at_125[j] = 1;
    else
      black_or_white_at_125[j] = 0;*/


    pixy.video.getRGB(i, 175, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    rgb_175[j] = gray;
    if(gray > THRESHOLD_GRAY)
      black_or_white_at_175[j] = 1;
    else
      black_or_white_at_175[j] = 0;
  }

  /*for(int i = 0; i < 63; i++) {
    if(i == 0) {
      if(abs(rgb_75[i] - rgb_75[i+1]) > THRESHOLD_DIFFERENCE)
        black_or_white_at_75[i] = 0;
      else
        black_or_white_at_75[i] = 1;

      if(abs(rgb_175[i] - rgb_175[i+1]) > THRESHOLD_DIFFERENCE)
        black_or_white_at_175[i] = 0;
      else
        black_or_white_at_175[i] = 1;
    } else if(i == 62) {
      if(abs(rgb_75[i] - rgb_75[i-1]) > THRESHOLD_DIFFERENCE)
        black_or_white_at_75[i] = 0;
      else
        black_or_white_at_75[i] = 1;

      if(abs(rgb_175[i] - rgb_175[i-1]) > THRESHOLD_DIFFERENCE)
        black_or_white_at_175[i] = 0;
      else
        black_or_white_at_175[i] = 1;
    } else {
      if((abs(rgb_75[i] - rgb_75[i+1]) > THRESHOLD_DIFFERENCE) || abs(rgb_75[i] - rgb_75[i-1]) > THRESHOLD_DIFFERENCE)
        black_or_white_at_75[i] = 0;
      else
        black_or_white_at_75[i] = 1;

      if((abs(rgb_175[i] - rgb_175[i+1]) > THRESHOLD_DIFFERENCE) || abs(rgb_175[i] - rgb_175[i-1]) > THRESHOLD_DIFFERENCE)
        black_or_white_at_175[i] = 0;
      else
        black_or_white_at_175[i] = 1;
    }
  }*/


  for(int i = 0; i < 63; i++) {
    debug(String(black_or_white_at_75[i]));
  } debug("\n");

  uint8_t number_of_lines = 0;
  if(currently_seeing_only_left_line)
    number_of_lines = count_lines(black_or_white_at_75, true, false);
  else if(currently_seeing_only_right_line)
    number_of_lines = count_lines(black_or_white_at_75, false, true);
  else
    number_of_lines = count_lines(black_or_white_at_75, false, false);

  debug("number of lines: " + String(number_of_lines) + "\n");
  if(number_of_lines == 2) {
    currently_seeing_both_lines = true;
    currently_seeing_only_left_line = false;
    currently_seeing_only_right_line = false;
    update_indexes_of_lines(black_or_white_at_75);
    debug("index_of_left_line: " + String(index_of_left_line) + "\n");
    debug("index_of_right_line: " + String(index_of_right_line) + "\n");
  } else if(number_of_lines == 1) {
    detect_if_left_or_right_line(black_or_white_at_75);
  } else if(number_of_lines == 0) {
    currently_seeing_both_lines = false;
    currently_seeing_only_left_line = false;
    currently_seeing_only_right_line = false;
  }

  int center = 0;
  if(currently_seeing_both_lines) {
    debug("currently seeing both lines!\n");
    center = (index_of_left_line + index_of_right_line) / 2;
  }
  if(currently_seeing_only_left_line) {
    debug("currently seeing only left line\n");
        debug("index_of_left_line: " + String(index_of_left_line) + "\n");
    center = index_of_left_line + OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;
    if(index_of_left_line > 40)
      index_of_right_line = 66;
    else
      index_of_right_line = 62;
  }
  if(currently_seeing_only_right_line) {
    debug("currently seeing only right line\n");
        debug("index_of_right_line: " + String(index_of_right_line) + "\n");
    center = index_of_right_line - OFFSET_FROM_ONE_LINE_AT_75_TO_CENTER;
    if(index_of_right_line < 22)
      index_of_left_line = -5;
    else
      index_of_left_line = 0;
  }



  if(number_of_lines > 0) {
    int steer_angle = calculate_pull_towards_ideallinie_in_degrees(abs(center - 32));
    if(center >= 32) {
      debug("steer right: " + String(steer_angle) + "\n");
    }
    else {
      debug("steer left: " + String(steer_angle) + "\n");
      steer_angle = steer_angle * -1;
    }


    //if((last_steer_angle > 400 && steer_angle < 100 && steer_angle > 0) || (last_steer_angle < -400 && steer_angle > -100 && steer_angle < 0))
      //steer_angle = steer_angle * 4;


    if(steer_angle > 450)
      steer_angle = 450;
    else if(steer_angle < -450)
      steer_angle = -450;


    generate_steer_angle_string(steer_angle);

    write_i2c(current_angle_string);
    debug("\n");
    debug("\n");
    last_steer_angle = steer_angle;
  } else {
      debug("did not find a line at 75, now looking at 175\n");
      for(int i = 0; i < 63; i++) {
        debug(String(black_or_white_at_175[i]));
      } debug("\n");

      uint8_t number_of_lines_175 = 0;
      if(currently_seeing_only_left_line)
          number_of_lines_175 = count_lines(black_or_white_at_175, true, false);
        else if(currently_seeing_only_right_line)
          number_of_lines_175 = count_lines(black_or_white_at_175, false, true);
        else
          number_of_lines_175 = count_lines(black_or_white_at_175, false, false);
        
      debug("number of lines: " + String(number_of_lines_175) + "\n");

      if(number_of_lines_175 == 2) {
        currently_seeing_both_lines = true;
        currently_seeing_only_left_line = false;
        currently_seeing_only_right_line = false;
        update_indexes_of_lines(black_or_white_at_175);
        debug("index_of_left_line: " + String(index_of_left_line) + "\n");
        debug("index_of_right_line: " + String(index_of_right_line) + "\n");
      } else if(number_of_lines_175 == 1) {
        detect_if_left_or_right_line(black_or_white_at_175);
      } else if(number_of_lines_175 == 0) {
        currently_seeing_both_lines = false;
        currently_seeing_only_left_line = false;
        currently_seeing_only_right_line = false;
      }

      int center = 0;
      if(currently_seeing_both_lines) {
        debug("currently seeing both lines!\n");
        center = (index_of_left_line + index_of_right_line) / 2;
      }
      if(currently_seeing_only_left_line) {
        debug("currently seeing only left line\n");
        debug("index_of_left_line: " + String(index_of_left_line) + "\n");
        center = index_of_left_line + OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER;
        if(index_of_left_line > 40)
          index_of_right_line = 66;
        else
          index_of_right_line = 62;
      }
      if(currently_seeing_only_right_line) {
        debug("currently seeing only right line\n");
        debug("index_of_right_line: " + String(index_of_right_line) + "\n");
        center = index_of_right_line - OFFSET_FROM_ONE_LINE_AT_175_TO_CENTER;
        if(index_of_right_line < 22)
          index_of_left_line = -5;
        else
          index_of_left_line = 0;
      }

      if(number_of_lines_175 > 0) {
        int steer_angle_175 = calculate_pull_towards_ideallinie_in_degrees(abs(center - 32));
        if(center >= 32) {
          debug("steer right: " + String(steer_angle_175) + "\n");
        }
        else {
          debug("steer left: " + String(steer_angle_175) + "\n");
          steer_angle_175 = steer_angle_175 * -1;
        }

        if((steer_angle_175 > 0 && last_steer_angle > 0) || (steer_angle_175 < 0 && last_steer_angle < 0))
          steer_angle_175 = last_steer_angle;
        else
          steer_angle_175 = steer_angle_175 * 2;
        //if((last_steer_angle > 400 && steer_angle_175 < 100 && steer_angle_175 > 0) || (last_steer_angle < -400 && steer_angle_175 > -100 && steer_angle_175 < 0))
          //steer_angle_175 = steer_angle_175 * 4;

        if(steer_angle_175 > 450)
          steer_angle_175 = 450;
        else if(steer_angle_175 < -450)
          steer_angle_175 = -450;

        generate_steer_angle_string(steer_angle_175);

        write_i2c(current_angle_string);
        debug("\n");
        debug("\n");
        last_steer_angle = steer_angle_175;
      }
      else { // write last steer angle
        //debug("seeing no line, sending last angle string\n");
        write_i2c(current_angle_string);
        currently_seeing_only_left_line = false;
        currently_seeing_only_right_line = false;
        debug("\n");
        debug("\n");
      }
  }


  if(DEBUG_GRAY_VALUES) {
    debug("##");
    String s_25;
    String s_75;
    String s_125;
    String s_175;

    //debug("\n");
    for(int i = 0; i < 63; i++) {
      //debug(String(black_or_white_at_75[i]));
      s_25 += String(rgb_25[i]) + " ";
      s_75 += String(rgb_75[i]) + " ";
      s_125 += String(rgb_125[i]) + " ";
      s_175 += String(rgb_175[i]) + " ";
      if(i == 62) {
        s_25.setCharAt(s_25.length() - 1, '\0');
        s_75.setCharAt(s_75.length() - 1, '\0');
        s_125.setCharAt(s_125.length() - 1, '\0');
        s_175.setCharAt(s_175.length() - 1, '\0');
      }
    }
    debug(s_25);
    debug("$");
    debug(s_75);
    debug("$$");
    debug(s_125);
    debug("$$$");
    debug(s_175);
    debug("$$$$");
    debug("&");
  }
}
