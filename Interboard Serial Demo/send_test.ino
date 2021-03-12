#include <Wire.h>

void setup(){
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

byte x = 0;

void loop(){
  String s = "plant data ";
  int len = s.length();
  char buf[len];
  s.toCharArray(buf, len);
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(buf);        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(500);
}
