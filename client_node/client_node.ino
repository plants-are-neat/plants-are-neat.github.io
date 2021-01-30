// modified from rf22_mesh_server

#define RH_MESH_MAX_MESSAGE_LEN 50
#define RF95_FREQ 868.0

#include <RHMesh.h>
#include <RH_RF95.h>
#include <RHGenericDriver.h>
#include <RHRouter.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER1_ADDRESS 2
#define SERVER2_ADDRESS 3
#define SERVER3_ADDRESS 4

#define THERM A0
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

//radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);

// set up manager
RHMesh manager(driver, CLIENT_ADDRESS);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(9600);
  while(!Serial)
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

uint8_t data[] = "Hello World!";
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];

void loop()
{
  Serial.println("Sending to manager_mesh_server");
    
  if (manager.sendtoWait(data, sizeof(data), SERVER1_ADDRESS) == RH_ROUTER_ERROR_NONE)
  {
    uint8_t len = sizeof(buf);
    uint8_t from;    
    if (manager.recvfromAckTimeout(buf, &len, 3000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("No reply, is rf95_mesh_server1, rf95_mesh_server2, rf95_mesh_server3 running?");
    }
  }
  else
     Serial.println("sendtoWait failed. Are the intermediate mesh servers running?");
}
