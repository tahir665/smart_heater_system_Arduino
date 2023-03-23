#include <FirebaseESP32.h>
#include "WiFi.h"
#define WIFI_SSID "Redmi Note 8"
#define WIFI_PASSWORD "faisal123"
#define FIREBASE_HOST "https://heater-d314c-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "qGuAvYqG3wibIZ1iMN49WsqZkUJLyo14yIa3kFqH"
FirebaseData fbdo;
FirebaseData getdata1;
FirebaseJson json;
String tempH="",tempR="",smokeH="",smokeR="",pres="",TIME="";
int d=0;
int fan_chk=0;
int temp_chk=0;
int smk_chk=0;
String saj="";
String h="1";
String h1="2";
String h2="3";

String d1="";
String d2="";
String d3="";
int a=0;
unsigned previous_mills=0;
const long interval=10000;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
pinMode(19,OUTPUT);pinMode(22,OUTPUT);pinMode(23,OUTPUT);
pinMode(15,OUTPUT);pinMode(2,OUTPUT);pinMode(4,OUTPUT);
digitalWrite(19,HIGH);digitalWrite(22,HIGH);digitalWrite(23,HIGH);
digitalWrite(15,LOW);digitalWrite(2,LOW);digitalWrite(4,LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

//      Firebase.setString(fbdo, "/tempH", "0");
//      Firebase.setString(fbdo, "/tempR", "0");
//      Firebase.setString(fbdo, "/pres", "0");
//      Firebase.setString(fbdo, "/smokeH", "0");
//      Firebase.setString(fbdo, "/smokeR", "0");
//      Firebase.setString(fbdo, "/TIME", "0");
  
}
void loop(){
  Firebase.getString(getdata1,"Fan");
  d1=getdata1.stringData();
  Serial.println(d1);
   
   if(d1==String(1) ||  fan_chk==1){
    digitalWrite(23,LOW);
   }
   else if(d1==String(0) ||  fan_chk==0){
   digitalWrite(23,HIGH);}
  Firebase.getString(getdata1,"Heater");
  d2=getdata1.stringData();
  Serial.println(d2);
     if(d2==String(1)){
    digitalWrite(22,LOW);   //volve on
    if(a==0){
    digitalWrite(19,LOW);  // spark on 
    delay(3000);
    digitalWrite(19,HIGH);
    a=1;
    }
   }
   if(d2==String(0) || temp_chk==1 || smk_chk==1){
    a=0;
   digitalWrite(22,HIGH);    //volv
   digitalWrite(19,HIGH);    //spark
   Firebase.setString(fbdo, "/Fan", "0");
   Firebase.setString(fbdo, "/Heater", "0");
   }
  Firebase.getString(getdata1,"case");
  d3=getdata1.stringData();
  String d3_=d3.substring(2,d3.length()-2);
  Serial.println(d3_);
if(Serial2.available()>0)
  {
        saj=Serial2.readStringUntil('$');
        Serial.println(saj);
        
       for(int i=0;i<saj.length();i++)
       {
        char f1=saj[i];
        
        if(f1==':')
        {
          d=d+1;
          }
         if(d==0)
         {
          tempH+=saj[i];
          }
         else if(d==1)
         {
          tempR+=saj[i];
          
          }
         else if(d==2)
         {
          pres+=saj[i];
          }
         else if(d==3)
         {
          smokeH+=saj[i];
          }
         else if(d==4)
         {
          smokeR+=saj[i];
          }
          else if(d==5)
         {
          TIME+=saj[i];
          }
          
        }
        saj="";
       // Serial.println(tempR);Serial.println(tempH); Serial.println(pres); Serial.println(smokeH);Serial.println(smokeR);Serial.println(TIME);
           
  
    tempR=tempR.substring(1,tempR.length());
    pres=pres.substring(1,pres.length());
    smokeH=smokeH.substring(1,smokeH.length());
    smokeR=smokeR.substring(1,smokeR.length());
    TIME=TIME.substring(1,TIME.length());
      Serial.println("outside case");
      Serial.println(pres.toInt());
      Serial.println(d2);
      Serial.println(TIME.toInt());
      Serial.println(pres.length());
    
    if(tempR.toInt()>d3_.toInt() && tempR.length()>=1)   // turn off heater when room temp increase than set value
    {
      temp_chk=1;
      }
      else
      {
        temp_chk=0;
        }
     
    if((smokeH.toInt()==0 || smokeR.toInt()==0) && (smokeH.length()>=1 && smokeR.length()>=1)) //  emergency case when gas leak
    {
        emergency();
      }
     else if((smokeH.toInt()==1 && smokeR.toInt()==1) && (smokeH.length()>=1 && smokeR.length()>=1))  // normal case check
     {
      digitalWrite(2,HIGH);
      digitalWrite(15,LOW);
      digitalWrite(4,LOW);
      //Serial.println("inside smoke2");
      //fan_chk=0;
      //smk_chk=0;
      }
     if ((pres.toInt() > 17000 && pres.toInt() < 20000) && d2==String(1) && TIME.toInt()==0 && pres.length()>=5){  // case for presure sensor and flow sensor=0
     emergency();
      }

      if (pres.toInt() <= 17000 && d2==String(1) && TIME.toInt()==0 && pres.length()>=5){  // case for presure sensor and volve
     emergency();
      }


      
      unsigned long current_mills=millis();
      if(current_mills-previous_mills>=interval){
        
      if (pres.toInt() > 17500 && d2==String(1) && TIME.toInt()>50 && tempH.toInt()<34 &&  pres.length()>=4){   //all sensor working but heater temp does not increase
        emergency();
      }
      
      previous_mills=current_mills;
      }
      
    if(tempR.length()>=1 && tempH.length()>=1 && pres.length()>=1 && smokeH.length()>=1 && smokeR.length()>=1 && TIME.length()>=1)
    {
      if(tempR.toInt()>1&&tempR.toInt()<35){
        Firebase.setInt(fbdo, "/hh", h.toInt());
      }
      else if (tempR.toInt()>36&&tempR.toInt()<66){
       Firebase.setInt(fbdo, "/hh", h1.toInt());

      }
      else if (tempR.toInt()>67&&tempR.toInt()<100){
       Firebase.setInt(fbdo, "/hh", h2.toInt());
      }


      if(tempH.toInt()>1&&tempH.toInt()<35){
        Firebase.setInt(fbdo, "/hh2", h.toInt());
      }
      else if (tempH.toInt()>36&&tempH.toInt()<66){
       Firebase.setInt(fbdo, "/hh2", h1.toInt());

      }
      else if (tempH.toInt()>67&&tempH.toInt()<100){
       Firebase.setInt(fbdo, "/hh2", h2.toInt());
      }

  //  Serial.println(tempR);Serial.println(tempH); Serial.println(pres); Serial.println(smokeH);Serial.println(smokeR);Serial.println(TIME);
    Firebase.setInt(fbdo, "/tempH", tempH.toInt());
    Firebase.setInt(fbdo, "/tempR", tempR.toInt());
    Firebase.setInt(fbdo, "/pres", pres.toInt());
    Firebase.setInt(fbdo, "/smokeH", smokeH.toInt());
    Firebase.setInt(fbdo, "/smokeR", smokeR.toInt());
    Firebase.setInt(fbdo, "/TIME", TIME.toInt());
      }
 }
tempH="";

tempR="";
smokeH="";
smokeR="";
pres="";
TIME="";
saj="";
d=0;
Serial.println(".............");

  
}



void emergency(){
      digitalWrite(2,LOW);      // green led
      digitalWrite(15,HIGH);  // buzzer
      digitalWrite(4,HIGH);  // red led
      digitalWrite(22,HIGH);    //volve
      digitalWrite(19,HIGH);    //spark
      digitalWrite(23,LOW);    //fan
      Firebase.setString(fbdo, "/Fan", "0");
      Firebase.setString(fbdo, "/Heater", "0");
                       // 
      delay(5000);
      digitalWrite(2,HIGH);      // green led
      digitalWrite(15,LOW);  // buzzer
      digitalWrite(4,LOW);  // red led
      digitalWrite(23,HIGH);    //fan

      }
