#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial esp(2,3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int sensor = A0;
int pot = A2;
int relay = 8;
void setup() {
  pinMode(relay, OUTPUT);
  lcd.init();
  lcd.backlight();
}
void automatic(){
  if (sensorPercent < potPercent) {
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }
}
void handle(int button){
  digitalWrite(relay,button);
}
void sendData(int sensor, int pot){
  String dataRaw = String(sensor)+","+String(pot);
  esp.print(dataRaw);
}
int getMode(String value){
  if(value.charAt(0)==0){
    return value.charAt(1).toInt(); 
  }
  else {
    return 2;
  }
}
int getButton(String value){
  if(value.charAt(0)==1){
    return value.charAt(1).toInt();
  }
}
void loop() {
  lcd.clear();
  //Đọc giá trị và chuyển đổi sang phần trăm
  int sensorValue = analogRead(sensor);
  int sensorPercent = map(sensorValue, 0, 1023, 100, 0);
  int potValue = analogRead(pot);
  int potPercent = map(potValue, 0, 1023, 0, 100);
  //So sánh
//  if (sensorPercent < potPercent) {
//    digitalWrite(relay, HIGH);
//  } else {
//    digitalWrite(relay, LOW);
//  }
   sendData(sensorValue,potValue);
   String value;
   if(esp.available()>0){
    value=esp.readString();
    if(getMode(value)==0){
      handle(getButton(esp.readString());
    }
    else if(getMode(value)==1){
      automatic();
    }
   }
  //Hiển thị giá trị cảm biến lên màn hình lcd
  lcd.setCursor(0, 0); //Đặt vị trí con trỏ ở cột 1 hàng 1
  lcd.print("Do am: "); //In lên màn hình lcd "Do am: "
  lcd.setCursor(7, 0); //Đặt vị trí con trỏ ở cột 8 hàng 1
  lcd.print(sensorPercent); //In lên màn hình lcd giá trị cảm biến
  lcd.print("%");
  //Hiển thị giá trị biến trở lên màn hình lcd
  lcd.setCursor(0, 1); //Đặt vị trí con trỏ ở cột 1 hàng 2
  lcd.print("Chon do am: "); //In lên màn hình lcd "Chon do am: "
  lcd.setCursor(12, 1); //Đặt vị trí con trỏ ở cột 13 hàng 2
  lcd.print(potPercent); //In lên màn hình lcd giá trị biến trở
  lcd.print("%");
  delay(100); //Ngưng 100ms rồi lặp lại
}
