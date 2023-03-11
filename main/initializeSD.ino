// SD card via initialisation SPI card access.
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <math.h>
#include <Wire.h>

// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 3
//
// Set DISABLE_CHIP_SELECT to disable a second SPI device.
// For example, with the Ethernet shield, set DISABLE_CHIP_SELECT
// to 10 to disable the Ethernet controller.
const int8_t DISABLE_CHIP_SELECT = -1;
//
// Test with reduced SPI speed for breadboards.  SD_SCK_MHZ(4) will select
// the highest speed supported by the board that is not over 4 MHz.
// Change SPI_SPEED to SD_SCK_MHZ(50) for best performance.
#define SPI_SPEED SD_SCK_MHZ(4)

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
#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

char buffer [64]; // must be large enough for your whole string!

// input buffer for line
char cinBuf[40];
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));
bool firstTry = true;
int index = 0;
// SD card chip select
int chipSelect;

void cardOrSpeed() {
  Serial.print("Try another SD card or reduce the SPI bus speed.\n");
  Serial.print("Edit SPI_SPEED in this program to change it.\n");
}

void clearSerialInput() {
  uint32_t m = micros();
  do {
    if (Serial.read() >= 0) {
      m = micros();
    }
  } while (micros() - m < 10000);
}

void reformatMsg() {
  Serial.print("Try reformatting the card.  For best results use\n");
  Serial.print("the SdFormatter program in SdFat/examples or download\n");
  Serial.print("and use SDFormatter from www.sdcard.org/downloads.\n");
}


void initSD() {
  index+=1;
  // Read any existing Serial data.
  clearSerialInput();

  firstTry = false;
 int chipSelect=10;

  if (DISABLE_CHIP_SELECT < 0) {
    Serial.print("\nAssuming the SD is the only SPI device.\n");
    Serial.print("Edit DISABLE_CHIP_SELECT to disable another device.\n");
  } else {
    sprintf(buffer,"\nDisabling SPI device on pin %i ",DISABLE_CHIP_SELECT);
    Serial.print(buffer);
    pinMode(DISABLE_CHIP_SELECT, OUTPUT);
    digitalWrite(DISABLE_CHIP_SELECT, HIGH);
  }
  if (!sd.begin(chipSelect, SPI_SPEED)) {
    if (sd.card()->errorCode()) {
      Serial.print("\nSD initialization failed.\n");
      Serial.print("Do not reformat the card!\n");
      Serial.print("Is the card correctly inserted?\n");
      Serial.print("Is chipSelect set to the correct value?\n");
      Serial.print("Does another SPI device need to be disabled?\n");
      Serial.print("Is there a wiring/soldering problem?\n");
      return;
    }
    Serial.print("\nCard successfully initialized.\n");
    if (sd.vol()->fatType() == 0) {
      Serial.print("Can't find a valid FAT16/FAT32 partition.\n");
      reformatMsg();
      return;
    }
    Serial.print("Can't determine error type\n");
    return;
  }
  Serial.print("\nCard successfully initialized.\n");

  uint32_t size = sd.card()->sectorCount();
  if (size == 0) {
    Serial.print("Can't determine the card size.\n");
    cardOrSpeed();
    return;
  }
  uint32_t sizeMB = 0.000512 * size + 0.5;
  sd.ls(LS_R | LS_DATE | LS_SIZE);

  if ((sizeMB > 1100 && sd.vol()->sectorsPerCluster() < 64)
      || (sizeMB < 2200 && sd.vol()->fatType() == 32)) {
    Serial.print("\nThis card should be reformatted for best performance.\n");
    Serial.print("Use a cluster size of 32 KB for cards larger than 1 GB.\n");
    Serial.print("Only cards larger than 2 GB should be formatted FAT32.\n");
    reformatMsg();
    return;
  }
  // Read any extra Serial data.
  clearSerialInput();
}
