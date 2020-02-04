#include <Pixy2.h>
#include <Wire.h> // I2Cs

#define THRESHOLD_GRAY 45
#define MIN_TRACK_WIDTH 35
#define I2C_ADDRESS_OF_SLAVE 8
#define I2C_READY_PIN 3
#define PIN_RELAY 6
#define PIN_LED_1 7
#define PIN_LED_2 8
#define PIN_LED_3 9
#define PIN_BATTERY_VOLTAGE 15


Pixy2 pixy;
bool current_lamp_status = 0;
uint8_t rgb_at_y_50[63];
uint8_t rgb_at_y_100[63];
uint8_t rgb_at_y_150[63];
uint8_t rgb_at_y_200[63];
int line_50_until_border = 0;
int line_100_until_border = 0;
int line_150_until_border = 0;
int line_200_until_border = 0;
bool currently_in_curve = false;
String current_angle_string;
float voltage_of_battery = 10;
int battery_low_counter = 0;

void debug(String s) {
  Serial.print(s);
  // if changing Serial number don't forget to change it in setup() as well!
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  
  pinMode(PIN_RELAY, OUTPUT);
  
  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug(String(voltage_of_battery)+"\n");
  if(voltage_of_battery < 6.25) {
    //debug("voltage of battery too low on startup");
    //digitalWrite(PIN_RELAY, LOW);
  } else {
    debug("turned on relay");
    digitalWrite(PIN_RELAY, HIGH);
  }

  pixy.init();
  Serial.println(pixy.changeProg("video"));
  current_lamp_status = 1;
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
  int angle = return_map(distance_from_ideallinie, 0, 20, 0, 450);
  if(angle > 450)
    angle = 450;

  if(distance_from_ideallinie <= 5)
    angle = angle / 2;

  return angle;
}

/*uint8_t center_of_drivable_space(uint8_t camera_line[63], int &track_till_border, int prefer_right_or_left) {

  bool currently_in_track = false;
  uint8_t index_of_current_track_begin = 0;
  uint8_t index_of_longest_track_begin = 0;
  uint8_t width_of_current_track = 0;
  uint8_t width_of_longest_track = 0;
  uint8_t center = 0;
  track_till_border = 0;
  
  for(int i = 0; i < 63; i++) {
    if(camera_line[i] == 1 && i < 62) {
      if(!currently_in_track) {
        index_of_current_track_begin = i;
        currently_in_track = true;
      }
        
      width_of_current_track++;
    } else if(camera_line[i] == 0 || i == 62) {
      currently_in_track = false;
      if(width_of_current_track > 0) {
        if(width_of_current_track > width_of_longest_track && width_of_current_track > MIN_TRACK_WIDTH) {
          width_of_longest_track = width_of_current_track;
          index_of_longest_track_begin = index_of_current_track_begin;
        }

        width_of_current_track = 0;
        if(i == 62)
          track_till_border = 1;
      }
    }
  }
  if(index_of_longest_track_begin == 0) {
    track_till_border = -1;
  }
  
  center = index_of_longest_track_begin + width_of_longest_track / 2;
  
  //debug("index_of_current_track_begin: " + String(index_of_current_track_begin) + "\n");
  debug("index_of_longest_track_begin: " + String(index_of_longest_track_begin) + "\n");
  //debug("width_of_current_track: " + String(width_of_current_track) + "\n");
  debug("width_of_longest_track: " + String(width_of_longest_track) + "\n");
  debug("center: " + String(center) + "\n");




  // no track
  if(center == 0)
    track_till_border = 0;

  //track_till_border = i;
    
  return center;
}*/


uint8_t center_of_drivable_space(uint8_t camera_line[63], int &track_till_border, int prefer_right_or_left) {
  bool currently_in_track = false;
  uint8_t index_of_current_track_begin = 0;
  uint8_t index_of_longest_track_begin = 0;
  uint8_t width_of_current_track = 0;
  uint8_t width_of_longest_track = 0;
  uint8_t center = 0;
  track_till_border = 0;
  
  if(prefer_right_or_left == -1) {
    for(int i = 0; i < 63; i++) {
      if(camera_line[i] == 1 && i < 62) {
        if(!currently_in_track) {
          index_of_current_track_begin = i;
          currently_in_track = true;
        }
        
        width_of_current_track++;
      } else if(camera_line[i] == 0 || i == 62) {
        currently_in_track = false;
        if(width_of_current_track > MIN_TRACK_WIDTH) {
          if(i == 62)
            track_till_border = 1;

          width_of_longest_track = width_of_current_track;
          index_of_longest_track_begin = index_of_current_track_begin;
          break;
        }
        width_of_current_track = 0;
      }
    }

    if(index_of_longest_track_begin == 0) {
      track_till_border = -1;
    }

    center = index_of_longest_track_begin + width_of_longest_track / 2;
  } else if(prefer_right_or_left == 0) {
    for(int i = 0; i < 63; i++) {
      if(camera_line[i] == 1 && i < 62) {
        if(!currently_in_track) {
          index_of_current_track_begin = i;
          currently_in_track = true;
        }
          
        width_of_current_track++;
      } else if(camera_line[i] == 0 || i == 62) {
        currently_in_track = false;
        if(width_of_current_track > 0) {
          if(width_of_current_track > width_of_longest_track && width_of_current_track > MIN_TRACK_WIDTH) {
            width_of_longest_track = width_of_current_track;
            index_of_longest_track_begin = index_of_current_track_begin;
          }

          width_of_current_track = 0;
          if(i == 62)
            track_till_border = 1;
        }
      }
    }
    if(index_of_longest_track_begin == 0) {
      track_till_border = -1;
    }
    
    center = index_of_longest_track_begin + width_of_longest_track / 2;
  } else if(prefer_right_or_left == 1) {
    for(int i = 62; i >= 0; i--) {
      if(camera_line[i] == 1 && i > 0) {
        if(!currently_in_track) {
          index_of_current_track_begin = i;
          currently_in_track = true;
        }
        
        width_of_current_track++;
      } else if(camera_line[i] == 0 || i == 0) {
        currently_in_track = false;
        if(width_of_current_track > MIN_TRACK_WIDTH) {
          if(i == 0)
            track_till_border = -1;

          width_of_longest_track = width_of_current_track;
          index_of_longest_track_begin = index_of_current_track_begin;
          break;
        }
        width_of_current_track = 0;
      }
    }

    if(index_of_longest_track_begin == 62) {
      track_till_border = 1;
    }

    center = index_of_longest_track_begin - width_of_longest_track / 2;
  }
  
  
  //debug("index_of_current_track_begin: " + String(index_of_current_track_begin) + "\n");
  debug("index_of_longest_track_begin: " + String(index_of_longest_track_begin) + "\n");
  //debug("width_of_current_track: " + String(width_of_current_track) + "\n");
  debug("width_of_longest_track: " + String(width_of_longest_track) + "\n");
  debug("center: " + String(center) + "\n");




  // no track
  if(center == 0)
    track_till_border = 0;

  //track_till_border = i;
    
  return center;
}

void loop() {


  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug(String(voltage_of_battery));
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

  line_50_until_border = 0;
  line_100_until_border = 0;
  line_150_until_border = 0;
  line_200_until_border = 0;
  uint8_t c50 = 0;
  uint8_t c100 = 0;
  uint8_t c150 = 0;
  uint8_t c200 = 0;
  uint8_t divisor_steering = 0;
  int steer_mean = 0;
  int look_towards_right_or_left = 0;

  
  for(int i = 0, j = 0; i < 315; i = i + 5, j++) {

    pixy.video.getRGB(i, 25, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if(gray > THRESHOLD_GRAY)
      rgb_at_y_50[j] = 1;
    else
      rgb_at_y_50[j] = 0;

    //debug(String(gray) + "\n");


    pixy.video.getRGB(i, 75, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if(gray > THRESHOLD_GRAY)
      rgb_at_y_100[j] = 1;
    else
      rgb_at_y_100[j] = 0;


    pixy.video.getRGB(i, 125, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if(gray > THRESHOLD_GRAY)
      rgb_at_y_150[j] = 1;
    else
      rgb_at_y_150[j] = 0;


    pixy.video.getRGB(i, 175, &r, &g, &b, false);
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
    if(gray > THRESHOLD_GRAY)
      rgb_at_y_200[j] = 1;
    else
      rgb_at_y_200[j] = 0;
  }


  c200 = center_of_drivable_space(rgb_at_y_200, line_200_until_border, look_towards_right_or_left);
  if(c200 < 32 && c200 > 0) {
    look_towards_right_or_left = -1;
  }
  else if(c200 > 32) {
    look_towards_right_or_left = 1;
  }



  for(int i = 0; i < 63; i++) {
    debug(String(rgb_at_y_50[i]));
  }
  debug("\n");
  c50 = center_of_drivable_space(rgb_at_y_50, line_50_until_border, look_towards_right_or_left);
  if(c50 < 32 && c50 > 0) {
    debug("steer left, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c50 - 32))) + "°\n");
    steer_mean -= 2 * calculate_pull_towards_ideallinie_in_degrees(abs(c50 - 32));
    divisor_steering += 2;
  }
  else if(c50 > 32) {
    debug("steer right, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c50 - 32))) + "°\n");
    steer_mean += 2 * calculate_pull_towards_ideallinie_in_degrees(abs(c50 - 32));
    divisor_steering += 2;
  }
  else if(c50 == 32) {
    debug("steer straight, perfectly centered\n");
  }
  else if(c50 <= 0) {
    debug("no line detected\n");
  }
  debug("line till border: " + String(line_50_until_border));
  

  debug("\n");
  for(int i = 0; i < 63; i++) {
    debug(String(rgb_at_y_100[i]));
  }
  debug("\n");
  c100 = center_of_drivable_space(rgb_at_y_100, line_100_until_border, look_towards_right_or_left);
  if(c100 < 32 && c100 > 0) {
    debug("steer left, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c100 - 32))) + "°\n");
    steer_mean -= 3 * calculate_pull_towards_ideallinie_in_degrees(abs(c100 - 32));
    divisor_steering += 3;
  }
  else if(c100 > 32) {
    debug("steer right, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c100 - 32))) + "°\n");
    steer_mean += 3 * calculate_pull_towards_ideallinie_in_degrees(abs(c100 - 32));
    divisor_steering += 3;
  }
  else if(c100 == 32)
    debug("steer straight, perfectly centered\n");
  else if(c100 <= 0)
    debug("no line detected\n");
  debug("line till border: " + String(line_100_until_border));

  debug("\n");
  for(int i = 0; i < 63; i++) {
    debug(String(rgb_at_y_150[i]));
  }
  debug("\n");
  c150 = center_of_drivable_space(rgb_at_y_150, line_150_until_border, look_towards_right_or_left);
  if(c150 < 32 && c150 > 0) {
    debug("steer left, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c150 - 32))) + "°\n");
    steer_mean -= calculate_pull_towards_ideallinie_in_degrees(abs(c150 - 32));
    divisor_steering += 1;
  }
  else if(c150 > 32) {
    debug("steer right, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c150 - 32))) + "°\n");
    steer_mean += calculate_pull_towards_ideallinie_in_degrees(abs(c150 - 32));
    divisor_steering += 1;
  }
  else if(c150 == 32)
    debug("steer straight, perfectly centered\n");
  else if(c150 <= 0)
    debug("no line detected\n");
  debug("line till border: " + String(line_150_until_border));




  debug("\n");
  for(int i = 0; i < 63; i++) {
    debug(String(rgb_at_y_200[i]));
  }
  debug("\n");
  if(c200 < 32 && c200 > 0) {
    debug("steer left, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c200 - 32))) + "°\n");
    steer_mean -= calculate_pull_towards_ideallinie_in_degrees(abs(c200 - 32));
    divisor_steering += 1;
  }
  else if(c200 > 32) {
    debug("steer right, angle: " + String(calculate_pull_towards_ideallinie_in_degrees(abs(c200 - 32))) + "°\n");
    steer_mean += calculate_pull_towards_ideallinie_in_degrees(abs(c200 - 32));
    divisor_steering += 1;
  }
  else if(c200 == 32)
    debug("steer straight, perfectly centered\n");
  else if(c200 <= 0)
    debug("no line detected\n");
  debug("line till border: " + String(line_200_until_border) + "\n");





  int steer_angle = steer_mean / divisor_steering;
    
  debug("\nsteer_angle: " + String(steer_angle) + "\n");

  uint8_t number_of_positive_1 = 0;
  uint8_t number_of_negative_1 = 0;
  if(line_50_until_border == 1)
    number_of_positive_1++;
  if(line_100_until_border == 1)
    number_of_positive_1++;
  if(line_150_until_border == 1)
    number_of_positive_1++;
  if(line_200_until_border == 1)
    number_of_positive_1++;
    
  if(line_50_until_border == -1)
    number_of_negative_1++;
  if(line_100_until_border == -1)
    number_of_negative_1++;
  if(line_150_until_border == -1)
    number_of_negative_1++;
  if(line_200_until_border == -1)
    number_of_negative_1++;

  if(line_50_until_border != 0 && c50 != 0) {
    if(number_of_positive_1 > number_of_negative_1) {
      steer_angle = steer_angle * 2;
      debug("probably in right curve, doubling steer_angle\n");
      if(steer_angle < 0)
        steer_angle = steer_angle * -1;
    } else if(number_of_positive_1 < number_of_negative_1) {
      steer_angle = steer_angle * 2;
      debug("probably in left curve, doubling steer_angle\n");
      if(steer_angle > 0)
        steer_angle = steer_angle * -1;
    }
  } else debug("not checking for curve, upper line sees straight\n");

  /*if((c200 > 32 && steer_angle < 0) || (c200 < 32 && c200 > 0 && steer_angle > 0)) {
    steer_angle = steer_angle * -1;
    debug("flipped steer_angle, listening to nearest line");
    steer_angle = steer_angle * 2;
  }*/

  if(abs(c200 - c50) < 10) { // on straight
    steer_angle /= 5;
  }

  if(steer_angle > 450)
    steer_angle = 450;
  else if(steer_angle < -450)
    steer_angle = -450;


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
    else { // steer right      
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
    }


  write_i2c(current_angle_string);
  debug("\n");
  debug("\n");
}
