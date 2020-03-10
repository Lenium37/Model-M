//Processing Sketch
// Serielle Bibliothek einbinden
import processing.serial.*;
import static javax.swing.JOptionPane.*;
import controlP5.*;

ControlP5 cp5;
ControlP5 cp6;
ControlP5 cp7;
ControlP5 cp8;
ControlP5 cp9;
ControlP5 cp10;
ControlP5 cp11;
ControlP5 cp12;
ControlP5 cp13;
ControlP5 cp14;
ControlP5 cp15;
ControlP5 cp16;
ControlP5 cp17;
ControlP5 cp18;
ControlP5 cp19;
ControlP5 cp20;
ControlP5 cp21;

Textarea myTextarea;
Textlabel Left_US;
Textlabel Right_US;
Textlabel Speed_kl25z;
Println Console;
Table table;

DropdownList d1, d2;
Button b1;
Button b2;
Button b3;
Button b4;
Button b5;
Button b6;
Knob Modus;

int Brightness = 0;
int Offset_at_75_to_Center = 0;
int Offset_at_120_to_Center = 0;
int Offset_at_165_to_Center = 0;
int Threshold_Gray = 0;
int Min_Track_width = 0;
int Red = 0;
int Green = 0;
int Blue = 0;
int Led_Brightness = 0;
int LEFT_DISTANCE = 0;
int RIGHT_DISTANCE = 0;
float norminal_speed = 0;
int THRESHOLD_DIFFERENCE = 0;
int Stop_Start = 0;
int Threshold_Gray_bar = 0;
int diff_top;
int prev_modus = 0;
int Brightness_table = 0;
int Offset_at_75_to_Center_table = 0;
int Offset_at_120_to_Center_table = 0;
int Offset_at_165_to_Center_table = 0;
int Threshold_Gray_table = 0;
int Min_Track_width_table = 0;
int Red_table = 0;
int Green_table = 0;
int Blue_table = 0;
int Led_Brightness_table = 0;
int LEFT_DISTANCE_table = 0;
int RIGHT_DISTANCE_table = 0;
float norminal_speed_table = 0;

int Y_AXIS = 1;
int X_AXIS = 2;
color c4, c5;

int myColor = color(255);
int cnt = 0;
int c1, c2;
int Port = 0;
float Drop_down_button;
float Drop_down_button_baud;
int address;
int baud;
float n, n1;
int counter = 0;
int Pixel_size = 10;
int Pixel_count = 63;
boolean press_start = false;
int button_counter = 0;
int button2_counter = 0;
int button3_counter = 0;
int drop_counter = 0;
int Console_status = 3;
// The serial port
Serial myPort;       
String Line_top;
String Data_send;
String[] save_Data;
String Line_top_middel;
String Line_bottom_middel;
String Line_bottom;
String US_data;
String Serial_Monitor_stream;
String line_detection_data;
PFont f;  // Global font variable
float x;  // horizontal location of headline
int index = 0;
int RowCount;
int x_line = 0;
color Black = color(0);
color White = color(255);
// String für empfangene Daten
String portStream;
int string_counter = 0;

int button_counter3 = 0;
boolean ready_stream = true;
int nums_top[];
int nums_top_middel[];
int nums_bottom_middel[];
int nums_bottom[];
int nums_line_detection[];
int[] differences_top = new int[64];
int[] differences_top_middle = new int[64];
int[] differences_bottom_middle = new int[64];
int[] differences_bottom = new int[64];
byte Bool_bin = 0b00000000;
float US_data_array[];
int reset_counter = 0;
// setup() wird einmal zu Beginn dea Programms ausgeführt
void setup() {
  // Ausgabefenster und Vorder-/Hintergrundfarben definieren
  size(930, 700);
  smooth(2);
  table = new Table();

  table.addColumn("Brightness", Table.INT);
  table.addColumn("Offset_at_75_to_Center", Table.INT);
  table.addColumn("Offset_at_120_to_Center", Table.INT);
  table.addColumn("Offset_at_165_to_Center", Table.INT);
  table.addColumn("Threshold_Gray", Table.INT);
  table.addColumn("Min_Track_width", Table.INT);
  table.addColumn("Red", Table.INT);
  table.addColumn("Blue", Table.INT);
  table.addColumn("Green", Table.INT);
  table.addColumn("Led_Brightness", Table.INT);
  table.addColumn("LEFT_DISTANCE", Table.INT);
  table.addColumn("RIGHT_DISTANCE", Table.INT);
  table.addColumn("norminal_speed", Table.FLOAT);
  table = loadTable("Settings.csv", "header");

  if (table != null)
  {
    RowCount = table.getRowCount();       
    Brightness_table = table.getInt(RowCount-1, "Brightness");
    Offset_at_75_to_Center_table = table.getInt(RowCount-1, "Offset_at_75_to_Center");
    Offset_at_120_to_Center_table = table.getInt(RowCount-1, "Offset_at_120_to_Center");
    Offset_at_165_to_Center_table = table.getInt(RowCount-1, "Offset_at_165_to_Center");
    Threshold_Gray_table = table.getInt(RowCount-1, "Threshold_Gray");
    Min_Track_width_table = table.getInt(RowCount-1, "Min_Track_width");
    Red_table = table.getInt(RowCount-1, "Red");
    Blue_table = table.getInt(RowCount-1, "Blue");
    Green_table = table.getInt(RowCount-1, "Green");
    Led_Brightness_table = table.getInt(RowCount-1, "Led_Brightness");
    LEFT_DISTANCE_table = table.getInt(RowCount-1, "LEFT_DISTANCE");
    RIGHT_DISTANCE_table = table.getInt(RowCount-1, "RIGHT_DISTANCE");
    norminal_speed_table = table.getFloat(RowCount-1, "norminal_speed");
    Threshold_Gray_bar = Threshold_Gray_table;
  }
  f = createFont("Arial", 16, true);  
  // Initialize headline offscreen to the right 
  x = 0; 
  //size(630,560);
  c4 = color(132, 195, 232);
  c5 = color(244, 244, 244);
  // Wenn nur ein Wert angegeben wird, wird dieser für alle 3 Farben verwendet, d.h. 255 entspricht RGB(255,255,255)
  background(132, 195, 232);
  //stroke(160);
  fill(0);
  /*for (int i = 0; i < Serial.list().length; i++)
   {
   print(Serial.list()[i]+ "\n");
   }*/
  // Hier muss der Index des Arduino-Ports ausgewählt werden. Notfalls ausprobieren.
  cp5 = new ControlP5(this);
  cp6 = new ControlP5(this);
  cp7 = new ControlP5(this);
  cp8 = new ControlP5(this);
  cp9 = new ControlP5(this);    
  cp10 = new ControlP5(this);  
  cp11 = new ControlP5(this); 
  cp12 = new ControlP5(this); 

  cp14 = new ControlP5(this);
  cp15 = new ControlP5(this);
  cp16 = new ControlP5(this);

  cp17 = new ControlP5(this);
  cp18 = new ControlP5(this);
  cp19 = new ControlP5(this);
  cp20 = new ControlP5(this);
  cp21 = new ControlP5(this);

  Left_US = cp21.addTextlabel("Left_US")
    .setPosition(45, 5)
    .setColorValue(255)   
    .setFont(createFont("Georgia", 20))
    .setVisible(true);
  ;
  Right_US = cp21.addTextlabel("Right_US")
    .setPosition(540, 5)
    .setColorValue(255)
    .setFont(createFont("Georgia", 20))
    .setVisible(true);
  ;
  Speed_kl25z = cp21.addTextlabel("Speed_kl25z")
    .setPosition(280, 5)
    .setColorValue(50)
    .setFont(createFont("Georgia", 20))   
    .setVisible(true);
  ;
  // create a toggle
  cp17.addToggle("Serial_Monitor")
    .setPosition(540, 520)
    .setSize(63, 20)
    .setColorLabel(100)
    ;   
  cp17.addToggle("Settings")
    .setPosition(20, 520)
    .setSize(63, 20)
    .setColorLabel(100)
    ;   
  cp18.addButton("Apply")
    .setPosition(20, 680)
    .setSize(63, 20)
    .setColorLabel(255)
    ;  
  b6 = cp18.addButton("Go")
    .setPosition(100, 680)
    .setSize(63, 20)
    .setColorLabel(255)
    .setLabel("Go")
    ;   
  cp9.addSlider("Line_4")
    .setPosition(215, 490)
    .setSize(200, 10)
    .setRange(0, 200)
    .setColorLabel(100)
    .setValue(128)  
    .setVisible(false); 
  ;
  cp10.addSlider("Line_1")
    .setPosition(215, 130)
    .setSize(200, 10)
    .setRange(0, 200)
    .setColorLabel(100)
    .setValue(128)
    .setVisible(false); 
  ;
  cp11.addSlider("Line_2")
    .setPosition(215, 250)
    .setSize(200, 10)
    .setRange(0, 200)
    .setColorLabel(100)
    .setValue(128)
    .setVisible(false); 
  ;
  cp12.addSlider("Line_3")
    .setPosition(215, 370)
    .setSize(200, 10)
    .setRange(0, 200)
    .setColorLabel(100)
    .setValue(128)
    .setVisible(false); 
  ;
  cp12.addSlider("Brightness")
    .setPosition(20, 560)
    .setSize(200, 10)
    .setRange(0, 255)
    .setColorLabel(100)
    .setValue(Brightness_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Offset_at_75_to_Center")
    .setPosition(20, 580)
    .setSize(200, 10)
    .setRange(0, 40)
    .setColorLabel(100)
    .setValue(Offset_at_75_to_Center_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Offset_at_120_to_Center")
    .setPosition(20, 600)
    .setSize(200, 10)
    .setRange(0, 40)
    .setColorLabel(100)
    .setValue(Offset_at_120_to_Center_table)
    .setVisible(true); 
  cp12.addSlider("Offset_at_165_to_Center")
    .setPosition(20, 620)
    .setSize(200, 10)
    .setRange(0, 40)
    .setColorLabel(100)
    .setValue(Offset_at_165_to_Center_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Threshold_Gray")
    .setPosition(20, 640)
    .setSize(200, 10)
    .setRange(0, 255)
    .setColorLabel(100)
    .setValue(Threshold_Gray_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Min_Track_width")
    .setPosition(20, 660)
    .setSize(200, 10)
    .setRange(0, 40)
    .setColorLabel(100)
    .setValue(Min_Track_width_table)
    .setVisible(true); 
  ;
  cp12.addSlider("LEFT_DISTANCE")
    .setPosition(350, 640)
    .setSize(200, 10)
    .setRange(0, 200)
    .setColorLabel(100)
    .setValue(LEFT_DISTANCE_table)
    .setVisible(true); 
  ;
  cp12.addSlider("RIGHT_DISTANCE")
    .setPosition(350, 660)
    .setSize(200, 10)
    .setRange(0, 200)
    .setColorLabel(100)
    .setValue(RIGHT_DISTANCE_table)
    .setVisible(true); 
  ;
  cp12.addSlider("norminal_speed")
    .setPosition(350, 680)
    .setSize(200, 10)
    .setRange(0.0, 5.0)
    .setColorLabel(100)
    .setValue(norminal_speed_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Red")
    .setPosition(350, 560)
    .setSize(200, 10)
    .setRange(0, 255)
    .setColorLabel(100)
    .setValue(Red_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Green")
    .setPosition(350, 580)
    .setSize(200, 10)
    .setRange(0, 255)
    .setColorLabel(100)
    .setValue(Green_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Blue")
    .setPosition(350, 600)
    .setSize(200, 10)
    .setRange(0, 255)
    .setColorLabel(100)
    .setValue(Blue_table)
    .setVisible(true); 
  ;
  cp12.addSlider("Led_Brightness")
    .setPosition(350, 620)
    .setSize(200, 10)
    .setRange(1, 20)
    .setColorLabel(100)
    .setValue(Led_Brightness_table)
    .setVisible(true); 
  ;

  b1 = cp5.addButton("Start")
    .setValue(1)
    .setPosition(215, 520)
    .setSize(200, 19)
    .setVisible(false); 

  b2 = cp7.addButton("Stop")
    .setValue(1)
    .setPosition(215, 520)
    .setSize(200, 19)
    .setVisible(false); 
  b3 = cp14.addButton("Pause")
    .setValue(1)
    .setPosition(631, 540)
    .setSize(99, 20)
    .setVisible(true); 
  b4 = cp15.addButton("Play")
    .setValue(1)
    .setPosition(730, 540)
    .setSize(100, 20)
    .setVisible(true); 
  b5 = cp16.addButton("Clear")
    .setValue(1)
    .setPosition(830, 540)
    .setSize(100, 20)
    .setVisible(true); 

  d1 = cp6.addDropdownList("Port_dropdown")
    .setPosition(320, 400)
    .setSize(200, 100)
    .close() 
    ;        
  customize(d1); // customize the first list

  d2 = cp8.addDropdownList("Baud_Dropdown")
    .setPosition(110, 400)
    .setSize(200, 100)
    .close() 
    ;        
  customize2(d2); // customize the first list

  myTextarea = cp5.addTextarea("txt")
    .setPosition(630, 0)
    .setSize(300, 540)
    .setFont(createFont("", 10))
    .setLineHeight(14)
    .setColor(color(200))
    .setColorBackground(color(0, 100))
    .setColorForeground(color(255, 100));
  ; 
  Modus = cp20.addKnob("Modus")
    .setRange(0, 5)
    .setValue(0)
    .setPosition(265, 395)
    .setRadius(50)
    .setNumberOfTickMarks(5)
    .setTickMarkLength(4)
    .snapToTickMarks(true)
    .setColorLabel(0)
    .setColorValue(0)
    .setColorValueLabel(0)
    .setColorCaptionLabel(0)
    .setColorForeground(0)
    .setColorBackground(color(255, 120, 117))
    .setColorActive(color(115, 188, 255))
    .setDragDirection(Knob.VERTICAL)
    .setVisible(true)
    ;

  Console = cp5.addConsole(myTextarea);

  cp9.setVisible(false); 
  cp10.setVisible(false);
  cp11.setVisible(false);
  cp12.setVisible(false); 
  Modus.setVisible(false);
  surface.setResizable(false);


  smooth(2);
  surface.setSize(630, 560); //+140
  cp18.setVisible(false); 
  cp17.setVisible(false); 
  Left_US.setVisible(false); 
  Right_US.setVisible(false); 
  Speed_kl25z.setVisible(false);
  //String portName = Serial.list()[4];
  // myPort initialisieren, Übertragungsrate wie bei Arduino Sketch einstellen
  //myPort = new Serial(this, portName, 115200);
  // Ankommende Zeichen am Port werden solange gebuffert, bis das angebene Zeichen empfangen wird.
  // Damit ist ein Datenblock vollständig übertragen. \n ist das 2. Zeichen eines Zeilenwechsels (\r\n)
}
void customize(DropdownList ddl) {
  // a convenience function to customize a DropdownList
  ddl.setBackgroundColor(color(190));
  ddl.setItemHeight(20);
  ddl.setBarHeight(15);
  ddl.getCaptionLabel().set("Port");
  for (int i=0; i<Serial.list().length; i++) {
    String ports = Serial.list()[i]; 
    Port = i;
    ddl.addItem(ports, i);
  }
  //ddl.scroll(0);
  ddl.setColorBackground(color(60));
  ddl.setColorActive(color(255, 128));
}
void customize2(DropdownList ddl) {
  // a convenience function to customize a DropdownList
  ddl.setBackgroundColor(color(190));
  ddl.setItemHeight(20);
  ddl.setBarHeight(15);
  ddl.getCaptionLabel().set("Baud");
  ddl.addItem("38400", 0);
  ddl.addItem("57600", 1);
  ddl.addItem("115200", 2);
  ddl.addItem("230400", 3);
  ddl.addItem("460800", 4);
  ddl.addItem("1382400", 5);
  //ddl.scroll(0);
  ddl.setColorBackground(color(60));
  ddl.setColorActive(color(255, 128));
  frameRate(600);
}

void connect_line_edges(int[] array) {
  boolean line_edge_found = false;
  int line_edge_index = 0;
  int line_width_counter = 0;

  THRESHOLD_DIFFERENCE = Threshold_Gray_bar;
  for (int i = 1; i < array.length - 1; i++) {
    if (array[i] < THRESHOLD_DIFFERENCE && !line_edge_found) {
      array[i] = 255;
    } else {
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

// Wie loop() beim Arduino wird draw() immer wieder aufgerufen, solange das Programm ausgeführt wird.
void draw() {
  smooth(2);
  if (press_start == false)
  {
    //background(132, 195, 232);
    setGradient(0, 0, 315, 700, c4, c5, X_AXIS);
    setGradient(315, 0, 315, 7000, c5, c4, X_AXIS);
  }
  //println(frameRate);
  // Steht was in portStream? (d.h. wurde ein vollständiger Datenblock übertragen)


  if (portStream != null&&press_start == true) {
    // Entspricht der Datenblock dem Format "SxxE\r\n"? Wenn ja, dann weiter
    //println(portStream);
    //println("\n");

    setGradient(0, 0, 629, 39, c4, c5, Y_AXIS);
    setGradient(0, 110, 629, 19, c5, c4, Y_AXIS);
    setGradient(0, 140, 629, 19, c4, c5, Y_AXIS);
    setGradient(0, 230, 629, 19, c5, c4, Y_AXIS);
    setGradient(0, 260, 629, 19, c4, c5, Y_AXIS);
    setGradient(0, 350, 630, 19, c5, c4, Y_AXIS);
    setGradient(0, 380, 630, 490, c4, c5, Y_AXIS);
    //setGradient(0, 470, 630, 19, c5, c4, Y_AXIS);
    //setGradient(0, 520, 630, 200, c4, c5, Y_AXIS);
    //setGradient(0, 0, 630, 40, c4, c5, Y_AXIS);
    //setGradient(0, 0, 630, 40, c4, c5, Y_AXIS);

    // Decrement x
    // If x is less than the negative width, 
    // then it is off the screen
    int Serial_Monitor = portStream.indexOf(" # ");    

    int p1 = portStream.indexOf(" $ ");
    int p2 = portStream.indexOf(" ! ");
    int p3 = portStream.indexOf(" % ");
    int p4 = portStream.indexOf(" / ");
    int p5 = portStream.indexOf(" ( ");
    int p6 = portStream.indexOf(" ; ");
    int p7 = portStream.indexOf(" ? ");
    //println(portStream);
    if (Console_status == 0)
    {
      Console.pause();
    } else if (Console_status == 1)
    {
      Console.play();
    } else if (Console_status == 2)
    {
      Console.clear();
    }

    /*print(p1);
     print(" ");
     print(p2);
     print(" ");
     print(p3);
     print(" ");
     print(p4);
     print(" ");
     print(p5);
     print(" ");
     println(p6);*/
    //println(portStream);


    //if (Serial_Monitor != -1 && p1 != -1 && p2 != -1 && p3 != -1 && p3 != -1&& p4 != -1&& p5 != -1 && p6 != -1)
    if (Serial_Monitor >= 0 && (p1 >= 0 && p1 > Serial_Monitor) && (p2 >= 0 && p2 > p1) && (p3 >= 0 && p3 > p2) && (p4 >= 0&& p4 > p3) && (p5 >= 0 && p5 > p4)&& (p6 >= 0&&p6>p5) && p6 < portStream.length())
    {

      Serial_Monitor_stream = portStream.substring(0, Serial_Monitor);
      Line_top = portStream.substring(Serial_Monitor+1, p1);
      Line_top_middel = portStream.substring(p1+1, p2);
      Line_bottom_middel = portStream.substring(p2+1, p3);
      Line_bottom = portStream.substring(p3+1, p4);
      US_data  = portStream.substring(p4+1, p5);
      String Speed_str = portStream.substring(p5+2, p6);
      line_detection_data = portStream.substring(p6+2, portStream.length());
      println(Serial_Monitor_stream);
      reset_counter++;
      if (reset_counter == 1)
      {         
        Console.clear();
        reset_counter = 0;
      }
 
      nums_top = int(split(Line_top, ' '));
      nums_top_middel = int(split(Line_top_middel, ' '));
      nums_bottom_middel = int(split(Line_bottom_middel, ' '));
      nums_bottom = int(split(Line_bottom, ' '));
      US_data_array = float(split(US_data, ' '));
      nums_line_detection = int(split(line_detection_data, ' '));
      /*println(nums_line_detection[0]);
       println(nums_line_detection[1]);
       println(nums_line_detection[2]);
       println(nums_line_detection[3]);*/
      for (int x = 5; x<35; x++)
      {
        for (int y = 5; y<35; y++)
        {
          set(x, y, White);
          set(x+590, y, White);
        }
      }
      // println(US_data_array[1]);
      //println(US_data_array[2]);
      if (US_data_array.length >= 3)
      {
        int US_data_array_int_Right = Math.round(US_data_array[2]);
        int US_data_array_int_Left = Math.round(US_data_array[1]);
        if (US_data_array_int_Right > 400 || US_data_array_int_Right == 1)
          US_data_array_int_Right = -1;
        if (US_data_array_int_Left > 400 || US_data_array_int_Left == 1)
          US_data_array_int_Left = - 1;
        String Left_stg = str(US_data_array_int_Left);
        String Right_stg = str(US_data_array_int_Right);

        Left_US.setText(Left_stg);
        Right_US.setText(Right_stg);
        Speed_kl25z.setText(Speed_str);
        //float Left = US_data_array[1];
        //float Right = US_data_array[2];
        if (US_data_array_int_Left < LEFT_DISTANCE  || US_data_array_int_Right < RIGHT_DISTANCE )
          for (int x = 5; x<35; x++)
          {
            for (int y = 5; y<35; y++)
            {
              if (US_data_array_int_Left < LEFT_DISTANCE && US_data_array_int_Left > 0)
                set(x, y, Black);
              if (US_data_array_int_Right < RIGHT_DISTANCE && US_data_array_int_Right > 0)
                set(x+590, y, Black);
            }
          }
      }


      int minimum = min(nums_top.length, nums_top_middel.length, nums_bottom_middel.length);
      if (minimum > nums_bottom.length)
      {
        minimum = nums_bottom.length;
      }

      for (int i = 0; i < minimum-1; i++) {
        if (i >= 1) {
          //differences_top[i - 1] = abs(nums_top[i - 1] - nums_top[i]);
        }
      }
      differences_top[differences_top.length - 1] = 255;
      connect_line_edges(differences_top);



      for (int k = 0; k < minimum-1; k++)
      {
        int Gray_top = nums_top[k];
        int Gray_top_middel = nums_top_middel[k];
        int Gray_bottom_middel = nums_bottom_middel[k];
        int Gray_bottom = nums_bottom[k];

        if (k < differences_top.length)
          diff_top = differences_top[k];


        color black_top = color(Gray_top);
        color black_top_middel = color(Gray_top_middel);
        color black_bottom_middel = color(Gray_bottom_middel);
        color black_bottom = color(Gray_bottom);

        x_line++;
        if (x_line == 64)
        {
          x_line = 0;
        }
        for (int x = 0+Pixel_size*k-10; x < Pixel_size+Pixel_size*k-10; x++)
        {
          /*for (int y = 0+Pixel_size*counter+40; y < Pixel_size+Pixel_size*counter+40; y++)
           {
           set(x, y, black_top);
           if (y < 50)
           {
           if (diff_top > 100)
           set(x, y+90, White);
           else
           set(x, y+90, Black);
           }
           }*/
          for (int y = 0+Pixel_size*counter+40; y < Pixel_size+Pixel_size*counter+40; y++)
          {
            set(x, y, black_top_middel);
            if (y < 50)
            {
              if (nums_line_detection[1] == 0 || nums_line_detection[1] == 120 || nums_line_detection[1] == 165)
                set(x, y+90, White);   
              else if (nums_line_detection[1] == 75)
              {
                //if ((nums_line_detection[2]*10 > x-9 || nums_line_detection[2]*10 < x) && (nums_line_detection[3]*10 > x-9 || nums_line_detection[3]*10 < x));
                if ((x < nums_line_detection[2]*10 || x > nums_line_detection[2]*10 + 9) && (x < nums_line_detection[3]*10 || x > nums_line_detection[3]*10 + 9))
                  set(x, y+90, White);

                if (nums_line_detection[2]*10 == x)
                {
                  set(x, y+90, Black);
                  set(x+1, y+90, Black);
                  set(x+2, y+90, Black);
                  set(x+3, y+90, Black);
                  set(x+4, y+90, Black);
                  set(x+5, y+90, Black);
                  set(x+6, y+90, Black);
                  set(x+7, y+90, Black);
                  set(x+8, y+90, Black);
                  set(x+9, y+90, Black);
                }
                if (nums_line_detection[3]*10 == x)
                {
                  set(x, y+90, Black);
                  set(x+1, y+90, Black);
                  set(x+2, y+90, Black);
                  set(x+3, y+90, Black);
                  set(x+4, y+90, Black);
                  set(x+5, y+90, Black);
                  set(x+6, y+90, Black);
                  set(x+7, y+90, Black);
                  set(x+8, y+90, Black);
                  set(x+9, y+90, Black);
                }
              }
            }
          }
          for (int y = 0+Pixel_size*counter+160; y < Pixel_size+Pixel_size*counter+160; y++)//280
          {
            set(x, y, black_bottom_middel);
            if (y < 50+120)//240
            {
              if (nums_line_detection[1] == 0 || nums_line_detection[1] == 75 || nums_line_detection[1] == 165)
                set(x, y+90, White);   
              else if (nums_line_detection[1] == 120)
              {
                if ((x < nums_line_detection[2]*10 || x > nums_line_detection[2]*10 + 9) && (x < nums_line_detection[3]*10 || x > nums_line_detection[3]*10 + 9))
                  set(x, y+90, White);
                if (nums_line_detection[2]*10 == x)
                {
                  set(x, y+90, Black);
                  set(x+1, y+90, Black);
                  set(x+2, y+90, Black);
                  set(x+3, y+90, Black);
                  set(x+4, y+90, Black);
                  set(x+5, y+90, Black);
                  set(x+6, y+90, Black);
                  set(x+7, y+90, Black);
                  set(x+8, y+90, Black);
                  set(x+9, y+90, Black);
                }
                if (nums_line_detection[3]*10 == x)
                {
                  set(x, y+90, Black);
                  set(x+1, y+90, Black);
                  set(x+2, y+90, Black);
                  set(x+3, y+90, Black);
                  set(x+4, y+90, Black);
                  set(x+5, y+90, Black);
                  set(x+6, y+90, Black);
                  set(x+7, y+90, Black);
                  set(x+8, y+90, Black);
                  set(x+9, y+90, Black);
                }
              }
            }
          }
          for (int y = 0+Pixel_size*counter+280; y < Pixel_size+Pixel_size*counter+280; y++)
          {
            set(x, y, black_bottom);
            if (y < 50+240)
            {
              if (nums_line_detection[1] == 0 || nums_line_detection[1] == 120 || nums_line_detection[1] == 75)
                set(x, y+90, White);   
              else if (nums_line_detection[1] == 165)
              {
                if ((x < nums_line_detection[2]*10 || x > nums_line_detection[2]*10 + 9) && (x < nums_line_detection[3]*10 || x > nums_line_detection[3]*10 + 9))
                  set(x, y+90, White);
                if (nums_line_detection[2]*10 == x)
                {
                  set(x, y+90, Black);
                  set(x+1, y+90, Black);
                  set(x+2, y+90, Black);
                  set(x+3, y+90, Black);
                  set(x+4, y+90, Black);
                  set(x+5, y+90, Black);
                  set(x+6, y+90, Black);
                  set(x+7, y+90, Black);
                  set(x+8, y+90, Black);
                  set(x+9, y+90, Black);
                }
                if (nums_line_detection[3]*10 == x)
                {
                  set(x, y+90, Black);
                  set(x+1, y+90, Black);
                  set(x+2, y+90, Black);
                  set(x+3, y+90, Black);
                  set(x+4, y+90, Black);
                  set(x+5, y+90, Black);
                  set(x+6, y+90, Black);
                  set(x+7, y+90, Black);
                  set(x+8, y+90, Black);
                  set(x+9, y+90, Black);
                }
              }
            }
          }
        }
      }
    }    
    //portStream = "";
    //myPort.clear();
    ready_stream = true;
    counter++;

    if (counter == 560/Pixel_size/8)
    {
      counter = 0;
    }
  }
  //myPort.clear();
  /*if (ready_stream == true && press_start == true)
   {
   myPort.write("Ready &"); 
   myPort.write("Ready &"); 
   myPort.write("Ready &"); 
   myPort.write("Ready &"); 
   ready_stream = false;
   }*/
}
void serialEvent(Serial myPort) {
  portStream = myPort.readStringUntil('&');
  myPort.clear();
}
public void Start() {
  button_counter++;
  if (button_counter == 2)
  {
    //myPort.write("((!!!!!))&");

    Left_US.setVisible(true); 
    Right_US.setVisible(true); 
    Speed_kl25z.setVisible(true);
    Modus.setVisible(true);
    cp17.setVisible(true); 
    cp18.setVisible(true); 
    cp9.setVisible(true); 
    cp10.setVisible(true);
    cp11.setVisible(true);
    cp12.setVisible(true);
    //b1.setVisible(false); 
    b2.setVisible(true); 
    d1.setVisible(false); 
    d2.setVisible(false); 
    press_start = true;
    button_counter = 1;
  }
}
public void Stop() {
  button2_counter++;
  if (button2_counter == 2)
  {
    Left_US.setVisible(false); 
    Right_US.setVisible(false);
    Modus.setVisible(false);
    Speed_kl25z.setVisible(false);
    cp17.setVisible(false); 
    cp18.setVisible(false); 
    cp9.setVisible(false); 
    cp10.setVisible(false);
    cp11.setVisible(false);
    cp12.setVisible(false); 
    b2.setVisible(false); 
    d1.setVisible(true); 
    d2.setVisible(true);
    press_start = false;
    button2_counter = 1;
    reset();
  }
}
public void Pause() 
{
  Console_status = 0;
}
public void Go() 
{
  button3_counter++;
  if (button3_counter == 1)
  {
    Stop_Start = 1;
    b6.setLabel("Brake");
  }
  if (button3_counter == 2)
  {
    Stop_Start = 2;
    button3_counter = 0;
    b6.setLabel("Go");
  }
}
public void Play() 
{
  Console_status = 1;
}
public void Clear() 
{
  Console_status = 2;
}
void Serial_Monitor(boolean theFlag) {
  if (theFlag==true) {
    surface.setSize(930, 560);
    b3.setPosition(631, 540);
    b4.setPosition(730, 540);
    b5.setPosition(830, 540);
    myTextarea.setSize(300, 540);
    myPort.write("Stop ");
    myPort.write('&');
    Console.clear();
    Console.play();
  } 
  if (theFlag==false) {
    surface.setSize(630, 560);
    myPort.write("Start ");
    myPort.write('&');     
    Console.clear();
    Console.pause();
  }
}
void Settings(boolean theFlag) {
  if (theFlag==true) {
    surface.setSize(630, 720);
    Console.clear();
    Console.pause();
  } 
  if (theFlag==false) {
    surface.setSize(630, 560);
  }
}
void Apply(boolean theFlag) {
  //Console.pause();
  //Console.clear();

  Data_send =  str(Brightness);
  Data_send += "!";
  Data_send += str(Offset_at_75_to_Center);
  Data_send += "[";
  Data_send += str(Offset_at_120_to_Center);
  Data_send += "$";
  Data_send += str(Threshold_Gray);
  Data_send += "%";
  Data_send += str(Min_Track_width);
  Data_send += "/";
  Data_send += str(Red);
  Data_send += "(";
  Data_send += str(Blue);
  Data_send += ")";
  Data_send += str(Green);
  Data_send += "=";
  Data_send += str(Led_Brightness);
  Data_send += "?";
  Data_send += str(LEFT_DISTANCE);
  Data_send += "#";
  Data_send += str(RIGHT_DISTANCE);
  Data_send += "+";
  Data_send += nf(norminal_speed, 1, 2);
  Data_send += "*";
  Data_send += str(Offset_at_165_to_Center);
  Data_send += "-";
  Data_send += str(Stop_Start);
  Data_send += ";";

  TableRow newRow = table.addRow();

  newRow.setInt("Brightness", Brightness);
  newRow.setInt("Offset_at_75_to_Center", Offset_at_75_to_Center);
  newRow.setInt("Offset_at_120_to_Center", Offset_at_120_to_Center);
  newRow.setInt("Offset_at_165_to_Center", Offset_at_165_to_Center);
  newRow.setInt("Threshold_Gray", Threshold_Gray);
  newRow.setInt("Min_Track_width", Min_Track_width);
  newRow.setInt("Red", Red);
  newRow.setInt("Blue", Blue);
  newRow.setInt("Green", Green);
  newRow.setInt("Led_Brightness", Led_Brightness);
  newRow.setInt("LEFT_DISTANCE", LEFT_DISTANCE);
  newRow.setInt("RIGHT_DISTANCE", RIGHT_DISTANCE);
  newRow.setFloat("norminal_speed", norminal_speed);
  saveTable(table, "Settings.csv");
  Threshold_Gray_bar = Threshold_Gray;
  myPort.write(Data_send);
  //println(Data_send.length());
  //println(Data_send);
  myPort.write('&');
}
void controlEvent(ControlEvent theEvent) {

  if (theEvent.isGroup()) {
  } else if (theEvent.isController()) {
  }
}
public void Port_dropdown(ControlEvent theEvent) { 
  Drop_down_button = theEvent.getController().getValue();
  address = Math.round(Drop_down_button); // 3
  //println(address);
  String portName = Serial.list()[address];
  myPort = new Serial(this, portName, baud);
  myPort.bufferUntil('&');
  b1.setVisible(true);  
  d1.setVisible(false);  
  d2.setVisible(false);
}
public void 
  Dropdown(ControlEvent theEvent) { 
  Drop_down_button_baud = theEvent.getController().getValue();
  baud = Math.round(Drop_down_button_baud); // 3
  //println(baud);
  if (baud == 0)
  {
    baud = 38400;
  } else if (baud == 1)
  {
    baud = 57600;
  } else if (baud == 2)
  {
    baud = 115200;
  } else if (baud == 3)
  {
    baud = 230400;
  } else if (baud == 4)
  {
    baud = 460800;
  } else if (baud == 5)
  {
    baud = 1382400;
  }
  //println(baud);
}
void reset()
{
  myColor = color(255);
  cnt = 0;
  Port = 0;
  Drop_down_button = 0;
  Drop_down_button_baud = 0;
  address = 0;
  counter = 0;
  press_start = false;
  button_counter = 1;
  button2_counter = 1;
  drop_counter = 1;
  myPort.stop();
  loop();
}
void setGradient(int x, int y, float w, float h, color c1, color c2, int axis ) {

  noFill();

  if (axis == Y_AXIS) {  // Top to bottom gradient
    for (int i = y; i <= y+h; i++) {
      float inter = map(i, y, y+h, 0, 1);
      color c = lerpColor(c1, c2, inter);
      stroke(c);
      line(x, i, x+w, i);
    }
  } else if (axis == X_AXIS) {  // Left to right gradient
    for (int i = x; i <= x+w; i++) {
      float inter = map(i, x, x+w, 0, 1);
      color c = lerpColor(c1, c2, inter);
      stroke(c);
      line(i, y, i, y+h);
    }
  }
}
void norminal_speed(float norminal_speed_value) {
  float correction_value = -0.15;
  if (norminal_speed_value < 0.15)
  {
    correction_value = 0;
  }
  if (norminal_speed_value > 1.3)
  {
    correction_value = -0.3;
  }
  if (norminal_speed_value > 2.0)
  {
    correction_value = -0.4;
  }
  norminal_speed = norminal_speed_value+correction_value;
}
void Modus(int theValue) {
  if (prev_modus != theValue)
    myPort.write("Modus "+ theValue + "&");
    delay(5);
  prev_modus = theValue;
}
