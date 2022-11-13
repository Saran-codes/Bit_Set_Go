
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include <math.h>
#define LED_BUILTIN 2

int bit[3] = {12,14,17};
int val = -1; 
const char* ssid = "";
const char* password = "";
const char* PARAM_INPUT_1 = "input1";
String msg="0";

AsyncWebServer server(80);
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
    <button class="button" onmousedown="toggleCheckbox('on');" ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">Start</button>
    <h2>Decimal Value: </h2>
    <p> Decimal: <span id="decimal"> </span> </p>
 
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup(){

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(bit[0], INPUT);
  pinMode(bit[1], INPUT);
  pinMode(bit[2], INPUT);
  Serial.println();
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  digitalWrite(LED_BUILTIN,LOW);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/click", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", msg);
  }); 
 server.on("/decimal", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", String(val));
  });
  server.on("/",[](AsyncWebServerRequest *request){
    String page= "<h2>Decimal value is:</h2> <h2>"+String(val)+"</h2>";
    request->send(200, "text/html",page);
  });
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(LED_BUILTIN, HIGH);
    msg="READ";
    request->send(200, "text/plain", "ok");
  });
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(LED_BUILTIN, LOW);
    msg="NOT READ";
    request->send(200, "text/plain", "ok");
  });
  server.onNotFound(notFound);
  server.begin();
}
int getdigit(){
  int decimal = 0;
  for(int i = 0;i<3;i++){
    if (digitalRead(bit[i]) == HIGH) decimal += pow(2,i);
  }
  return decimal;
}
void loop(){
  if(msg=="READ"){
    digitalWrite(LED_BUILTIN,HIGH);
      val = getdigit();
      Serial.print(val);
      delay(4000);
  }
  else {
    val=0;
    
    digitalWrite(LED_BUILTIN,LOW);
  }
}
