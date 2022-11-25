
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLfMVeDNKk"
#define BLYNK_DEVICE_NAME "HE THONG TUOI TIEU TU DONG"
//#define BLYNK_AUTH_TOKEN "8QkzG6boWySQ3JLJHzDRaRVgRbkNcn3P"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <SoftwareSerial.h>
SoftwareSerial esp(D2,D3);//D2=RX  D3=TX

void setup()
{
  Serial.begin(115200);
  delay(100);
  esp.begin(115200);
  BlynkEdgent.begin();
}
//BLYNK_CONNECTED() { // checks if there is a connection to Blynk.Cloud  
//  Blynk.syncVirtual(V1); // get the latest value
//  Blynk.syncVirtual(V2);
//}
BLYNK_WRITE(V1){
  int i = param.asInt();
  mod(i);
  if(i==0){
    Blynk.setProperty(V0,"isDisabled",true);
//    Serial.println("pump disable");
  }
  else{
    Blynk.setProperty(V0,"isDisabled",false);
//    Serial.println("pump enable");
  }
}
BLYNK_WRITE(V0){
  int i = param.asInt();
  pump(i);
//  if(i==0){
//    Serial.println("pump off");
//  }
//  else{
//    Serial.println("pump on");
//  }
}
void mod(int i){
  esp.write("0",i);
}
void pump(int i){
  esp.write("1",i);
}
void getData(int data[2]){
  String dataRaw;
  String tmp="";
  dataRaw = esp.readString();
//  dataRaw = String(random(0,100))+","+String(random(0,100));
  for(int i =0 ; i<dataRaw.length(); i++){
    if((dataRaw.charAt(i)!=',')){
      tmp+=dataRaw.charAt(i);
    }
    else{
      Serial.print("actual ");
      
      data[0] = tmp.toInt();
      Serial.println(data[0]);
      tmp="";
      for(int j = i+1;j<dataRaw.length();j++){
        tmp+=dataRaw.charAt(j);
      }
      Serial.print("setting");
      
      data[1]=tmp.toInt();
      Serial.println(data[1]);
      break;
    }
  }
}
void loop() {
  BlynkEdgent.run();
  int data[2];
  getData(data);
  Blynk.setProperty(V3,"isDisabled",false);
  Blynk.virtualWrite(V3,data[0]);
  Blynk.setProperty(V2,"isDisabled",false);
  Blynk.virtualWrite(V2,data[1]);
  delay(1500);
}
