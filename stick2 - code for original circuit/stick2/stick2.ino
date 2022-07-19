// ---------------------------------------------
#include <Wire.h>
#include <EEPROM.h>
#include <TinyGPS++.h>

#include <SoftwareSerial.h>
#define ALARM 4 
#define RXT 10 
#define LD 13 
#define BZ 7 
#define wet A2 
#define data A1 

SoftwareSerial gsmSerial(2,3);//rx,tx

TinyGPSPlus gps;

const int trigPin = 9; //trig pin connection 
const int echoPin = 8;  //echopin connection 

const int trigPin1 = 12; //trig pin connection 
const int echoPin1 = 11;  //echopin connection 

long duration;
int distanceCm;

long duration1;
int distanceCm1;

int vall =0;
int valk =0;
#define HELP3 5 // Assistance button
#define HELP2 6 // Assistance button

//gps

  double latitude,longitude;
    unsigned long alt;

//gsm
int p=0;
int q=0;
int b=0;
int r=0;
int state = 0;
int m;  
int temp=0;
int i=0;
char str[15];  

float Lat;
float Long;
int dis1;
int dis2;
int object;
int hole;
int val;

int SET = 1;
int t = 0;
int count;
int a=0;
int k;
int y;

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
pinMode(LD, OUTPUT);

digitalWrite(HELP2, LOW);
digitalWrite(RXT, LOW);
digitalWrite(HELP3, LOW);
digitalWrite(BZ, LOW);
digitalWrite(wet, LOW);
digitalWrite(ALARM, LOW);
digitalWrite(LD, HIGH);

Serial.begin(9600); 
gsmSerial.begin(9600);
         
delay(20000);
// Put comments later
/*
         EEPROM.write(0,42.76543);  // Lat
         EEPROM.write(1,32.76543);  // Long
         EEPROM.write(2,0);  // object
         EEPROM.write(3,0);  // hole
         

*/
//GSM initialization
delay(2000);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(2000);
gsmSerial.println("AT+CMGF=1");
delay(2000);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(2000);
gsmSerial.print("Blind stick system ready");// The SMS text you want to send
delay(2000);
gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(2000);
}



void loop() {
vall = analogRead(wet);  
valk = analogRead(data); 
p=0;
digitalWrite(LD, LOW);

digitalWrite(ALARM, LOW);
serialEvent(); 
delay(1000); 
if(temp==1)
{
check();
temp=0;
i=0;
delay(500);
}

//unsigned long currentMillis = millis();

//Getting sensor data

dis();
delay(10);
disa();
delay(10);

//Reading data from eeprom

//Lat = EEPROM.read(0);
Long = EEPROM.read(1);
object = EEPROM.read(2);
hole = EEPROM.read(3);

delay(100);


if (vall>350)
{
  digitalWrite(ALARM, HIGH);
  delay(500);
  digitalWrite(ALARM, LOW);
  delay(500);
  digitalWrite(ALARM, HIGH);
  delay(500);
  digitalWrite(ALARM, LOW);
  delay(500);
  digitalWrite(ALARM, HIGH);
  delay(500);
  digitalWrite(ALARM, LOW);
  delay(500);
}


if((digitalRead(HELP2)==HIGH)&(k<3)){
  msg2();
  k++;
}
else{
k=0;
}

if((digitalRead(HELP3)==HIGH)&(y<3))
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

//if(valk>10)
if(digitalRead(RXT)==HIGH)
{
  digitalWrite(BZ, HIGH);
  
}
else{
  digitalWrite(BZ, LOW); 
}

if(hole>25)
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
delay(2000);
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
 //while((p==0))
while((Serial.available() > 0)& (p==0))
{
displayInfo();
Serial.print(latitude,6);
Serial.print(F(","));
Serial.println(longitude,6);
if((latitude>0)|(longitude>0))
{
  p++;
}
}
 gsmSerial.println(" ");
 delay(2000);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(2000);
gsmSerial.println("AT+CMGF=1");
delay(2000);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(2000);
gsmSerial.print("Lat: ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(latitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.print("Long: ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.println(longitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.println(" ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(latitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(",");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(longitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(2000); 
gsmSerial.println(" ");

}
}







void msg2()
{
// while(p==0)
 while((Serial.available() > 0)& (p==0))
{
displayInfo();
Serial.print(latitude,6);
Serial.print(F(","));
Serial.println(longitude,6);
if((latitude>0)|(longitude>0))
{
  p++;
}
}
gsmSerial.println(" ");
delay(2000);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(2000);
gsmSerial.println("AT+CMGF=1");
delay(2000);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(2000);
gsmSerial.println("I am lost");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print("Lat: ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(latitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.print("Long: ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.println(longitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.println(" ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(latitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(",");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(longitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(2000); 
gsmSerial.println(" ");
}

void msg3()
{
//while(p==0)
while((Serial.available() > 0)& (p==0))
{
displayInfo();
Serial.print(latitude,6);
Serial.print(F(","));
Serial.println(longitude,6);
if((latitude>0)|(longitude>0))
{
  p++;
}
}
gsmSerial.println(" ");
delay(2000);
gsmSerial.println("AT+CNMI=2,2,0,0,0");
delay(2000);
gsmSerial.println("AT+CMGF=1");
delay(2000);
gsmSerial.println("AT+CMGS=\"+255657515557\"\r"); // Replace x with mobile number
delay(2000);
gsmSerial.println("I am attacked, please assist");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print("Lat: ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(latitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.print("Long: ");// The SMS text you want tomost home send
delay(2000);
delay(2000);
gsmSerial.println(longitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.println(" ");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(latitude,6);// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(",");// The SMS text you want tomost home send
delay(2000);
gsmSerial.print(longitude,6);// The SMS text you want tomost home send
gsmSerial.println((char)26); // ASCII code of CTRL+Z
delay(2000);
gsmSerial.println(" ");
}

void displayInfo()
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

    gps.f_get_position(&flat, &flon);   
    latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 4;
   longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 4;

EEPROM.write(0,latitude);
delay(10);
EEPROM.write(1,longitude);
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
