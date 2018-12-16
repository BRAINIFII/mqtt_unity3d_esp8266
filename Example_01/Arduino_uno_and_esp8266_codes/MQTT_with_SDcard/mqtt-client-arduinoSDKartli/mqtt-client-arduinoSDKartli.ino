#include "SoftwareSerial.h"
 
// Servo için
#include <Servo.h>
/*
pinMode(3,OUTPUT);
Servo01.attach(3);
pinMode(9,OUTPUT);
Servo02.attach(9);
*/

//SD kart için
#include <SPI.h>
#include <SD.h>

Servo Servo01;
int Servo01deg;
int Servo01milis;

Servo Servo02;
int Servo02deg;
int Servo02milis;


SoftwareSerial esp(2, 5);
String espin;
String espout;
String espinBuf;
int espok=0;
String Myid="00000"; //oyuncağın hafızasındaki telefona yazılacak telefonda da id olarak tutulacak
String Kuid="";
String Myidt="";
String Kuidt="";
String Komut="";
String Deger="";
String Asama="0";

  char msg[1023];
  //char msgm[1023];

File myFile;


void setup() {
  esp.begin(19200);
  Serial.begin(115200);
  delay(200);
  while (!Serial) {
     ; // wait for serial port to connect. Needed for native USB port only
   }
     Serial.print("Initializing SD card...");
   if (!SD.begin(4)) {
     Serial.println("initialization failed!");
     while(1);
   }
   Serial.println("initialization done.");
    myFile = SD.open("config.txt");
   if (myFile) {

     // read from the file until there's nothing else in it:
     while (myFile.available()) {
      char sdc=myFile.read();
          espin+=sdc;
     }

          if(espin.indexOf("<")>-1){
          if(espin.indexOf("myid=")>-1){
            String myt=espin.substring(espin.indexOf("myid=")+5);
            Myid=myt.substring(0,myt.indexOf(";"));
          Serial.println(Myid);
          }
          Serial.println("config.txt:");
          Serial.println(espin);
          esp.print(espin);
          espin="";
          }

     // close the file:
     myFile.close();
   } else {
     // if the file didn't open, print an error:
     Serial.println("error opening config.txt");
   }
   esp.print("<");
   while(espok==0){
        espok=1;
        if (esp.available()>0) {
          char ec = esp.read();
         Serial.print(ec);
         if(ec=='<'){
            espok=1;
           }
        }
   }


}

void loop (){
    if (espok==1 & esp.available()>0) {
      char ec = esp.read();
      /// Serial.println("Received character:");
      /// Serial.println(request);
      Serial.print(ec);
          if(espinBuf==""){
          espin+=ec;
          if(espin.indexOf("|")>-1){
            espin=espin.substring(espin.indexOf(">")+1,espin.length());
            espin=espin.substring(0,espin.indexOf("|")+1);
            espinBuf=espin;
            Serial.println();
            Serial.println(espinBuf);
            espin="";
            Serial.println("Servo01deg: " + Asama);
          }
        }
    }

//Aşama 1
// id no isteme > id no gönderir 
// id e senmisin sorusu > id yannda Blink komutu ile led yak 1000, söndür, 1000 yak söndür.
// ya da direk id e bağlandım mesajı olabilir. > 
    if(espok==1 & espinBuf!="" & Asama=="0"){
      if(Kuid==""){
          if(espinBuf.indexOf("=Myid?;|")>-1)
          { 
            String Kuidtmp= espinBuf.substring(0,espinBuf.indexOf("="));
            if(Kuidtmp.length()>1)   { 
            espout=Kuidtmp+"=<"+Myid+";|";
            espinBuf="";espin="";
           }
          }
  
          if(espinBuf.indexOf("=="+Myid+";|")>-1)
          {
            String Kuidtmp=espinBuf.substring(0,espinBuf.indexOf("="));
            espout=Kuidtmp+"=="+Myid+";|";
            Kuid=Kuidtmp;
            Asama="2";
            espinBuf="";espin="";
          }

      }   //Kuid Myid sorgusu
    }
if(espinBuf!="")Serial.println(Asama);

// BAĞLANTI TAMAMLANDI DOĞRU KUMANDA İD VE ARAÇ İD Mİ ********
if(espok==1 & espinBuf!="" & Asama=="2")
{
  Serial.println(espinBuf);
  int ara=espinBuf.indexOf("=");
Kuidt=espinBuf.substring(0,ara);
  int son=espinBuf.indexOf(";");
Myidt=espinBuf.substring(ara+1,son);
//Serial.print(Kuidt +" "+ Myidt);
// BAĞLANTI TAMAMLANDI DOĞRU KUMANDA İD VE ARAÇ İD İSE ********
if(Kuidt==Kuid & Myidt==Myid)
{

// BAĞLANTI TAMAMLANDI İŞLEMLER AŞAĞIDA YAPILACAK **************************
    while(espinBuf.length()>0)
    {
        //keselim öncesini
     espinBuf=espinBuf.substring(son+1,espinBuf.indexOf("|")+1);

     int ara=espinBuf.indexOf("=");
      Komut=espinBuf.substring(0,ara);
     int son=espinBuf.indexOf(";");
      Deger=espinBuf.substring(ara+1,son);
        Serial.println(Komut);
// KOMUTLARIN ANALİZİ BAŞI +++++++++++++++++++++++++++++++++++++
      if(Komut=="SERVO01")
      {
      Servo01deg=Deger.toInt();
      Servo01milis=abs(Servo01deg-Servo01milis)*1;
        //Servo01.write(Servo01deg);
      }

      if(Komut=="SERVO02")
      {
      Servo02deg=Deger.toInt();
      Servo02milis=abs(Servo02deg-Servo02milis)*1;
        //Servo02.write(Servo02deg);
      }


      if(Komut=="KUIDKILL")
      {
      Kuid="";Asama="0";
      }
// KOMUTLARIN ANALİZİ SONU -------------------------------------
     if(espinBuf.length()<2){espinBuf="";}
    }
}else{espinBuf="";}
}


// KOMUTLARIN İCRA EDİLMESİ BAŞI +++++++++++++++++++++++++
// Servolar
// servo01
if(Servo01milis!=0){
    pinMode(3,OUTPUT);
      Servo01.attach(3);
        Servo01.write(Servo01deg);
        Servo01milis-=1;
  }
//servo02
if(Servo02milis!=0){
    pinMode(9,OUTPUT);
      Servo02.attach(9);
        Servo02.write(Servo02deg);
        Servo02milis-=1;
}
// KOMUTLARIN İCRA EDİLMESİ SONU -------------------------


// GERİ BİLDİRİMLER GÖNDERİLMESİ BAŞI ++++++++++++++++++
if(espok==1 & espout!=""){
  //espout.toCharArray(msgm, espout.length());
  //snprintf (msg, 1023, msgm , espout.length());
  esp.print(espout);
  Serial.println(espout);
  espout="";
}
// GERİ BİLDİRİMLER GÖNDERİLMESİ SONU -----------------

// BAĞLANTI TAMAMLANDI İŞLEMLER YUKARIDA YAPILACAK **************************
}




