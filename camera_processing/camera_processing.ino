#define DEVIATION_FROM_VERTICAL 8 // threshold, how far from center_x a Line can be to still be during straight driving (not curve)
#define ALLOWED_DEVIATON_FROM_IDEALLINIE 5

#define WAIT_TIME_BETWEEN_LINE_DETECTION 25
#define I2C_ADDRESS_OF_SLAVE 8
#define SPEED_ON_STRAIGHT 1.0 // 1 m/s
#define SPEED_IN_CURVE 0.5 // 0.5 m/s
#define ENUM_STEER_LEFT 1.0
#define ENUM_STEER_STRAIGHT 2.0
#define ENUM_STEER_RIGHT 3.0

#define PIN_LEFT 3
#define PIN_RIGHT 4


#include <Pixy2.h>
#include <vector>
#include <Line.h>
#include <algorithm> // sort()
#include <Wire.h> // I2C


Pixy2 pixy;
bool current_lamp_status = 0;
std::vector<Line> real_lines;
bool currently_in_curve = false;
int counter_no_line_detected = 0;

void debug(String s) {
  Serial2.println(s);
  // if changing Serial number don't forget to change it in setup() as well!
}

int return_min(int i1, int i2) {
  if(i1 <= i2)
    return i1;
  else
    return i2;  
}

// Compares Lines and returns true if Line1 has the smaller m_y0
bool compareLines(Line l1, Line l2) {
    return (l1.get_y0() < l2.get_y0());
}

void write_i2c(String s) {
  char buffer[8];
  s.toCharArray(buffer, 8);
  Wire.beginTransmission(I2C_ADDRESS_OF_SLAVE);
  Wire.write(buffer);
  Wire.endTransmission();
}

void steer_left(int steer_angle) {
  if(currently_in_curve)
    steer_angle = return_map(steer_angle, 0, 38, 0, 45);
  else
    steer_angle = return_map(steer_angle, 0, 38, 0, 3);
  debug("steer_left(): " + String(steer_angle));
  //digitalWrite(PIN_LEFT, HIGH);
  //digitalWrite(PIN_RIGHT, LOW);
  if(steer_angle < 10)
    write_i2c("L0"+String(steer_angle));
  else
    write_i2c("L"+String(steer_angle));
}

void steer_right(int steer_angle) {
  if(currently_in_curve)
    steer_angle = return_map(steer_angle, 0, 38, 0, 45);
  else
    steer_angle = return_map(steer_angle, 0, 38, 0, 3);
  debug("steer_right(): " + String(steer_angle));
  //digitalWrite(PIN_LEFT, LOW);
  //digitalWrite(PIN_RIGHT, HIGH);
  if(steer_angle < 10)
    write_i2c("R0"+String(steer_angle));
  else
    write_i2c("R"+String(steer_angle));
}

void steer_straight() {
  debug("steer_straight()");
  //digitalWrite(PIN_LEFT, LOW);
  //digitalWrite(PIN_RIGHT, LOW);
  write_i2c("S00");
}

int return_map(int value_to_map, int in_min, int in_max, int out_min, int out_max) {
  return (value_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool pixel_is_in_lower_left_corner(int x, int y) {
	return (x < (pixy.frameWidth - 1) / 2 && y > pixy.frameHeight / 2);
}

bool pixel_is_in_lower_right_corner(int x, int y) {
	return (x > (pixy.frameWidth - 1) / 2 && y > pixy.frameHeight / 2);
}

/**
 * If the Pixy 2 detects an intersection, combine the two vectors to just one.
 * TODO: What happens if there are 3 or more lines at one intersection?
 */
void merge_vectors() {
  bool merged = false;
  for(int i = 0; i < pixy.line.numVectors; i++) {
    Vector v1 = pixy.line.vectors[i];
    for(int j = 0; i < pixy.line.numVectors; j++) { // I know I am comparing every line with itself, maybe will improve it later
      Vector v2 = pixy.line.vectors[j];
      // If v2 starts at the position that v1 ends at, merge them and add to real_lines
      if(v1.m_x1 == v2.m_x0 && v1.m_y1 == v2.m_y0) {
        debug("merging");
        int index = return_min(v1.m_index, v2.m_index);
        Line new_line(v1.m_x1, v1.m_y1, v2.m_x1, v2.m_y1, index);
        merged = true;
        real_lines.push_back(new_line);
        //debug("merged two lines at (" + String(v2.m_x0) + ", " + String(v2.m_y0) + ")");
        //debug("new line beginning at " + String(new_line.get_x0()) + ", " + String(new_line.get_y0()) + ")");
      }
      
      // If merged, stop comparing this v1
      if(merged)
        break;
        
      // If no other Vectors starts at the end of v1, add v1 to real_lines
      if(!merged && j == pixy.line.numVectors) {
        Line new_line(v1.m_x0, v1.m_y0, v1.m_x1, v1.m_y1, v1.m_index);
        real_lines.push_back(new_line);
        //debug("did not merge line");
        //debug("old (and real) line beginning at " + String(new_line.get_x0()) + ", " + String(new_line.get_y0()) + ")");
        merged = true;
      }
    }
  }
}

void select_bottom_two_lines() {
  sort(real_lines.begin(), real_lines.end(), compareLines);
  real_lines.erase(real_lines.begin()+2, real_lines.end());
}

void preprocess_vectors() {
  real_lines.clear();
  merge_vectors();
  if(real_lines.size() > 2)
    select_bottom_two_lines();
}

/*void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    //debug(String(c));         // print the character
  }
  //Serial2.println("received message over I2C");
}*/


void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");

  pixy.init();
  // change to the line_tracking program.  Note, changeProg can use partial strings, so for example,
  // you can change to the line_tracking program by calling changeProg("line") instead of the whole
  // string changeProg("line_tracking")
  Serial.println(pixy.changeProg("line"));
  pixy.setLamp(0, 0);
  delay(100);
  pixy.setLamp(1, 1);
  delay(100);
  pixy.setLamp(0, 0);
  delay(100);
  current_lamp_status = 1;
  pixy.setLamp(current_lamp_status, current_lamp_status);
  Serial.print("Turned on lamps...\n");
 
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);
  digitalWrite(PIN_LEFT, LOW);
  digitalWrite(PIN_RIGHT, LOW);

  Wire.begin(); // join i2c bus as master (address optional as master)
  //Wire.begin(8);                // join i2c bus with address #8
  //Wire.onReceive(receiveEvent); // register event
  
  Serial2.begin(38400); // Default communication rate of the Bluetooth module
}

void loop()
{
  //int8_t i;
  //char buf[128];
  Line line1;
  Line line2;
  pixy.line.getAllFeatures();
  
  // if zero lines
    // stop and blink hazard lights. maybe look left, then look right. If no line in sight: stop and blink hazard lights.
  
  // if only one line
    // determine if it is more on the left or more on the right and from that steer back into lane

  // if two lines
    // calculate Ideallinie and calculate deviation from it.
    // maybe calculate the mean of start_X and end_x of Ideallinie and compare that to pixy.frameWidth/2 (frameWidth should be 79 in line detection mode)
    // if the calculated x is to the left of the middle, steer left. Else steer right.

  // if more than two lines
    // get the two lines with the lowest starting point (on the camera)
    // maybe not the best for turns but will probably work out for now

  // think of something for intersections
    // probably throw away all lines that are very horizontal, calculate pitch for that (if close to 0, line is horizontal)

  // if Steigung of Ideallinie big
    // probably in curve, not on straight (= steer harder)

  // If no vectors are detected, turn on hazard lights
  // Else preprocess lines and go on
  real_lines.clear();
  
  if(counter_no_line_detected > 5) {
    // STOP MOTOR!
    write_i2c("X");
  }
    
  if(pixy.line.numVectors == 0) {
    counter_no_line_detected++;
    //debug("WARNING: No lines!");
    if(current_lamp_status == 0) {
      current_lamp_status = 1;
      pixy.setLamp(current_lamp_status, current_lamp_status);
    } else {
      current_lamp_status = 0;
      pixy.setLamp(current_lamp_status, current_lamp_status);
    }
    if(counter_no_line_detected > 5) {
      // STOP MOTOR!
      write_i2c("X");
    }
    delay(250);
  } else {
    counter_no_line_detected = 0;
      //current_lamp_status = 1;
      //pixy.setLamp(current_lamp_status, current_lamp_status);
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

  if(real_lines.size() == 2) {
    line1 = real_lines[0];
    line2 = real_lines[1];
    debug("Found " + String(real_lines.size()) + " lines");
  } else { debug("Found " + String(real_lines.size()) + " line(s)"); }

  if(real_lines.size() == 1) {
    Line l = real_lines[0];
    
    // if single line is pretty much vertical
    if(l.get_x1() >= l.get_x0() - DEVIATION_FROM_VERTICAL && l.get_x1() <= l.get_x0() + DEVIATION_FROM_VERTICAL) {
      currently_in_curve = false;
      int mean_x = (l.get_x0() + l.get_x1()) / 2;
      debug("mean_x of single line: " + String(mean_x));
      int compare_x = (pixy.frameWidth - 1) / 2 - mean_x;
      if(compare_x < - ALLOWED_DEVIATON_FROM_IDEALLINIE) {
        debug("Steer to the right?");
        steer_right(abs(compare_x));
      }
      else if(compare_x > ALLOWED_DEVIATON_FROM_IDEALLINIE) {
        debug("Steer to the left?");
        steer_left(abs(compare_x));
      }
      else {
        debug("Perfectly centered?");
        steer_straight();
      }   
    }
    // if the single line is not vertical, car in a curve
    else {
      currently_in_curve = true;
      int mean_x = (l.get_x0() + l.get_x1()) / 2;
      int mean_y = (l.get_y0() + l.get_y1()) / 2;
      int compare_x = (pixy.frameWidth - 1) / 2 - mean_x; 
      if(pixel_is_in_lower_right_corner(mean_x, mean_y) || pixel_is_in_lower_left_corner(mean_x, mean_y)) {
      	debug("Only seeing INNER line in turn, steer straight!");
      	steer_straight();
      }
      else if(l.get_x1() > l.get_x0()) {
        debug("Only seeing outer line in right turn, steer right!");
        //steer_right(l.get_x1() - l.get_x0());
        steer_right(abs(compare_x));
      } else {
        debug("Only seeing outer line in left turn, steer left!");
        //steer_left(l.get_x0() - l.get_x1());
        steer_left(abs(compare_x));
      }
    }
    delay(WAIT_TIME_BETWEEN_LINE_DETECTION);
  }
  else if(real_lines.size() == 2) {
    //Serial.println("Generating Ideallinie");
    int ideallinie_start_x = (line1.get_x0() + line2.get_x0()) / 2;
    int ideallinie_start_y = (line1.get_y0() + line2.get_y0()) / 2;
    int ideallinie_end_x = (line1.get_x1() + line2.get_x1()) / 2;
    int ideallinie_end_y = (line1.get_y1() + line2.get_y1()) / 2;
    int pitch = (ideallinie_end_y - ideallinie_start_y) / (ideallinie_end_x - ideallinie_start_x);
    if(abs(pitch) > 10)
      currently_in_curve = true;
    else
      currently_in_curve = false;
      
    debug("Ideallinie from (" + String(ideallinie_start_x) + ", " + String(ideallinie_start_y) + ") to (" + String(ideallinie_end_x) + ", " + String(ideallinie_end_y) + ")");

    int ideallinie_mean_x = (ideallinie_start_x + ideallinie_end_x) / 2;
    int compare_x = (pixy.frameWidth - 1) / 2 - ideallinie_mean_x;
    if(compare_x < - ALLOWED_DEVIATON_FROM_IDEALLINIE) {
      debug("Steer to the right!");
      steer_right(abs(compare_x));
    }
    else if(compare_x > ALLOWED_DEVIATON_FROM_IDEALLINIE) {
      debug("Steer to the left!");
      steer_left(abs(compare_x));
    }
    else {
      debug("Perfectly centered!");
      steer_straight();
    }
    debug("");
    delay(WAIT_TIME_BETWEEN_LINE_DETECTION);
  }
   
  
}