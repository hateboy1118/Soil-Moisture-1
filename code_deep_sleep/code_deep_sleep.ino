////////////////////////////////////////////////
//Tools Setup:  Board: NodeMCU 0.9
//              CPU Frequency: 80MHz
//              Flash Size: 4M (3M SPIFFS)
//              Upload Speed: 115200
//              Programmer: AVRISP mkII
////////////////////////////////////////////////
#include <ESP8266WiFi.h>

#define SensorPin A0 //Soil Moisture meter Analog output to Arduino Analog Input 0

// Change your thingspeak API key, then change ssid and password
String apiKey = "FHR1QZ13O72HOXZX"; //APIkey
const char* ssid = "Xuan Vinh";     //Wifi's name
const char* password = "0942411939";//Wifi's password

const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() {
  // Initialize Serial with baud 115200
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin(ssid, password);
  // Print information
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //Check
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); //Connection succeeded
}

void loop() {
  int  smValue = analogRead(SensorPin);// read the input on analog pin 0
  smValue = constrain(smValue, 485, 1023);
  smValue = map(smValue,1023,485,0,100);//map the value to a percentage
  
  if (client.connect(server, 80)) { //server thingspeak is "184.106.153.149" or api.thingspeak.com
    // Send to thingspeak at field1
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(smValue);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("% send to Thingspeak");
  }
  client.stop();

  Serial.println("Waiting…");
  //delay(60000);
  ESP.deepSleep(60e6);
}

