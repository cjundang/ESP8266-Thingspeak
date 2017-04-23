#include<ESP8266WiFi.h>
const char *ssid = "IoTGateway";
const char *password = "1q2w3e4r";
const char *host = "api.thingspeak.com";
 
#define HTTP_PORT 80

void initSerial(){
  Serial.begin(9600);
  delay(10);
  }

void initWiFi(){
  Serial.print("Connect to : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address");
  Serial.println(WiFi.localIP());
  
    
  }  

WiFiClient client;

void setup() {
  initSerial();
  initWiFi();

}

void Client_Request(){
  Serial.println("Connect to TCP Server");
  int cnt = 0;
  while(!client.connect(host, HTTP_PORT)){
      Serial.println(".");
      delay(100);
      cnt++;
      if (cnt>50) return;
    }
    Serial.println("Success");
    delay(500);

   /* 
    *  GET / HTTP/1.1
    *  HOST: 192.168.56.10
    */

    // get Light --> variable 
    int sensorValue = analogRead(A0);
    
    String http_request ="";
    http_request +="GET /update?api_key=6MDNFOFVU7MEJCYM&field1=";
    http_request += sensorValue;
    http_request += " HTTP/1.1\r\n";
    http_request +="HOST: ";
    http_request += host;
    http_request += "\r\n\r\n";

    
    client.print(http_request);
    Serial.println(http_request);
    delay(100);
  }


unsigned long pMil = 0;
const long interval = 30000;  //send every 30 seconds

void loop() {
  while(client.available()){
    String line = client.readStringUntil('\n');
    Serial.println(line);
    }

    unsigned long cMil = millis();
    if(cMil - pMil >= interval){
      pMil = cMil;
      Client_Request();
      }

}
