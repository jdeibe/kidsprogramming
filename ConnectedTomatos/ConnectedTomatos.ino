/*
2015-04-26 Protoyp för att larma om växer behöver vattnas dvs om jorden är för torr
Anton 8år - anton.deibe@hotmail.com
*/

#include <SoftwareSerial.h>

#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=JFNSNEA2C9083PGD&field1=";

SoftwareSerial esp8266(10, 11); // RX, TX

#define SSID        "wifi_2GHz"
#define PASS        ""

#define DEBUG true

int led1 = 4; int led2 = 5; int led3 = 6; int led4 = 7; int led5 = 8; int led6 = 9;

void setup()
{
  Serial.begin(9600);
  esp8266.begin(115200);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  
  
  Serial.println("Hello Connected Tomatos - Version 0.1");
  Serial.println("Connecting to WIFI");  
  while (!connectWiFi())
  {
    Serial.println("Connecting to WIFI");  
  }
  
  //connectWiFi();
}

void loop()
{
  int sensorValue = analogRead(0);
  Serial.println(sensorValue);  
  if (sensorValue >= 820)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, HIGH);
  }
  else if (sensorValue >= 615 && sensorValue < 820)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, LOW);
  }
  else if (sensorValue >= 410 && sensorValue < 615)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }
  else if (sensorValue >= 250 && sensorValue < 410)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }
  else if (sensorValue >= 0 && sensorValue < 250)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
  }  
  
  char buffer[4];
  String s = dtostrf(sensorValue, 4, 1, buffer);

  updateSensor(s);
  delay(60000);
}

boolean connectWiFi(){
  Serial.println("AT");
  Serial.println("AT+CWMODE=1");
  esp8266.println("AT");
  delay(1000);
  esp8266.println("AT+CWMODE=1");
  delay(2000);
  esp8266.println("AT+RST\r\n");

  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(5000);
  esp8266.println("AT+CIFSR");
  if(esp8266.find("OK"))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void updateSensor(String sensorValue){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  delay(3000);
  if(esp8266.find("Error")){
    Serial.println("Failed to update backend");
    return;
  }
  cmd = GET;
  cmd += sensorValue;
  cmd += "\r\n";

  //print to debug what to send. this is not sent to server if esp8266 doesnt return a > sign
  Serial.println(cmd);
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());

  esp8266.print("AT+CIPSEND=");
  esp8266.println(cmd.length());
  Serial.println("Success update"); 
  esp8266.print(cmd);
  delay(1000);
  Serial.println("AT+CIPCLOSE");
  Serial.println("Close connection"); 
  esp8266.println("AT+CIPCLOSE");
}

