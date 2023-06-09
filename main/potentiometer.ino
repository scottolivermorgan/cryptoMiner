// Measure voltage using voltage divider.

float measureVoltage(){
  // number of analog samples to take per reading
  #define NUM_SAMPLES 10

  int sum = 0;                    // sum of samples taken
  unsigned char sample_count = 0; // current sample number
  float voltage = 0.0;            // calculated voltage
  
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES) {
     sum += analogRead(A2);
     sample_count++;
     delay(10);
  }
  
  // calculate the voltage
  // use 5.0 for a 5.0V ADC reference voltage
  // 5.015V is the calibrated reference voltage
  
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.015) / 1024.0;
  
  // send voltage for display on Serial Monitor
  // voltage multiplied by 11 when using voltage divider that
  // divides by 11. 11.132 is the calibrated voltage divide
  // value

  return voltage* 11.132; }
