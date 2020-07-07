
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *APssid = "Model M";
const char *APpassword = "iawz6194"; 

IPAddress APlocal_IP(192, 168, 4, 1);
IPAddress APgateway(192, 168, 4, 1);
IPAddress APsubnet(255, 255, 255, 0);
unsigned int localPort = 8888;      // local port to listen on
int v = 0;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[UDP_TX_PACKET_MAX_SIZE + 1];       // a string to send back
String Data;
WiFiUDP Udp;
void setup()
{
  ESP.wdtDisable();
  *((volatile uint32_t*) 0x60000900) &= ~(1);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(500000);
  WiFi.mode(WIFI_AP); 
  WiFi.softAPConfig(APlocal_IP, APgateway, APsubnet);
  WiFi.softAP(APssid, APpassword);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Udp.begin(localPort);
  //digitalWrite(LED_BUILTIN, LOW);
}
void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    ESP.getFreeHeap();
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Data = String(packetBuffer);
    //digitalWrite(START_PIN, LOW);
    for (int i = 0; i < UDP_TX_PACKET_MAX_SIZE; i++) packetBuffer[i] = 0;
    //Serial.println(Data);
    if (Data == "Connect")
    {
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write("1)");
      Udp.endPacket();
    }
  }
  if (Data == "Start")
  {
    Serial.println("#2!&");
    digitalWrite(LED_BUILTIN, LOW);
    //   digitalWrite(START_PIN, HIGH);
  }
  if (Data == "Start_stream")
  {
    Serial.println("#3!&");
    Serial.println("#3!&");
    Serial.println("#3!&");
    Serial.println("#3!&");
  }
  if (Data == "Stop_stream")
  {
    Serial.println("#4!&");
    Serial.println("#4!&");
    Serial.println("#4!&");
    Serial.println("#4!&");
  }
  if (Data == "Stop")
  {
    digitalWrite(LED_BUILTIN, HIGH);
    //   digitalWrite(START_PIN, LOW);
    Serial.println("#1!&");
  }
  if(Data.length() > 40)
  {
    Serial.println(Data);
  }
  if(Data.length() == 8)
  {
    Serial.println(Data);
  }
  Data = "";
  serial();
}
void serial()
{
  if (Serial.available() > 0) {
    // Lies das eingehende Byte:
    String Serial_data = Serial.readStringUntil('&');
    Serial_data.toCharArray(ReplyBuffer, UDP_TX_PACKET_MAX_SIZE);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}
