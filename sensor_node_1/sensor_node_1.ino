// modified from rf22_mesh_server 
#define RH_MESH_MAX_MESSAGE_LEN 50
#define RF95_FREQ 915.0

#include <RHMesh.h>
#include <RH_RF95.h>
#include <RHGenericDriver.h>
#include <RHRouter.h>
#include <SPI.h>
#define THERM A2
#define VH400 A5
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHT31.h"
#include <Adafruit_SleepyDog.h>

#define CLIENT_ADDRESS 1
#define SERVER1_ADDRESS 2
#define SERVER2_ADDRESS 3
#define SERVER3_ADDRESS 4

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

//radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);

// set up manager
RHMesh manager(driver, SERVER1_ADDRESS);

// Initialization of sensors
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_SHT31 sht = Adafruit_SHT31();

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(9600);
  while(!Serial)
  pinMode(THERM, INPUT);
  pinMode(VH400, INPUT);
  // Setup VEML7700
  veml.begin();
  veml.setGain(VEML7700_GAIN_1_8); // largest light range
  veml.setIntegrationTime(VEML7700_IT_100MS);
  // Setup SHT31-D
  sht.begin(0x44);
  if (!manager.init())
    Serial.println("init failed");
  while (!driver.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
     
 // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!driver.setFrequency(RF95_FREQ)) {
   Serial.println("setFrequency failed");
   while (1);
   }
   Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
   driver.setTxPower(23, false);
}


// Collect Data function
String collectData() {
  // Get soil temperature with thermistor
  float resistance = analogRead(THERM); // get 0-1023 value from thermistor
  resistance = (1023 / resistance) - 1;
  resistance = 10000.0 / resistance; // get thermistor actual resistance
  float B = 3935; // depends on the NTC model
  float soiltemp = log(resistance / 10000) / B;
  soiltemp += 1.0 / (25 + 273.15);
  soiltemp = 1.0 / soiltemp;
  soiltemp -= 273.15; // apply thermistor characteristic B parameter equation
  soiltemp = soiltemp * (9.0 / 5.0) + 32; // units of degrees F
  soiltemp = round(soiltemp * 10) / 10; // one decimal place
  
  // Get soil moisture with VH400
  float moisture = analogRead(VH400); // get 0-1023 value from VH400
  float maxMoistureVoltage = (3 * 1023) / 3.3; // VH400 output voltage is 0V - 3V
  moisture = map(moisture, 0, maxMoistureVoltage, 0, 100); // units of %
  moisture = round(moisture * 10) / 10; //one decimal place
  
  // Get lux from VEML7700
  float lux = veml.readLux(); // units of Lux
  lux = round(lux * 10) / 10; // one decimal place

  // Get temp and humidity from SHT31-D
  float temp = sht.readTemperature() * (9.0 / 5.0) + 32; // units of degrees F
  temp = round(temp * 10) / 10; // one decimal place
  float humidity = sht.readHumidity(); // units of %
  humidity = round(humidity * 10) / 10; // one decimal place
  
  // Return data
  String results = String(temp) + "," + String(humidity) + "," + String(lux) + "," + String(moisture) + "," + String(soiltemp);
  return results;
}
void Sleep(int Time)
  { // Determine the amount of loops needed to reach the goal of sleeping for "Time"
    int rem = 0, stime = 0;
    rem = Time % 10;
    stime = (Time-rem)/10;
    if(stime >= 1) // In case sleeping is only needed less than 10 s
    {
      for(int i = 0; i < stime; i++)
        Watchdog.sleep(10000);
    }
    
    if(rem > 0) // In case of remainder being 0 (multiple of 10)
      Watchdog.sleep(rem*1000);
  }


uint8_t data[50];
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
int sleepFlag = 0;
void loop()
{
  int i;
  uint8_t len = sizeof(buf);
  uint8_t from;
  String datapoints = collectData();
  for(i = 0; i < sizeof(data); i++)
    data[i] = 0;
  for(i = 0; i < datapoints.length(); i++)
    data[i] = datapoints[i];
  if (manager.recvfromAck(buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.println(": ");
    Serial.println((char*)buf);

    // Send a reply back to the originator client
    if (manager.sendtoWait(data, sizeof(data), from) == RH_ROUTER_ERROR_NONE)
      sleepFlag = 1; // When it sends the information successfully, go to sleep
    else
      Serial.println("sendtoWait failed");
  }
 for(i = 0; i < sizeof(buf); i++) // Reset
    buf[i] = 0;
    
 if(sleepFlag){
  Sleep(300); // Sleeps for 30 secs
  sleepFlag = 0;
 }
}
