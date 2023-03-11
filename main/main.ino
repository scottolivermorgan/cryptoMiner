// Quick hardware test for SPI card access.
//
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <math.h>
#include <Wire.h>
#include <DS3231.h>

#if SD_FAT_TYPE == 0
SdFat sd;
File file;
#elif SD_FAT_TYPE == 1
SdFat32 sd;
File32 file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile file;
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

File myFile;
DS3231 clock;
RTCDateTime dt;
int sourceA = 6;    //Solar
int sourceB = 5;     //Mains
float voltage = 0.0;
String Source = "mains"; 
float lowerThreashold = 12.5 ;
float upperThreashold = 13.5 ;

char buffer2[5];

void setup() {
  Serial.begin(9600);
   // Wait for USB Serial
  while (!Serial) {
    yield();
  }
  // Initialize DS3231
  clock.begin();
  // Send sketch compiling time to Arduino
  clock.setDateTime(__DATE__, __TIME__);  
  
  pinMode(sourceA, OUTPUT);
  pinMode(sourceB, OUTPUT);
  pinMode(10, OUTPUT); // change this to 53 on a mega
  
  digitalWrite(10, HIGH); // Add this line
  
  clock.begin();
  // Send sketch compiling time to Arduino
  clock.setDateTime(__DATE__, __TIME__); 

  mains();
  String currentTime = getTime();
  initSD();
  dtostrf((voltage),5,2,buffer2);
  writeData(currentTime,Source,buffer2);
}
/*
float truncate(float val, byte dec) 
{
    float x = val * pow(10, dec);
    float y = round(x);
    float z = x - y;
    if ((int)z == 5)
    {
        y++;
    } else {}
    x = y / pow(10, dec);
    return x;
}

*/

void loop() {
  voltage = measureVoltage();
  String currentTime = getTime();

  if((voltage <= lowerThreashold) && (Source = "solar")){
    Source = "mains";
    Serial.print(voltage);
    Serial.print(Source);
    mains();
    //initSD();
    dtostrf((voltage),5,2,buffer2);
    writeData(currentTime,Source,buffer2);
  }

  else if((voltage >= upperThreashold) && (Source = "mains")){
    Source = "solar";
    Serial.print(voltage);
    Serial.print(Source);
    solar();
    //initSD();
    dtostrf((voltage),5,2,buffer2);
    writeData(currentTime,Source,buffer2);
  };

  delay(1000);
}

  
/*
  //Serial.print(voltage * 11.132);

   if (voltage * 11.132<=12.05){
      Serial.print(voltage * 11.132);
       //Serial.print("Source = mains");
       String Source = "mains";
       mains();
       initSD();
       dtostrf((voltage*11.132),5,2,buffer2);
       writeData(currentTime,Source,buffer2);
       }
  if (voltage * 11.132>12.05){
      Serial.print(voltage * 11.132);
       //Serial.print("Source = Solar");
       String Source = "solar";
       solar();
       initSD();
       dtostrf((voltage*11.132),5,2,buffer2);
       writeData(currentTime,Source,buffer2);
       }

  delay(1000);
  }
*/

/*



void loop() {
  // put your main code here, to run repeatedly:
  voltage=measureVoltage();
  Serial.print(voltage * 11.132);
  Serial.println (" V");
  if (voltage * 11.132<=12.05){
    Serial.print("Source = mains");
    mains();
  ;}
  if (voltage * 11.132>12.05){
    Serial.print("Source = Solar");
    solar();
  ;}
}



*/
