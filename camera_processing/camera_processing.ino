#define DEVIATION_FROM_VERTICAL 5 // threshold, how far from center_x a Line can be to still be during straight driving (not curve)

#define WAIT_TIME_BETWEEN_LINE_DETECTION 0 // ms
#define I2C_ADDRESS_OF_SLAVE 8
#define I2C_READY_PIN 3
#define PIN_RELAY 6
#define PIN_LED_1 7
#define PIN_LED_2 8
#define PIN_LED_3 9
#define PIN_BATTERY_VOLTAGE 15
//#define SPEED_ON_STRAIGHT 1.0 // 1 m/s
//#define SPEED_IN_CURVE 0.5 // 0.5 m/s

#include <Pixy2.h>
#include <vector>
#include <Line.h>
#include <algorithm> // sort()
#include <Wire.h> // I2Cs


Pixy2 pixy;
bool current_lamp_status = 0;
std::vector<Line> merged_lines;
std::vector<Line> real_lines;
bool currently_in_curve = false;
int counter_no_line_detected = 0;
bool stopped = false;
std::vector<int> steer_angles;
String interpolated_angle_string;
int steer_angles_interpolation[2];
int steer_angles_interpolation_counter = 0;
float voltage_of_battery = 10;
int batter_low_counter = 0;
int last_steer_angle = 0;
int second_last_steer_angle = 0;
String current_angle_string;
String last_steering_direction = "straight";
bool blocked_last_steering = false;

void debug(String s) {
  Serial1.println(s);
  // if changing Serial number don't forget to change it in setup() as well!
}

int return_min(int i1, int i2) {
  if(i1 <= i2)
    return i1;
  else
    return i2;  
}

void write_i2c(String s) {
  debug(s);
  char buffer[5];
  s.toCharArray(buffer, 5);
  while(digitalRead(I2C_READY_PIN) == LOW);
  Wire.beginTransmission(I2C_ADDRESS_OF_SLAVE);
  Wire.write(buffer);
  Wire.endTransmission();
}

int pitch(int x1, int y1, int x2, int y2) {
  return (y2 - y1) / (x2 - x1);
}

int calculate_steer_angle_in_degrees(int num) {
  //int angle = return_map(num, 0, 20, 0, 600); // 450
  int angle = return_map(num, 0, 20, 0, 150);
  if(angle > 450)
    angle = 450;

  return angle;
}

int calculate_push_away_angle_in_degrees(int distance_line_camera_center) {
  //int angle = return_map(distance_line_camera_center, 0, 20, 250, 0); // 0, 20, 25, 0
  int angle = return_map(distance_line_camera_center, 0, 20, 350, 0);
  if(angle < 0)
    angle = 0;

  return angle;
}

int calculate_pull_towards_ideallinie_in_degrees(int distance_from_ideallinie) {
  //int angle = return_map(distance_from_ideallinie, 0, 77, 0, 45); // can go from 0 to 77 but it is probably wise to steer 45 degrees much earlier (maybe around 40?)
  //int angle = return_map(distance_from_ideallinie, 0, 38, 0, 45); // works quite good
  //int angle = return_map(distance_from_ideallinie, 0, 30, 0, 450);
  int angle = return_map(distance_from_ideallinie, 0, 30, 0, 1350);
  if(angle > 450)
    angle = 450;

  //if(pixy.line.numVectors == 1 && angle > 220)
    //angle = 450;

  return angle;
}

/*int offset_depending_on_y_position(int y_position) {
  //debug("y_position: " + String(y_position));
  int offset = return_map(y_position, 0, 51, 28, 38); // 0 und 51 sind die min/max-Werte für Y, entsprechend mappen auf track_width/2 ganz unten und ganz oben
  if(offset < 28)
    offset = 28;
  else if(offset > 38)
    offset = 38;

  return offset;
}*/

int return_map(int value_to_map, int in_min, int in_max, int out_min, int out_max) {
  return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * If the Pixy 2 detects an intersection, combine the two vectors to just one.
 * TODO: What happens if there are 3 or more lines at one intersection?
 */
void merge_vectors() {
  //debug("numVectors: " + String(pixy.line.numVectors));
  for(int i = 0; i < pixy.line.numVectors; i++) {
    bool merged = false;
    //debug("comparing line");
    Vector v1 = pixy.line.vectors[i];
     //debug("v1: (" + String(v1.m_x0) + ", " + String(v1.m_y0) + "), (" + String(v1.m_x1) + ", " + String(v1.m_y1) + ")");
      //pixy.line.vectors[i].print();
    for(int j = 0; j < pixy.line.numVectors; j++) { // I know I am comparing every line with itself, maybe will improve it later
      if(i != j) {
        Vector v2 = pixy.line.vectors[j];
        //debug("v2: (" + String(v2.m_x0) + ", " + String(v2.m_y0) + "), (" + String(v2.m_x1) + ", " + String(v2.m_y1) + ")");
        // If v2 starts at the position that v1 ends at, merge them and add to merged_lines
        //if(v1.m_x1 == v2.m_x0 && v1.m_y1 == v2.m_y0) {
        if(v2.m_x0 > v1.m_x1 - 11 && v2.m_x0 < v1.m_x1 + 11 && v2.m_y0 > v1.m_y1 - 8 && v2.m_y0 < v1.m_y1 + 8) {
          //debug("merging");
          int index = return_min(v1.m_index, v2.m_index);
          Line new_line(v1.m_x0, v1.m_y0, v2.m_x1, v2.m_y1, index);
          merged = true;
          bool endpoint_already_exists = false;
          int index_of_endpoint_already_exists = 0;
          for(int k = 0; k < merged_lines.size(); k++) {
            if((merged_lines[k].get_x1() >= v2.m_x1 - 3 || merged_lines[k].get_x1() <= v2.m_x1 + 3) && (merged_lines[k].get_y1() >= v2.m_y1 - 3 || merged_lines[k].get_y1() <= v2.m_y1 + 3)) {
              endpoint_already_exists = true;
              merged_lines[k] = new_line;
            debug("ENDPOINT ALREADY EXISTS");
            }
          }
          if(!endpoint_already_exists)
            merged_lines.push_back(new_line);
            
          //debug("merged two lines at (" + String(v2.m_x0) + ", " + String(v2.m_y0) + ")");
          //debug("new line beginning at " + String(new_line.get_x0()) + ", " + String(new_line.get_y0()) + ")");
          debug("merged two lines:");
          debug("    Line1: (" + String(v1.m_x0) + ", " + String(v1.m_y0) + "), (" + String(v1.m_x1) + ", " + String(v1.m_y1) + ")");
          debug("    Line2: (" + String(v2.m_x0) + ", " + String(v2.m_y0) + "), (" + String(v2.m_x1) + ", " + String(v2.m_y1) + ")");
          debug("    new_line: (" + String(v1.m_x0) + ", " + String(v1.m_y0) + "), (" + String(v2.m_x1) + ", " + String(v2.m_y1) + ")");
        }
        
        // If merged, stop comparing this v1
        if(merged)
          break;
          
        // If no other Vectors starts at the end of v1, add v1 to merged_lines
        if(!merged && j == pixy.line.numVectors) {
          Line new_line(v1.m_x0, v1.m_y0, v1.m_x1, v1.m_y1, v1.m_index);
          bool endpoint_already_exists = false;
          for(int k = 0; k < merged_lines.size(); k++) {
            if(merged_lines[k].get_x1() == v1.m_x1 && merged_lines[k].get_y1() == v1.m_y1)
              endpoint_already_exists = true;
            debug("ENDPOINT ALREADY EXISTS");
          }
          if(!endpoint_already_exists) {
            merged_lines.push_back(new_line);
            debug("did not merge line, pushed back line:");
            debug("v1: (" + String(v1.m_x0) + ", " + String(v1.m_y0) + "), (" + String(v1.m_x1) + ", " + String(v1.m_y1) + ")");
          }
          //debug("old (and real) line beginning at " + String(new_line.get_x0()) + ", " + String(new_line.get_y0()) + ")");
          merged = true;
        }
        debug("v2: (" + String(v2.m_x0) + ", " + String(v2.m_y0) + "), (" + String(v2.m_x1) + ", " + String(v2.m_y1) + ")");
      }
    }
  }
}

int length_of_line(int x0, int y0, int x1, int y1) {
  return sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));
  //return (x0 + y0 + x1 + y1);
}

void add_longest_lines_to_real_lines() {
  /*for(int i = 0; i < pixy.line.numVectors; i++) {
    Vector v1 = pixy.line.vectors[i];
    if(length_of_line(v1.m_x0, v1.m_y0, v1.m_x1, v1.m_y1) > 15) {
      Line long_line(v1.m_x0, v1.m_y0, v1.m_x1, v1.m_y1, v1.m_index);
      real_lines.push_back(long_line);
    }
  }
  if(real_lines.size() == 0) {
    int length_of_longest_line = 0;
    int index_of_longest_line = 0;
    for(int i = 0; i < pixy.line.numVectors; i++) {
      Vector v1 = pixy.line.vectors[i];
      if(length_of_line(v1.m_x0, v1.m_y0, v1.m_x1, v1.m_y1) > length_of_longest_line) {
        length_of_longest_line = length_of_line(v1.m_x0, v1.m_y0, v1.m_x1, v1.m_y1);
        index_of_longest_line = i;
      }
    }
    Vector longest_vector = pixy.line.vectors[index_of_longest_line];
    Line longest_line(longest_vector.m_x0, longest_vector.m_y0, longest_vector.m_x1, longest_vector.m_y1, longest_vector.m_index);
    real_lines.push_back(longest_line);
  }*/
  for(int i = 0; i < merged_lines.size(); i++) {
    Line l1 = merged_lines[i];
    if(length_of_line(l1.get_x0(), l1.get_y0(), l1.get_x1(), l1.get_y1()) > 7) {
      //Line long_line(v1.m_x0, v1.m_y0, v1.m_x1, v1.m_y1, v1.m_index);
      real_lines.push_back(l1);
    }
  }
  if(real_lines.size() == 0) { // alternativ vielleicht einfach den Haupt-Vector der Pixy nehmen?
    int length_of_longest_line = 0;
    int index_of_longest_line = 0;
    for(int i = 0; i < merged_lines.size(); i++) {
      Line l1 = merged_lines[i];
      if(length_of_line(l1.get_x0(), l1.get_y0(), l1.get_x1(), l1.get_y1()) > length_of_longest_line) {
        length_of_longest_line = length_of_line(l1.get_x0(), l1.get_y0(), l1.get_x1(), l1.get_y1());
        index_of_longest_line = i;
      }
    }
    //Vector longest_vector = pixy.line.vectors[index_of_longest_line];
    //Line longest_line(longest_vector.m_x0, longest_vector.m_y0, longest_vector.m_x1, longest_vector.m_y1, longest_vector.m_index);
    real_lines.push_back(merged_lines[index_of_longest_line]);
  }
}

void remove_outer_line_if_both_on_one_side() {
  if(real_lines.size() == 2) {
    int index_of_outer_line = 0;
    Line l1 = real_lines[0];
    Line l2 = real_lines[1];
    if(l1.get_x0() > 39 && l2.get_x0() > 39) {
      if(l1.get_x0() > l2.get_x0())
        index_of_outer_line = 0;
      else
        index_of_outer_line = 1;
        
      real_lines.erase(real_lines.begin()+index_of_outer_line);
    } else if(l1.get_x0() <= 39 && l2.get_x0() <= 39) {
      if(l1.get_x0() > l2.get_x0())
        index_of_outer_line = 1;
      else
        index_of_outer_line = 0;
        
      real_lines.erase(real_lines.begin()+index_of_outer_line);
    }
  }
}

void preprocess_vectors() {
  merged_lines.clear();
  real_lines.clear();
  merge_vectors();
  if(pixy.line.numVectors >= 1) {
    add_longest_lines_to_real_lines();
    remove_outer_line_if_both_on_one_side();
  }
  
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pinMode(PIN_RELAY, OUTPUT);
  
  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug(String(voltage_of_battery));
  if(voltage_of_battery < 6.25) {
    debug("voltage of battery too low on startup");
    digitalWrite(PIN_RELAY, LOW);
  } else {
    digitalWrite(PIN_RELAY, HIGH);
  }

  pixy.init();
  // change to the line_tracking program.  Note, changeProg can use partial strings, so for example,
  // you can change to the line_tracking program by calling changeProg("line") instead of the whole
  // string changeProg("line_tracking")
  Serial.println(pixy.changeProg("line"));
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

void loop()
{
  Line line1;
  Line line2;
  //debug(String(millis()));
  pixy.line.getAllFeatures();
  //debug(String(millis()));

  voltage_of_battery = analogRead(PIN_BATTERY_VOLTAGE) * 0.000806 * 16;
  debug(String(voltage_of_battery));
  if(voltage_of_battery < 6.25) {
    batter_low_counter++;
    if(batter_low_counter > 500) {
      debug("voltage of battery too low");
      digitalWrite(PIN_RELAY, LOW);
      //delay(1000);
      return;
    }
  }
  
  real_lines.clear();
  //debug("Pixy lines: " + String(pixy.line.numVectors));
  //if(currently_in_curve)
    //debug("currently in curve");
  
  if(counter_no_line_detected > 100) {
    // STOP MOTOR!
    write_i2c("XXXXX");
    stopped = true;
    debug("stopped car");
  }

  if(pixy.line.numVectors == 0) {
    counter_no_line_detected++;
    if(current_lamp_status == 0) {
      current_lamp_status = 1;
      pixy.setLamp(current_lamp_status, current_lamp_status);
    } else {
      current_lamp_status = 0;
      pixy.setLamp(current_lamp_status, current_lamp_status);
    }
    /*if(counter_no_line_detected > 100) {
      // STOP MOTOR!
      write_i2c("XXXX");
      stopped = true;
      debug("stopped car");
    }*/
    delay(WAIT_TIME_BETWEEN_LINE_DETECTION);
  } else {
    if(stopped != false) {
      //write_i2c("DDDD");
      stopped = false;
    }
    counter_no_line_detected = 0;
    if(pixy.line.numVectors > 1) {
      preprocess_vectors();
    }
    else if(pixy.line.numVectors == 1) {
      Vector v = pixy.line.vectors[0];
      Line l(v.m_x0, v.m_y0, v.m_x1, v.m_y1, v.m_index);
      real_lines.push_back(l);
    }
  }
  

  debug("real_lines.size(): " + String(real_lines.size()));
  for(int i = 0; i < real_lines.size(); i++) {
    Line l = real_lines[i];
    debug("    Line "+String(i)+": (" + String(l.get_x0()) + ", " + String(l.get_y0()) + "), (" + String(l.get_x1()) + ", " + String(l.get_y1()) + ")");
  }

  int goal_x = 0;
  int distance_from_ideallinie = 0;
  int steer_angle = 0;
  if(real_lines.size() > 1) {
    Line l1 = real_lines[0];
    Line l2 = real_lines[1];

    goal_x = (l1.get_x1() + l2.get_x1()) / 2;
    debug("goal x 2 lines: " + String(goal_x));

    distance_from_ideallinie = abs(goal_x - (pixy.frameWidth / 2)); // min: 0, max: ~77
    steer_angle = calculate_pull_towards_ideallinie_in_degrees(distance_from_ideallinie);

    debug("steer_angle before correction: " + String(steer_angle));
    if(real_lines.size() > 1) {
      steer_angle = steer_angle / 2;
      if(steer_angle >= 200)
        steer_angle = 350;
    }

  }
  else if(real_lines.size() == 1) {
    Line l = real_lines[0];
    int abs_gradient = abs(l.get_x1() - l.get_x0());
    bool push_car_into_center = false;
    
    // if single line is pretty much vertical
    //if(abs(l.get_y1() - l.get_y0()) > 10 && l.get_x1() >= l.get_x0() - DEVIATION_FROM_VERTICAL && l.get_x1() <= l.get_x0() + DEVIATION_FROM_VERTICAL) {
    //if(abs(l.get_y1() - l.get_y0()) > 10 && abs(l.get_x1() - l.get_x0()) > DEVIATION_FROM_VERTICAL) {
    /*if(pitch(l.get_x0(), l.get_y0(), l.get_x1(), l.get_y1()) > 10) {
      currently_in_curve = false;
    }
    // if the single line is not vertical, car in a curve
    else {
      currently_in_curve = true;
    }*/
    if(abs(l.get_x0() - l.get_x1() > 30))
      currently_in_curve = true;
    else
      currently_in_curve = false;
    
    int line_center = (l.get_x1() + l.get_x0()) / 2;
    push_car_into_center = true;
    
    if(push_car_into_center) {
      Serial.println("push degrees into center: " + String(calculate_push_away_angle_in_degrees(abs(line_center - (pixy.frameWidth) / 2))));
    }
    //Serial.println("currently_in_curve: "+String(currently_in_curve));
    //if(currently_in_curve)
      //debug("currently in curve.");
      
    if(l.get_x1() > l.get_x0()) { // line points to the right
      if(currently_in_curve) {
        if(push_car_into_center && calculate_steer_angle_in_degrees(abs_gradient) + calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2))) <= 45) {
          //Serial.println("pushing away from line nach rechts");
          //Serial.println("debug1");
          //steer_angles_interpolation[steer_angles_interpolation_counter] = calculate_steer_angle_in_degrees(abs_gradient) + calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
          steer_angle = calculate_steer_angle_in_degrees(abs_gradient) + calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));;
        }
        else {
          //Serial.println("pushing not away nach rechts");
          //Serial.println("debug2");
          //steer_angles_interpolation[steer_angles_interpolation_counter] = calculate_steer_angle_in_degrees(abs_gradient);
          steer_angle = calculate_steer_angle_in_degrees(abs_gradient);
        }
      } else {
        if(line_center >= (pixy.frameWidth - 1) / 2) { // line on right
          //steer_angles_interpolation[steer_angles_interpolation_counter] = -calculate_steer_angle_in_degrees(abs_gradient) - calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
          //Serial.println("debug3");
          steer_angle = -calculate_steer_angle_in_degrees(abs_gradient) - calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
        } else {
          //steer_angles_interpolation[steer_angles_interpolation_counter] = calculate_steer_angle_in_degrees(abs_gradient) + calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
          //Serial.println("debug4");
          steer_angle = calculate_steer_angle_in_degrees(abs_gradient) + calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
        }
        
      }
    } else { // line points to the left or completely straight
      if(currently_in_curve) {
        if(push_car_into_center && -calculate_steer_angle_in_degrees(abs_gradient) - calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2))) >= -45) {
          //Serial.println("pushing away from line nach links");
          //steer_angles_interpolation[steer_angles_interpolation_counter] = -calculate_steer_angle_in_degrees(abs_gradient) - calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
          //Serial.println("debug5");
          steer_angle = -calculate_steer_angle_in_degrees(abs_gradient) - calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
        }
        else {
          //steer_angles_interpolation[steer_angles_interpolation_counter] = -calculate_steer_angle_in_degrees(abs_gradient);
          //Serial.println("pushing not away nach links");
          //Serial.println("debug6");
          steer_angle = -calculate_steer_angle_in_degrees(abs_gradient);
        }
      } else {
        if(line_center <= (pixy.frameWidth - 1) / 2) { // line on left
          //steer_angles_interpolation[steer_angles_interpolation_counter] = calculate_steer_angle_in_degrees(abs_gradient) + calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
          //Serial.println("debug7");
          steer_angle = calculate_steer_angle_in_degrees(abs_gradient) + calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
        } else {
          //steer_angles_interpolation[steer_angles_interpolation_counter] = -calculate_steer_angle_in_degrees(abs_gradient) - calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
          //Serial.println("debug8");
          steer_angle = -calculate_steer_angle_in_degrees(abs_gradient) - calculate_push_away_angle_in_degrees(abs(line_center - ((pixy.frameWidth - 1) / 2)));
        }
      }
    }

    if(steer_angle >= 300 && currently_in_curve)
      steer_angle = 450;
      
  if(steer_angle > 450)
    steer_angle = 450;
    
  if(steer_angle < -450)
    steer_angle = -450;

  }

  // goal_x max: 116 (78 + 38)
  // goal_x realistic max: 106 (78 + 28)
  // goal_x min: -38 (0 - 38)
  // goal_x realisitc min: -33 (0 - 33 (geschaetzt bei y irgendwo in der Mitte))

  //debug("goal_x: " + String(goal_x));
  //debug("end_point_of_line_x: " + String(end_point_of_line_x));
  //debug("distance from ideallinie: " + String(distance_from_ideallinie));
  //debug("steer_angle: " + String(steer_angle));
  /*if(goal_x <= 39)
    debug("steer direction: LEFT");
  else
    debug("steer direction: RIGHT");*/
  //debug("");

  /*if(steer_angle <= 5 && last_steer_angle >= ) {

  }*/

  // if steer direction changes, make the change not so hard
  /*if(second_last_steer_angle > 40 && last_steer_angle > 40 && steer_angle < 0)
    steer_angle = steer_angle / 2;
  else if(second_last_steer_angle < -40 && last_steer_angle < -40 && steer_angle > 0)
    steer_angle = steer_angle / 2;*/

  //if(!currently_in_curve)
    //steer_angle = steer_angle / 2;

  if(real_lines.size() > 1) {
  if(steer_angle > 450)
    steer_angle = 450;
    
  if(steer_angle < -450)
    steer_angle = -450;
    
    if(goal_x <= 39) { // steer left
      if(last_steering_direction == "right")
        steer_angle = steer_angle / 2;
        
      if(steer_angle < 10) {
        if(currently_in_curve)
          current_angle_string = "L00"+String(steer_angle)+"C";
        else
          current_angle_string = "L00"+String(steer_angle)+"S";
      } else if(steer_angle < 100) {
        if(currently_in_curve)
          current_angle_string = "L0"+String(steer_angle)+"C";
        else
          current_angle_string = "L0"+String(steer_angle)+"S";
      } else {
        if(currently_in_curve)
          current_angle_string = "L"+String(steer_angle)+"C";
        else
          current_angle_string = "L"+String(steer_angle)+"S";
      }
    }
    else { // steer right      
      if(last_steering_direction == "left")
        steer_angle = steer_angle / 2;
        
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
  } else {
    if(steer_angle <= 0) { // steer left
      if(last_steering_direction == "right")
        steer_angle = steer_angle / 2;
        
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
    } else {      
      if(last_steering_direction == "left")
        steer_angle = steer_angle / 2;
        
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
  }

  //debug(current_angle_string);
  if((current_angle_string.charAt(0) == "L" && last_steering_direction == "left") || (current_angle_string.charAt(0) == "R" && last_steering_direction == "right") || current_angle_string.charAt(0) == "S" || last_steering_direction == "straight") {    
    write_i2c(current_angle_string);
    second_last_steer_angle = last_steer_angle;
    last_steer_angle = steer_angle;
  }
  else
    debug("blocked sudden change of steering direction");

  if(current_angle_string.charAt(0) == "L")
      last_steering_direction = "left";
  else if(current_angle_string.charAt(0) == "R")
      last_steering_direction = "right";
  else// if(current_angle_string.charAt(0) == "S")
    last_steering_direction = "straight";



}
