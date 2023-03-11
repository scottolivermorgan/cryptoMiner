// Log data to SD card.
void writeData(String timeStamp, String source,String voltage) {
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = sd.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    String data = timeStamp + ',' + source + ',' + voltage;
    myFile.println(data);
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
