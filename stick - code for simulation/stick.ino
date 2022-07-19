// ---------------------------------------------
#include <Wire.h>
#include <EEPROM.h>
#include <TinyGPS.h>

#include <SoftwareSerial.h>
#define ALARM 4 // define the relay control pin
#define RXT 10 // define the relay control pin
#define BZ 13 // define the relay control pin
#define wet A2 // define the relay control pin

SoftwareSerial gsmSerial(2,3);//rx,tx

TinyGPS gps; // create gps object

const int trigPin = 9; //trig pin connection 
const int echoPin = 8;  //echopin connection 

const int trigPin1 = 12; //trig pin connection 
const int echoPin1 = 11;  //echopin connection 

long duration;
int distanceCm;

long duration1;
int distanceCm1;


#define HELP3 5 // Assistance button
#define HELP2 6 // Assistance button

//gps

  float flat, flon;
    unsigned long alt;

//gsm
int state = 0;
int m;  
int temp=0;
int i=0;
char str[15];  


//Read from eeprom

float Lat;
float Long;
int dis1;
int dis2;
int object;
int hole;
int val;
//int i = 0;
int SET = 1;
int t = 0;
int count;
int a=0;
int k=0;
int y=0;


//
long previousMillis = 0;        // will store last time LED was updated
long interval = 5000;
const int sensorPin = A0;   
int sensorValue = 0; 
long int taa;
void setup()
{ 
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(HELP2, INPUT);
pinMode(HELP3, INPUT);
pinMode(RXT, INPUT);
pinMode(BZ, OUTPUT);
pinMode(wet, INPUT);
pinMode(ALARM, OUTPUT);

digitalWrite(HELP2, LOW);
digitalWrite(HELP3, LOW);
digitalWrite(RXT, LOW);
digitalWrite(BZ, LOW);
digitalWrite(wet, LOW);
digitalWrite(ALARM, LOW);

Serial.begin(9600); 
gsmSerial.begin(9600);
         
delay(100);
// Put comments later

         EEPROM.write(0,42.76543);  // Lat
         EEPROM.write(1,32.76543);  // Long
         EEPROM.write(2,0);  // object
         EEPROM.write(3,0);  // hole
         

//GSM initialization
delay(100);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(500);
gsmSerial.println("AT+CMGF=1");
delay(500);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(500);
gsmSerial.print("System ready");// The SMS text you want to send
delay(500);
gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(500);

}



void loop() {
digitalWrite(ALARM, LOW);
serialEvent(); 
delay(50); 
if(temp==1)
{
check();
temp=0;
i=0;
delay(500);
}

unsigned long currentMillis = millis();
int buttonState = digitalRead(wet);

//Getting sensor data
gpdata();
delay(50);
dis();
delay(50);
disa();
delay(50);

//Reading data from eeprom

Lat = EEPROM.read(0);
Long = EEPROM.read(1);
object = EEPROM.read(2);
hole = EEPROM.read(3);

delay(100);

if (buttonState == HIGH)
{
  digitalWrite(ALARM, HIGH);
  delay(200);
  digitalWrite(ALARM, LOW);
  delay(200);
  digitalWrite(ALARM, HIGH);
  delay(200);
  digitalWrite(ALARM, LOW);
  delay(200);
  digitalWrite(ALARM, HIGH);
  delay(200);
  digitalWrite(ALARM, LOW);
  delay(200);
}


if((digitalRead(HELP2)==HIGH)&(k==0)){
  msg2();
  k++;
}
else{
k=0;
}

if((digitalRead(HELP3)==HIGH)&(y==0))
{
  msg3();
  y++;
}
else{
  y=0;
}

if(object <50)
{
  digitalWrite(ALARM, HIGH);

}

if(digitalRead(RXT)==HIGH)
{
  digitalWrite(BZ, HIGH);
  delay(250);
  digitalWrite(BZ, LOW);
}

if(hole>55)
{
  digitalWrite(ALARM, HIGH);
  delay(200);
  digitalWrite(ALARM, LOW);
  delay(200);
  digitalWrite(ALARM, HIGH);
  delay(200);
  digitalWrite(ALARM, LOW);
  delay(200);
}

serialEvent();  
delay(50);
if(temp==1)
{
check();
temp=0;
i=0;
delay(500);
}



}

//********************************************
void serialEvent()
{
while(gsmSerial.available())
{
if(gsmSerial.find("/"))
{
delay(500);
while (gsmSerial.available())
{
char inChar=gsmSerial.read();
str[i++]=inChar;
if(inChar=='/')
{
temp=1;
return;
}
}
}
}
}
void check()
{
if(!(strncmp(str,"Location",8)))
{
 gsmSerial.println(" ");
 delay(500);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(500);
gsmSerial.println("AT+CMGF=1");
delay(500);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(500);
gsmSerial.print("Lat: ");// The SMS text you want tomost home send
delay(500);
gsmSerial.print(Lat);// The SMS text you want tomost home send
delay(1000);
gsmSerial.print("Long: ");// The SMS text you want tomost home send
delay(500);
gsmSerial.print(Long);// The SMS text you want tomost home send
delay(500);

gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(500); 
gsmSerial.println(" ");

}
}







void msg2()
{
gsmSerial.println(" ");
delay(500);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(500);
gsmSerial.println("AT+CMGF=1");
delay(500);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(500);
gsmSerial.println("I am lost");// The SMS text you want tomost home send
delay(500);
gsmSerial.print("Lat: ");// The SMS text you want tomost home send
delay(500);
gsmSerial.print(Lat);// The SMS text you want tomost home send
delay(500);
gsmSerial.print("Long: ");// The SMS text you want tomost home send
delay(500);
gsmSerial.print(Long);// The SMS text you want tomost home send
delay(500);

gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(500); 
gsmSerial.println(" ");
}

void msg3()
{
gsmSerial.println(" ");
delay(500);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(500);
gsmSerial.println("AT+CMGF=1");
delay(500);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(500);
gsmSerial.println("I am attacked, please assist");// The SMS text you want tomost home send
delay(500);
gsmSerial.print("Lat: ");// The SMS text you want tomost home send
delay(1000);
gsmSerial.print(Lat);// The SMS text you want tomost home send
delay(500);
gsmSerial.print("Long: ");// The SMS text you want tomost home send
delay(500);
gsmSerial.print(Long);// The SMS text you want tomost home send
delay(500);

gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(500);
gsmSerial.println(" ");
}


void gpdata()
{
bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }

  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long alt;
   
    gps.f_get_position(&flat, &flon);   
    flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 4;
    flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 4;

EEPROM.write(0,flat);
delay(10);
EEPROM.write(1,flon);
delay(10);
}

}

void dis()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;   
delay(10);
EEPROM.write(2,distanceCm); 
}


void disa()
{
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1, HIGH);
distanceCm1= duration1*0.034/2; 
EEPROM.write(3,distanceCm1);    
}
