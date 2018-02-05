
#include <Ethernet.h>
/*--------Ethernet-------------------------*/
// the MAC - media access control (ethernet hardware) address for the Ethernet shield
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
char webServerIP[] = "10.100.102.4"; 
char webServerURL[] ="www.google.com"; 
String httpResponse;

// initialize the Ethernet client library
EthernetClient client;

const int httpLEDPin=4;       // LED indicator for http communication with the web server
const int errorLEDPin=9;      // LED indicator for any system error 
/******************************************/
void setup() 
{
/*----open serial communications and setup digital pins mode--------*/ 
  Serial.begin(9600);
  pinMode(errorLEDPin,OUTPUT);      // LED is an output pin
  pinMode(httpLEDPin,OUTPUT);      // LED is an output pin
  delay(1000);                      // providing the ethernet module boot up time
/*----start the ethernet connection with DHCP IP allocation-----*/
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    digitalWrite(errorLEDPin, HIGH);
    // do nothing forevermore:
    while (true); 
  }
/*----print the allocated IP address*/
 // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println("System is ready.");
  Serial.println("10 seconds delay...");
  delay(10000);
  httpRequest();
}
/******************************************/
void loop() {
/*--- reading the HTTP respone coming from the server------*/
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
/*-----if the server's disconnected, stop the client------*/
  if (!client. connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    digitalWrite(httpLEDPin, LOW);     //Turn OFF http communication LED  
    client.stop();
    // do nothing forevermore:
    while (true);
  }
}
/******************************************/
// this method makes a HTTP connection to the server:
void httpRequest() 
{
/*---connect the "Arduino" as client to the web server---*/ 
  if (client.connect(webServerURL, 80)) {
     digitalWrite(httpLEDPin, HIGH);     //Turn ON http communication LED  
/*---for debuging-----------------------*/      
    Serial.println("connected");
    Serial.println("sending http request to web server...");
    Serial.println("GET /search?q=arduino HTTP/1.1");
    Serial.println("Host: www.google.com");
    Serial.println("Connection: close");
    Serial.println();
    Serial.println("******************************************");
/*---make a HTTP request----------------*/
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  } else {
/*---if Arduino can't connect to the server----------*/
       Serial.println("--> connection failed\n");
        digitalWrite(errorLEDPin, HIGH);       // Turn ON Error LED 
        while (true); 
  }
}
//******************************************
//  END OF PROGRAM
//******************************************

