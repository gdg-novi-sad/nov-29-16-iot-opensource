#include <MQ135.h>
#include <Wire.h>
#include "cactus_io_BME280_I2C.h"

BME280_I2C bme(0x76);
MQ135 gasSensor = MQ135(PF0);
String inputString = "";  
boolean stringComplete;
String addr="";

// the setup function runs once when you press reset or power the board
void setup() {
  Serial3.begin(57600);
  Serial.begin(9600);
  RN2483_init();
  
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  bme.setTempCal(-7);
  
}

// the loop function runs over and over again forever
void loop() {
  float ppm = gasSensor.getPPM();
  //float rzero = gasSensor.getRZero(); Calibration
  //delay(1000);                       // wait for a second

  bme.readSensor();

  String printVal = "" + String(bme.getPressure_MB()) + "F" + String(bme.getHumidity()) + "F" + 
  String(bme.getTemperature_C()) + "F" + String(ppm) + "F";

  String converted = printVal;
  converted.replace(".","C");
  Serial.println("Addr is");
  Serial.println(addr);
  String is = "radio tx " + converted + "\r\n";
  sendmsg(is);


  
  delay(3000);  
}

void RN2483_init(){
  sendcmd("sys reset");
//  sendcmd("radio set mod lora");
//  sendcmd("radio set freq 868100000");
//  sendcmd("radio set pwr 14");
//  sendcmd("radio set sf sf12");
//  sendcmd("radio set afcbw 125");
//  sendcmd("radio set rxbw 250");
//  sendcmd("radio set fdev 5000");
//  sendcmd("radio set prlen 8");
//  sendcmd("radio set crc on");
//  sendcmd("radio set cr 4/8");
//  sendcmd("radio set wdt 0");
//  sendcmd("radio set sync 12");
//  sendcmd("radio set bw 250");
  sendcmd("sys get hweui");
  sendcmd("mac pause");
}

void sendcmd(String data){
  Serial.println(data);
  Serial3.println(data); 
  delay(200);
  Serial.println(Serial3.readStringUntil('\n'));
}

void sendmsg(String data){
  Serial.println(data);
  Serial3.println(data); 
  delay(200);
  // radio rx command has two responses, command and transmision
  Serial.println(Serial3.readStringUntil('\n')); // command response
  Serial.println(Serial3.readStringUntil('\n')); // transmision repsonse
  
}

void getSerial(){
  Serial.println("sys get hweui");
  Serial3.println("sys get hweui"); 
  delay(200);
  // radio rx command has two responses, command and transmision
  Serial.println(Serial3.readStringUntil('\n')); // command response
  //Serial.println(Serial3.readStringUntil('\n')); // transmision repsonse
  addr = Serial3.readStringUntil('\n');
  Serial.println(addr);
}

String strtohex(String data){
  String sh;
  char ch;
  for (int i=0;i<data.length();i++){
    ch = data.charAt(i);
    sh += String(ch,HEX);
  }
  return sh;
}
