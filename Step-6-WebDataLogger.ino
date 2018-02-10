
/******************************************/
#include <DHT.h>
#include <Ethernet.h>

// variables declaration
/*--------LDR sensor-------------------------*/
const int ldrPin = A0;
String ldrLast="";
String ldrCurrent="";
/*--------DHT sensor-------------------------*/
const byte dhtPin=2;
DHT dht(dhtPin, DHT11);          // initialize the DHT sensor
const long timeInterval=15000;   // the time in miliseconds between reading the DHT sensor  
unsigned long previousTime = 0; 
unsigned long currentTime = 0;
/*--------Ultrasonic sensor------------------*/
const int triggerPin = 8;        // pin connected to Trig Pin in the ultrasonic distance sensor
const int echoPin = 9;           // pin connected to Echo Pin in the ultrasonic distance sensor
boolean newThreshold=true;       // boolean flag to indicate if it is a new threshold event 
byte thresholdDistance=15; 
/*--------Indicators LEDs-------------------------*/
const int eventLEDPin=5;        // LED indicator for any sensor event 
const int httpLEDPin=4;         // LED indicator for http communication with the web server
const int errorLEDPin=3;        // LED indicator for any system error 
/*--------Ethernet-------------------------*/
// the MAC - media access control (ethernet hardware) address for the Ethernet shield
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
// Web Server IP - when using WampServer package it is your current computer IP
char webServerIP[] = "192.168.1.101";  
EthernetClient client;         // initialize the Ethernet client library  
/******************************************/
void setup() 
{
/*----open serial communications ---------*/ 
  Serial.begin(9600);
/*----setting each digital pin mode-------*/
  pinMode(triggerPin,OUTPUT);        // Trigger is an output pin
  pinMode(echoPin,INPUT);            // Echo is an input pin
  pinMode (ldrPin, INPUT);           // LDR is input pin 
  pinMode(eventLEDPin,OUTPUT);       // LED is an output pin
  pinMode(errorLEDPin,OUTPUT);       // LED is an output pin
  pinMode(httpLEDPin,OUTPUT);        // LED is an output pin
/*----perform short LEDs test------------*/  
  digitalWrite(eventLEDPin, HIGH);   
  digitalWrite(httpLEDPin, HIGH);
  digitalWrite(errorLEDPin, HIGH);
/*----start the DHT sensor---------*/
  dht.begin();
  delay(10000);                     // let the DHT sensor some starting time 
  digitalWrite(eventLEDPin, LOW);
  digitalWrite(httpLEDPin, LOW);
  digitalWrite(errorLEDPin, LOW);
  ldrLast=getSensorLight();         // set the current reading from the LDR sensor 
/*----start the ethernet connection with DHCP IP allocation-----*/
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    digitalWrite(errorLEDPin, HIGH); // turn ON Error LED indicator 
    // do nothing forevermore:
    while (true);                    
  }
/*----print the allocated IP address*/
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println("System is ready...");
}
/******************************************/
void loop() 
{
/*------check every sensor in endless loop---------*/  
  checkLDR();            
  checkDHT(); 
  checkUltrasonic();
/*-----delay between the sensors reading-----------*/
  delay(400);      
  digitalWrite(eventLEDPin,LOW);    // Turn OFF the event LED  
  //Serial.println(analogRead(ldrPin));
}
/******************************************/
// LDR Sensor Functions 
/******************************************/
/*------ return the light level reading from the LDR sensor------*/ 
String getSensorLight()
{
  int value = analogRead(ldrPin);
  String lightLevel;
  if(value < 850)      lightLevel="Dark";
  else lightLevel="Bright";   
  return lightLevel; 
}
/******************************************/
void checkLDR()
{
  ldrCurrent=getSensorLight();         // read the current LDR sensor 
   if (ldrCurrent!=ldrLast) {          // do we have new reading compared to the last reading ?
/*---for debuging---------------------*/
       Serial.print("LDR Sensor Changed:");
       Serial.println(ldrCurrent);
/*----update the new reading and turn ON the event indicator---*/
       ldrLast=ldrCurrent;
       digitalWrite(eventLEDPin,HIGH); 
/*-----send event log to the web server to be stored in the database-----*/
       String sensorsData="sensor1="+ldrCurrent;
       Serial.println(sensorsData);
       httpRequest("GET /mydatalogger/add_ldr.php?"+sensorsData+" HTTP/1.0");
   }
}
/******************************************/
// Ultrasonic Sensor Functions 
/******************************************/
int getSensorDistance()
{
  long duration;
/*--- trigger ping burst and wait for echo from measured object----------------------*/ 
  digitalWrite(triggerPin, LOW);                   
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(triggerPin, LOW);           // Trigger pin to LOW
/*----waits for the echo pin to get high and returns the duration in microseconds----*/
  duration = pulseIn(echoPin,HIGH); 
  return (duration/2) / 29.386;            // return distance in cm
}
/******************************************/
void checkUltrasonic()
{
  int distance = getSensorDistance();      // read the current distance from the ultrasonic sensor
/*-----check if distance crossed the threshold level---*/
  if (distance<=thresholdDistance and newThreshold){     
/*---for debuging---------------------*/    
      Serial.print("Threshold Distance was Crossed:");         
      Serial.println(distance);        
      newThreshold=false;
      digitalWrite(eventLEDPin,HIGH);      // Turn ON event indicator 
/*-----send event log to the web server to be stored in the database-----*/
       String sensorsData="sensor1="+String(distance);
       httpRequest("GET /mydatalogger/add_ultrasonic.php?"+sensorsData+" HTTP/1.0");
  }  
  else if (distance>thresholdDistance) newThreshold=true;
}
/******************************************/
void checkDHT()
{
  float t,h;
  currentTime = millis();
/*---check the DHT sensor every time interval---*/ 
  if(currentTime - previousTime > timeInterval) {     
    previousTime = currentTime;
    digitalWrite(eventLEDPin,HIGH); 
    t = dht.readTemperature();
    h = dht.readHumidity(); 
/*---for debuging-----------------------*/
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %\t"); 
/*-----send event log to the web server database-----*/    
    String sensorsData="sensor1="+String(t)+"&sensor2="+String(h);
    httpRequest("GET /mydatalogger/add_dht11.php?"+sensorsData+" HTTP/1.0");
  }
}
/******************************************/
/*--------push data to the web server with GET request-------*/
void httpRequest(String request) 
{
/*---connect the "Arduino" as client to the web server---*/ 
   if (client.connect(webServerIP,80)) {  //connect the "Arduino" as client to the web server using socket   
      digitalWrite(httpLEDPin, HIGH);     //Turn ON http communication LED  
/*---for debuging-----------------------*/      
      Serial.println("connected.");
      Serial.println("sending data to web server...");      
      Serial.println(request);
      Serial.println("Connection: close"); // telling the server that we are over transmitting the message
      Serial.println(); // empty line
/*----send sensors data to the web server using GET request---*/ 
      client.println(request);
      client.println("Connection: close"); // telling the server that we are over transmitting the message
      client.println();                    // empty line
/*----display the response message from the server------------*/
      httpResponse();
    }
    else {
/*---if Arduino can't connect to the server----------*/
      Serial.println("--> connection failed\n");
      digitalWrite(errorLEDPin, HIGH);       // Turn ON Error LED
      while (true);                          // do nothing forevermore
    }
    if (client.connected()) { 
      client.stop();                         // close communication socket 
    }
    digitalWrite(httpLEDPin, LOW);           // Turn OFF the http communication LED
}
/******************************************/
void httpResponse() 
{
  long timestamp=millis();
  do {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  } while ((millis()-timestamp)<=3000);

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();    // closing connection to server
  } 
}
/******************************************/
//******************************************
//  END OF PROGRAM
//******************************************

