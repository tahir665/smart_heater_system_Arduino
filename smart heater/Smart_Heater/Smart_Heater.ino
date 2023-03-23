
#include <SimpleDHT.h>

 
#define TH 8
#define TR 9
#define SH 10
#define SR 11
#define FLOW A0
#define BUZ 4
#define LEDR 12
#define LEDG 13
#define SPARK 6
#define VALVE 5
#define FAN 7

SimpleDHT11 dht1(TR);
SimpleDHT11 dht2(TH);

int X;
int Y;
float TIME;
byte tempH , tempR , hum1 , hum2 , pres , smokeH , smokeR;
String tx_u="";

void setup() {
pinMode (10,INPUT);pinMode (FLOW,INPUT);pinMode (11,INPUT);pinMode(4,OUTPUT);pinMode(12,OUTPUT);pinMode(13,OUTPUT);pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(7,OUTPUT);
Serial.begin(115200);

}

void loop() {
  
   dht1.read(&tempH, &hum1, NULL);
   dht2.read(&tempR, &hum2, NULL);
   smokeH =  digitalRead(SH);
   smokeR =  digitalRead(SR);
   pres = random(0,100);
   
   X = pulseIn(FLOW, HIGH);
   Y = pulseIn(FLOW, LOW);
   TIME = X + Y;
   
  
 
  
 /// Serial.print((int)tempH); Serial.print(" *C, "); 
  //Serial.print((int)tempR); Serial.print(" *C, "); 
 

 
  senddata();
  
  //tempH
  //tempR
  //pres
  //smokeH
  //smokeR
  //TIME
}
void senddata(){
   //making data in the form of string for wifimodule// 
tx_u+=String(tempH);
tx_u+=':';
tx_u+=String(tempR);
tx_u+=':';
tx_u+=String(pres);
tx_u+=':';
tx_u+=String(smokeH);
tx_u+=':';
tx_u+=String(smokeR);
tx_u+=':';
tx_u+=String(TIME);
tx_u+='$';
Serial.println(tx_u);
tx_u="";
delay(1000);
}
  



  
