#include <Wire.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>

Adafruit_LPS25 lps;

void setup(void) {
  Serial.begin(115200); 

  lps.begin_I2C();  //initialize i2c

  lps.setDataRate(LPS25_RATE_1_HZ); //we don't need updated data more than once per second, 1Hz is more than sufficient for a data rate

  //lps.setDataRate(LPS25_RATE_ONE_SHOT); depending on how we want to have data trans work (ie sensors always reading, only transmit when pinged, vs only read & transmit when pinged)
}

void loop() {
  sensors_event_t temp;
  sensors_event_t pressure;
  lps.getEvent(&pressure, &temp);// get pressure and temp
  
  Serial.print("Temperature: ");
  Serial.print(((1.8)*(temp.temperature))+32);  //celcius -> fahrenheit conversion
  Serial.println(" degrees F");

  Serial.print("Pressure: ");Serial.print(pressure.pressure);Serial.println(" hPa");
  Serial.println("");
  delay(1000);
}
