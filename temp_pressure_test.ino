#include <Wire.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>

Adafruit_LPS25 lps;

void setup(void) {
  Serial.begin(115200); 

  lps.begin_I2C();

  lps.setDataRate(LPS25_RATE_ONE_SHOT);
}

void loop() {
  sensors_event_t temp;
  sensors_event_t pressure;
  lps.getEvent(&pressure, &temp);// get pressure and temp
  
  Serial.print("Temperature: ");
  Serial.print(((1.8)*(temp.temperature))+32);
  Serial.println(" degrees F");

  

  Serial.print("Pressure: ");Serial.print(pressure.pressure);Serial.println(" hPa");
  Serial.println("");
  delay(100);
}
