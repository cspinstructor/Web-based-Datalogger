
// variables declaration
/*--------LDR sensor-------------------------*/
const int ldrPin = A0;
String ldrLast="";
String ldrCurrent="";

/*--------ultrasonic sensor------------------*/
const int triggerPin = 8;      // pin connected to Trig Pin in the ultrasonic distance sensor
const int echoPin = 9;         // pin connected to Echo Pin in the ultrasonic distance sensor
boolean newThreshold=true;     // boolean flag to indicate if it is a new threshold event 
byte thresholdDistance=15; 


/*--------Temperature sensor--------------*/
const int tempPin = A1;
const long timeInterval=5000;   // the time in miliseconds between reading the sensor  
unsigned long previousTime = 0; 
unsigned long currentTime = 0;


/*--------Indicators LEDs-------------------------*/
const int eventLEDPin=5;     // LED indicator for any sensor event 
const int httpLEDPin=4;       // LED indicator for http communication with the web server
const int errorLEDPin=3;      // LED indicator for any system error 
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

  delay(2000);
  digitalWrite(eventLEDPin, LOW);
  digitalWrite(httpLEDPin, LOW);
  digitalWrite(errorLEDPin, LOW);

  /*----start the Temperature sensor---------*/
  //dht.begin();
  delay(2000);                       // let the DHT sensor some starting time 
  digitalWrite(eventLEDPin, LOW);
  digitalWrite(httpLEDPin, LOW);
  digitalWrite(errorLEDPin, LOW);
  
  ldrLast=getSensorLight();         // set the current reading from the LDR sensor 
  Serial.println("System is ready...");
}
/******************************************/
void loop() 
{
/*------check every sensor in endless loop---------*/  
  checkLDR();           
  checkTemp();
  checkDistance();
  delay(400);      // delay between the sensors reading 
  digitalWrite(eventLEDPin,LOW);
  //Serial.println(analogRead(ldrPin));
}
/******************************************/
// LDR Sensor Functions 
/******************************************/
String getSensorLight()
{
  int value = analogRead(ldrPin);
  String lightLevel;
  if(value < 750)      lightLevel="Dark";
   else if(value < 900) lightLevel="Dim";
    else if(value < 950) lightLevel="Light";  
     else if(value < 1000) lightLevel="Bright";
      else lightLevel="Very-Bright";  
  return lightLevel; 
}
/******************************************/
void checkLDR()
{
  ldrCurrent=getSensorLight();         // read the current LDR sensor 
   if (ldrCurrent!=ldrLast) {          // if we have new reading compared to the last reading
/*---for debuging---------------------*/
       Serial.print("LDR Sensor Changed:");
       Serial.println(ldrCurrent);
/*----update the new reading and turn ON the event indicator---*/
       ldrLast=ldrCurrent;
       digitalWrite(eventLEDPin,HIGH); 
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
void checkDistance()
{
  int distance = getSensorDistance();      // read the current distance from the ultrasonic sensor
/*-----check if distance crossed the threshold level---*/
  if (distance<=thresholdDistance and newThreshold){   
/*---for debuging---------------------*/    
      Serial.print("Threshold Distance was Crossed:");         
      Serial.println(distance);
      newThreshold=false;
      digitalWrite(eventLEDPin,HIGH);      // Turn ON event indicator 
  }  
  else if (distance>thresholdDistance) newThreshold=true;
}

void checkTemp()
{
  //analogReference(INTERNAL); //See https://goo.gl/N3eQVb
  float t,h;
  currentTime = millis();
/*---check the Temp sensor every time interval---*/ 
  if(currentTime - previousTime > timeInterval) {     
    previousTime = currentTime;
    digitalWrite(eventLEDPin,HIGH); 
    //t = dht.readTemperature();
    //h = dht.readHumidity(); 
    t = (5.0 * analogRead(tempPin) * 100.0) / 1024 - 10; //-10 offset correction
    
/*---for debuging-----------------------*/
    Serial.print("Temperature: ");
    Serial.print(t); 
    Serial.print(" *C ");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println(" %\t"); 
  }

}

//******************************************
//  END OF PROGRAM
//******************************************

