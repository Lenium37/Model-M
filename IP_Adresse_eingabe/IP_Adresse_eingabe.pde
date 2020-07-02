/**
 * ControlP5 Textfield
 *
 *
 * find a list of public methods available for the Textfield Controller
 * at the bottom of this sketch.
 *
 * by Andreas Schlegel, 2012
 * www.sojamo.de/libraries/controlp5
 *
 */

import hypermedia.net.*;
import controlP5.*;

UDP udp;

ControlP5 cp5;
ControlP5 cp6;

Textfield IP1;
Textfield IP2;
Textfield IP3;
Textfield IP4;
Textfield IP5;
//Textarea  Warning_flied;
Textlabel Warning_flied;
Bang Clear;
String textValue = "";
String IP_ADRESS = "";
String message = "";
String label = "";
String IP_1;
String IP_2; 
String IP_3; 
String IP_4;
String IP_5;
int taped = 0;
int time_out = 0;
boolean wait_connect = true;
boolean Connecting = false;
boolean Connected = false;
boolean first_try = true;
int acknowledged = 0;
int IP_POSTION[] ={50, 10};
void setup() {
  size(700, 400);

  IP_SETUP();
}
void UDP_Setup(int Port_setup)
{
  udp = new UDP( this, Port_setup); // create a new datagram connection on port 8888
  udp.log( true ); // <– printout the connection activity
  udp.listen( true ); // and wait for incoming message
}

void IP_SETUP()
{

  PFont font = createFont("arial", 20);
  cp6 = new ControlP5(this);





  cp5 = new ControlP5(this);


  Clear = cp5.addBang("clear")
    .setPosition(260+IP_POSTION[0], 100+IP_POSTION[1])
    .setSize(80, 40)
    .align(ControlP5.CENTER, ControlP5.CENTER, ControlP5.CENTER, ControlP5.CENTER)
    .setVisible(true); 
  ;

  IP1 = cp5.addTextfield("Ip-Adresse")
    .setPosition(20+IP_POSTION[0], 100+IP_POSTION[1])
    .setSize(35, 40)
    .setFont(font)
    .setFocus(true)
    .setMax(3)
    .setColor(color(255, 0, 0))
    ;
  IP2 = cp5.addTextfield("input1")
    .setPosition(65+IP_POSTION[0], 100+IP_POSTION[1])
    .setSize(35, 40)
    .setFont(font)
    //.setFocus(true)
    .setLabelVisible(false) 
    .setMax(3)
    .setColor(color(255, 0, 0))
    ;
  IP3 = cp5.addTextfield("input2")
    .setPosition(110+IP_POSTION[0], 100+IP_POSTION[1])
    .setSize(35, 40)
    .setFont(font)
    //.setFocus(true)
    .setLabelVisible(false) 
    .setMax(3)
    .setColor(color(255, 0, 0))
    ;
  IP4 = cp5.addTextfield("input3")
    .setPosition(155+IP_POSTION[0], 100+IP_POSTION[1])
    .setSize(35, 40)
    .setFont(font)
    //.setFocus(true)
    .setMax(3)
    .setColor(color(255, 0, 0))
    ;
      IP5 = cp5.addTextfield("input4")
    .setPosition(200+IP_POSTION[0], 100+IP_POSTION[1])
    .setSize(35, 40)
    .setFont(font)
    //.setFocus(true)
    .setMax(3)
    .setColor(color(255, 0, 0))
    ;

  Warning_flied = cp5.addTextlabel("Warning2")
    .setPosition(20+IP_POSTION[0], 180+IP_POSTION[1])
    .setSize(200, 40)
    .setFont(createFont("arial", 12))
    .setLineHeight(14)
    .setColor(color(128))
    .setColorBackground(0)
    .setColorForeground(0);
  ;



  textFont(font);
  IP1.setMax(3);
  //IP1.setLabel("");
  IP2.setLabel("");
  IP3.setLabel("");
  IP4.setLabel("");
  IP4.setLabel("");
  Warning_flied.setLabel("");
  Warning_flied.setColorBackground(0);
  Warning_flied.setVisible(true);
  //Warning_flied2.setVisible(true);
}
void Visible(boolean ON_OFF)
{
  IP1.setVisible(ON_OFF);
  IP2.setVisible(ON_OFF);
  IP3.setVisible(ON_OFF);
  IP4.setVisible(ON_OFF);
  //IP5.setVisible(ON_OFF);
  Clear.setVisible(ON_OFF);
}
void draw() {

  //int IP = int(cp5.get(Textfield.class, "input").getText());
  // int IP = int(cp5.get(Textfield.class, "input").getText());
  //int IP = int(cp5.get(Textfield.class, "input").getText());
  //int IP = int(cp5.get(Textfield.class, "input").getText());
  background(132, 195, 232);
  Warning_flied.update();
  // label
  fill(255);

  if (Connecting == true && wait_connect == false)
  {
    // println("c");
    Visible(true);
    Connect();
  }
  if (Connected)
  {
    label = "Connected!"; 
    Visible(false);
    //println("Connected!");
  }
  Warning_flied.setText(label);
}
void keyPressed() {
  if (key == TAB)
  {
    taped++;
    switch(taped)
    {
    case 1: 
      IP1.setFocus(false);
      IP2.setFocus(true);
  //    IP5.setFocus(false);
      break;
    case 2: 
      IP2.setFocus(false);
      IP3.setFocus(true);    
      break;
    case 3: 
      IP3.setFocus(false);
      IP4.setFocus(true);    
      break;
    case 4: 
   //   IP5.setFocus(true);
      IP4.setFocus(false);    
      break;
    case 5: 
   //   IP5.setFocus(false);
      IP1.setFocus(true);    
      taped = 0;
      break;
    }
  }
  if (key == ENTER)
  {
    Connecting = true;  
    wait_connect = false;
  }
}
void Connect()
{
  if (first_try)
  {
   
    
    println(IP_5);
    first_try = false;
    clear();
  }  
  //
  if (IP_1.length() <= 3 && IP_2.length() <= 3 && IP_3.length() <= 3 && IP_4.length() <= 3 && IP_5.length() <= 5)
  {
    if (IP_1.length() != 0 && IP_2.length() != 0  && IP_3.length() != 0  && IP_4.length() != 0 && IP_5.length() != 5)
    {

      IP_ADRESS = IP_1 + "." + IP_2 + "." + IP_3 + "." + IP_4;  
      println(IP_ADRESS);
      //int Port_int = int(Port);
      //print("  ");
      //println(Port);
      //UDP_Setup(Port_int);

      if (acknowledged == 1)
      {
        wait_connect = true;
        Connecting = false; 
        Connected = true;
        acknowledged = 0;
      } else
      {
        label = "Connecting......";       
        //udp.send("&Connect!", IP_ADRESS, Port_int);
        time_out++;
        //delay(10);
        if (time_out > 200)
        {
          label = "Connection failed!";
          println("Fail");
          Connecting = false;
          time_out = 0;
          first_try = true;
          wait_connect = true;
          Connected = false;
        }
      }
    } else
    {
      //label = "IP Adress invalid";
    }
  } else
  {
    label = "IP Adress invalid";
  }
}
public void clear() {
  cp5.get(Textfield.class, "Ip-Adresse").clear();
  cp5.get(Textfield.class, "input1").clear();
  cp5.get(Textfield.class, "input2").clear();
  cp5.get(Textfield.class, "input3").clear();
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isAssignableFrom(Textfield.class)) {
    IP_1 = cp5.get(Textfield.class, "Ip-Adresse").getText();
    IP_2 = cp5.get(Textfield.class, "input1").getText();
    IP_3 = cp5.get(Textfield.class, "input2").getText();
    IP_4 = cp5.get(Textfield.class, "input3").getText();   
  }
}
void receive(byte[] data) { // <– default handler
  //void receive( byte[] data, String ip, int port ) { // <– extended handler

  data = subset(data, 0, data.length);
  message = new String( data );

  int p1 = message.indexOf("&");
  int p2 = message.indexOf("!");
  message = trim(message.substring(p1+1, p2));

  acknowledged = int(message);
  println(acknowledged);
}

public void input(String theText) {
}




/*
a list of all methods available for the Textfield Controller
 use ControlP5.printPublicMethodsFor(Textfield.class);
 to print the following list into the console.
 
 You can find further details about class Textfield in the javadoc.
 
 Format:
 ClassName : returnType methodName(parameter type)
 
 
 controlP5.Controller : CColor getColor() 
 controlP5.Controller : ControlBehavior getBehavior() 
 controlP5.Controller : ControlWindow getControlWindow() 
 controlP5.Controller : ControlWindow getWindow() 
 controlP5.Controller : ControllerProperty getProperty(String) 
 controlP5.Controller : ControllerProperty getProperty(String, String) 
 controlP5.Controller : ControllerView getView() 
 controlP5.Controller : Label getCaptionLabel() 
 controlP5.Controller : Label getValueLabel() 
 controlP5.Controller : List getControllerPlugList() 
 controlP5.Controller : Pointer getPointer() 
 controlP5.Controller : String getAddress() 
 controlP5.Controller : String getInfo() 
 controlP5.Controller : String getName() 
 controlP5.Controller : String getStringValue() 
 controlP5.Controller : String toString() 
 controlP5.Controller : Tab getTab() 
 controlP5.Controller : Textfield addCallback(CallbackListener) 
 controlP5.Controller : Textfield addListener(ControlListener) 
 controlP5.Controller : Textfield addListenerFor(int, CallbackListener) 
 controlP5.Controller : Textfield align(int, int, int, int) 
 controlP5.Controller : Textfield bringToFront() 
 controlP5.Controller : Textfield bringToFront(ControllerInterface) 
 controlP5.Controller : Textfield hide() 
 controlP5.Controller : Textfield linebreak() 
 controlP5.Controller : Textfield listen(boolean) 
 controlP5.Controller : Textfield lock() 
 controlP5.Controller : Textfield onChange(CallbackListener) 
 controlP5.Controller : Textfield onClick(CallbackListener) 
 controlP5.Controller : Textfield onDoublePress(CallbackListener) 
 controlP5.Controller : Textfield onDrag(CallbackListener) 
 controlP5.Controller : Textfield onDraw(ControllerView) 
 controlP5.Controller : Textfield onEndDrag(CallbackListener) 
 controlP5.Controller : Textfield onEnter(CallbackListener) 
 controlP5.Controller : Textfield onLeave(CallbackListener) 
 controlP5.Controller : Textfield onMove(CallbackListener) 
 controlP5.Controller : Textfield onPress(CallbackListener) 
 controlP5.Controller : Textfield onRelease(CallbackListener) 
 controlP5.Controller : Textfield onReleaseOutside(CallbackListener) 
 controlP5.Controller : Textfield onStartDrag(CallbackListener) 
 controlP5.Controller : Textfield onWheel(CallbackListener) 
 controlP5.Controller : Textfield plugTo(Object) 
 controlP5.Controller : Textfield plugTo(Object, String) 
 controlP5.Controller : Textfield plugTo(Object[]) 
 controlP5.Controller : Textfield plugTo(Object[], String) 
 controlP5.Controller : Textfield registerProperty(String) 
 controlP5.Controller : Textfield registerProperty(String, String) 
 controlP5.Controller : Textfield registerTooltip(String) 
 controlP5.Controller : Textfield removeBehavior() 
 controlP5.Controller : Textfield removeCallback() 
 controlP5.Controller : Textfield removeCallback(CallbackListener) 
 controlP5.Controller : Textfield removeListener(ControlListener) 
 controlP5.Controller : Textfield removeListenerFor(int, CallbackListener) 
 controlP5.Controller : Textfield removeListenersFor(int) 
 controlP5.Controller : Textfield removeProperty(String) 
 controlP5.Controller : Textfield removeProperty(String, String) 
 controlP5.Controller : Textfield setArrayValue(float[]) 
 controlP5.Controller : Textfield setArrayValue(int, float) 
 controlP5.Controller : Textfield setBehavior(ControlBehavior) 
 controlP5.Controller : Textfield setBroadcast(boolean) 
 controlP5.Controller : Textfield setCaptionLabel(String) 
 controlP5.Controller : Textfield setColor(CColor) 
 controlP5.Controller : Textfield setColorActive(int) 
 controlP5.Controller : Textfield setColorBackground(int) 
 controlP5.Controller : Textfield setColorCaptionLabel(int) 
 controlP5.Controller : Textfield setColorForeground(int) 
 controlP5.Controller : Textfield setColorLabel(int) 
 controlP5.Controller : Textfield setColorValue(int) 
 controlP5.Controller : Textfield setColorValueLabel(int) 
 controlP5.Controller : Textfield setDecimalPrecision(int) 
 controlP5.Controller : Textfield setDefaultValue(float) 
 controlP5.Controller : Textfield setHeight(int) 
 controlP5.Controller : Textfield setId(int) 
 controlP5.Controller : Textfield setImage(PImage) 
 controlP5.Controller : Textfield setImage(PImage, int) 
 controlP5.Controller : Textfield setImages(PImage, PImage, PImage) 
 controlP5.Controller : Textfield setImages(PImage, PImage, PImage, PImage) 
 controlP5.Controller : Textfield setLabel(String) 
 controlP5.Controller : Textfield setLabelVisible(boolean) 
 controlP5.Controller : Textfield setLock(boolean) 
 controlP5.Controller : Textfield setMax(float) 
 controlP5.Controller : Textfield setMin(float) 
 controlP5.Controller : Textfield setMouseOver(boolean) 
 controlP5.Controller : Textfield setMoveable(boolean) 
 controlP5.Controller : Textfield setPosition(float, float) 
 controlP5.Controller : Textfield setPosition(float[]) 
 controlP5.Controller : Textfield setSize(PImage) 
 controlP5.Controller : Textfield setSize(int, int) 
 controlP5.Controller : Textfield setStringValue(String) 
 controlP5.Controller : Textfield setUpdate(boolean) 
 controlP5.Controller : Textfield setValue(float) 
 controlP5.Controller : Textfield setValueLabel(String) 
 controlP5.Controller : Textfield setValueSelf(float) 
 controlP5.Controller : Textfield setView(ControllerView) 
 controlP5.Controller : Textfield setVisible(boolean) 
 controlP5.Controller : Textfield setWidth(int) 
 controlP5.Controller : Textfield show() 
 controlP5.Controller : Textfield unlock() 
 controlP5.Controller : Textfield unplugFrom(Object) 
 controlP5.Controller : Textfield unplugFrom(Object[]) 
 controlP5.Controller : Textfield unregisterTooltip() 
 controlP5.Controller : Textfield update() 
 controlP5.Controller : Textfield updateSize() 
 controlP5.Controller : boolean isActive() 
 controlP5.Controller : boolean isBroadcast() 
 controlP5.Controller : boolean isInside() 
 controlP5.Controller : boolean isLabelVisible() 
 controlP5.Controller : boolean isListening() 
 controlP5.Controller : boolean isLock() 
 controlP5.Controller : boolean isMouseOver() 
 controlP5.Controller : boolean isMousePressed() 
 controlP5.Controller : boolean isMoveable() 
 controlP5.Controller : boolean isUpdate() 
 controlP5.Controller : boolean isVisible() 
 controlP5.Controller : float getArrayValue(int) 
 controlP5.Controller : float getDefaultValue() 
 controlP5.Controller : float getMax() 
 controlP5.Controller : float getMin() 
 controlP5.Controller : float getValue() 
 controlP5.Controller : float[] getAbsolutePosition() 
 controlP5.Controller : float[] getArrayValue() 
 controlP5.Controller : float[] getPosition() 
 controlP5.Controller : int getDecimalPrecision() 
 controlP5.Controller : int getHeight() 
 controlP5.Controller : int getId() 
 controlP5.Controller : int getWidth() 
 controlP5.Controller : int listenerSize() 
 controlP5.Controller : void remove() 
 controlP5.Controller : void setView(ControllerView, int) 
 controlP5.Textfield : String getText() 
 controlP5.Textfield : String[] getTextList() 
 controlP5.Textfield : Textfield clear() 
 controlP5.Textfield : Textfield keepFocus(boolean) 
 controlP5.Textfield : Textfield setAutoClear(boolean) 
 controlP5.Textfield : Textfield setColor(int) 
 controlP5.Textfield : Textfield setColorCursor(int) 
 controlP5.Textfield : Textfield setFocus(boolean) 
 controlP5.Textfield : Textfield setFont(ControlFont) 
 controlP5.Textfield : Textfield setFont(PFont) 
 controlP5.Textfield : Textfield setFont(int) 
 controlP5.Textfield : Textfield setHeight(int) 
 controlP5.Textfield : Textfield setInputFilter(int) 
 controlP5.Textfield : Textfield setPasswordMode(boolean) 
 controlP5.Textfield : Textfield setSize(int, int) 
 controlP5.Textfield : Textfield setText(String) 
 controlP5.Textfield : Textfield setValue(String) 
 controlP5.Textfield : Textfield setValue(float) 
 controlP5.Textfield : Textfield setWidth(int) 
 controlP5.Textfield : Textfield submit() 
 controlP5.Textfield : boolean isAutoClear() 
 controlP5.Textfield : boolean isFocus() 
 controlP5.Textfield : int getIndex() 
 controlP5.Textfield : void draw(PGraphics) 
 controlP5.Textfield : void keyEvent(KeyEvent) 
 java.lang.Object : String toString() 
 java.lang.Object : boolean equals(Object) 
 
 created: 2015/03/24 12:21:31
 
 */
