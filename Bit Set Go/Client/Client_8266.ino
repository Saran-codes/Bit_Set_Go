#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;
#include <math.h>
#define LED_BUILTIN 2
#include <math.h>
#define PI 3.142857
double speed = 100;
double wheel_d = 17;
double quanta = 50;
//double speed = (rpm/60)
int pin[4] = {0,2,4,5};


unsigned long int turntime = (unsigned long int)((((PI*wheel_d/2)/2)/speed)*2500);
unsigned long int traveltime = (unsigned long int)((quanta/speed)*2500);

const char* ssid = "";
const char* password = "";
const char* serverNameClick = "http://192.168.4.1/click";
const char* serverDecimal = "http://192.168.4.1/decimal";

#include <Wire.h>


String link;
String decimal;

unsigned long previousMillis = 0;
const long interval = 5000; 

void setup() {
  Serial.begin(115200);
   pinMode(pin[0], OUTPUT); //motor 1 +ve
   pinMode(pin[1], OUTPUT);  //motor 1 -ve
   pinMode(pin[2], OUTPUT); //motor 2 +ve
   pinMode(pin[3], OUTPUT); //motor 2 -ve

   digitalWrite(pin[0],LOW);
   digitalWrite(pin[1],LOW);
   digitalWrite(pin[2],LOW);
   digitalWrite(pin[3],LOW);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");;l
  Serial.println("Connected to WiFi");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      link = httpGETRequest(serverNameClick);
      decimal = httpGETRequest(serverDecimal);
      Serial.println("Decimal value is: " + decimal);
 
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }

  if(decimal.toInt() >=0 && decimal.toInt() <=7){
//    followpath(path[decimal.toInt()]);
Serial.println("Working");
    forward(decimal.toInt());
  
  delay(4000);
    } 
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
   
  http.begin(client, serverName);

  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}
void forward(int units){
  digitalWrite(pin[0],HIGH);
  digitalWrite(pin[2],HIGH);
  delay(traveltime*units);
  digitalWrite(pin[0],LOW);
  digitalWrite(pin[2],LOW);
  delay(1000);
}
void right(){
  digitalWrite(pin[0],HIGH);
  digitalWrite(pin[3],HIGH);
  delay(turntime);
  digitalWrite(pin[0],LOW);
  digitalWrite(pin[3],LOW);
  delay(1000);
}

void left(){
  digitalWrite(pin[1],HIGH);
  digitalWrite(pin[2],HIGH);
  delay(turntime);
  digitalWrite(pin[1],LOW);
  digitalWrite(pin[2],LOW);
  delay(1000);
}
void followpath(String path){
  for(int i=0;i<sizeof(path.c_str())-1;i++){
    if (path[i]=='s') forward(0.5);
    else if (path[i]=='r') right();
    else if (path[i]=='l') left();
  }
}
