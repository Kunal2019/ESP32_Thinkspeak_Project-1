#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Kunal";
const char* password = "Kunnumunnu1";

String servername = "https://api.thingspeak.com/update?api_key=W0Q93VRN5M2DPW6S"; //Is the link which sends the data to the Thinkspeak Server.
unsigned long lastTime = 0;

unsigned long timeDelay = 15000; //Specifying the Delay Timer

const int LDR = 34; 
int outputpin= 32;

void setup() {
  Serial.begin(9600); 
  WiFi.begin(ssid, password);
  Serial.println("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int LDR_val = analogRead(LDR); //LDR reading
  int analogValue = analogRead(outputpin);
  float millivolts = (analogValue * 3300) /2048.0; //3300 is the voltage provided by NodeMCU
  float celsius = millivolts/10;

  if ((millis() - lastTime) > timeDelay) {
    if (WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      String serverPath = servername + "&field1=" + String(celsius);
      String serverPath1 = serverPath + "&field2=" + String(LDR_val);
      http.begin(serverPath1.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode > 0){
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        }
      else {
        Serial.print("Error Code: ");
        Serial.println(httpResponseCode);        
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }  
}
