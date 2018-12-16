#include "SoftwareSerial.h"

// Servo için
#include <Servo.h>

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
String Myid="00000"; //oyuncağın hafızasındaki telefona yazılacak telefonda da id olarak tutulacak
String Kuid="";
String Myidt="";
String Kuidt="";
String Komut="";
String Deger="";
String Asama="0";

  char msg[1023];
  //char msgm[1023];


void setup() {
  pinMode(3,OUTPUT);
  pinMode(9,OUTPUT);
  Servo01.attach(3);
  Servo02.attach(9);
  esp.begin(19200);
  Serial.begin(115200);
  
}

void loop (){

    if (esp.available()) {
      char ec = esp.read();
      /// Serial.println("Received character:");
      /// Serial.println(request);
      //Serial.print(ec);
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
    if(espinBuf!="" & Asama=="0"){
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

if(espinBuf!="" & Asama=="2")
{
  Serial.println(espinBuf);
  int ara=espinBuf.indexOf("=");
Kuidt=espinBuf.substring(0,ara);
  int son=espinBuf.indexOf(";");
Myidt=espinBuf.substring(ara+1,son);
//Serial.print(Kuidt +" "+ Myidt);
if(Kuidt==Kuid & Myidt==Myid)
{

// işlemler burada yapılacak
    while(espinBuf.length()>0)
    {
        //keselim öncesini
     espinBuf=espinBuf.substring(son+1,espinBuf.indexOf("|")+1);

     int ara=espinBuf.indexOf("=");
      Komut=espinBuf.substring(0,ara);
     int son=espinBuf.indexOf(";");
      Deger=espinBuf.substring(ara+1,son);
        Serial.println(Komut);

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
     if(espinBuf.length()<2){espinBuf="";}
    }

}else{espinBuf="";}

}
// Servolar
// servo01
if(Servo01milis!=0){
        Servo01.write(Servo01deg);
        Servo01milis-=1;
  }
//servo02
if(Servo02milis!=0){
        Servo02.write(Servo02deg);
        Servo02milis-=1;
  }

if(espout!=""){
  //espout.toCharArray(msgm, espout.length());
  //snprintf (msg, 1023, msgm , espout.length());
  esp.print(espout);
  Serial.println(espout);
  espout="";
  }



 

// bura monitorden giriş içindi.
// **************************************************************
//  Servo01.write(Servo01deg);
//      delay(10);
//    if (Serial.available()) {
//    char request = Serial.read();
//    /// Serial.println("Received character:");
//    /// Serial.println(request);
    
//    esp.print(request);
//  }
// ***************************************************************
}




