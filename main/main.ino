/* Program measures voltage of battery and switches relays 
between mains and solar power, logging usage data to SD card. Has logical
voltage buffer to stop ocillation around switching threashold*/

#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <math.h>
#include <Wire.h>
#include <DS3231.h>

// Initialise params for various SD types.
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


File myFile;  //SD output file
char buffer2[5];

DS3231 clock; // timing module
RTCDateTime dt;

int sourceA = 6;    //Solar
int sourceB = 5;     //Mains

float voltage = 0.0;  // Voltage measurement and switching params.
String Source = "mains"; 

float lowerThreashold = 12.5 ;  // below which switch system to mains power.
float upperThreashold = 13.5 ;  // above which switch system to solar power.


void setup() {
  Serial.begin(9600);
   // Wait for USB Serial
  while (!Serial) {
    yield();
  }

  clock.begin();   // Initialize DS3231
  clock.setDateTime(__DATE__, __TIME__);  // Send sketch compiling time to Arduino
  pinMode(sourceA, OUTPUT);   // Initialise signal pin for relay directing to solar power.
  pinMode(sourceB, OUTPUT);   // Initialise signal pin for relay directing to mains power.

  pinMode(10, OUTPUT); // Required for SD card shield.
  digitalWrite(10, HIGH); // 
  
  //clock.begin();
  // Send sketch compiling time to Arduino
  //clock.setDateTime(__DATE__, __TIME__); 
 
  // initialise and log settings to mains power.
  mains();
  String currentTime = getTime();
  initSD();
  dtostrf((voltage),5,2,buffer2);
  writeData(currentTime,Source,buffer2);
}

void loop() {
  // get voltage & current date time.
  voltage = measureVoltage();
  Serial.print(voltage);
  String currentTime = getTime();

  // If true, switch to mains voltage and charge battery.
  if((voltage <= lowerThreashold) && (Source = "solar")){
    Source = "mains";
    mains();
    dtostrf((voltage),5,2,buffer2);
    writeData(currentTime,Source,buffer2);
  }

  // if true battery charged, switch back to solar power.
  else if((voltage >= upperThreashold) && (Source = "mains")){
    Source = "solar";
    solar();
    dtostrf((voltage),5,2,buffer2);
    writeData(currentTime,Source,buffer2);
  };
  delay(1000);
};
