// modified from rf22_mesh_server


#define RH_MESH_MAX_MESSAGE_LEN 50
#define RF95_FREQ 868.0 

#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER1_ADDRESS 2

//radio driver
RH_RF95 driver;

//set up manager
RHMesh manager(driver, SERVER1_ADDRESS);


void setup() 
{
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("RF95 init failed");

  driver.setTxPower(23, false);
  driver.setFrequency(RF95_FREQ);
}



uint8_t data[] = "And hello back to you from server1";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
void loop()
{
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char*)buf);

    // Send a reply back to the originator client
    if (manager.sendtoWait(data, sizeof(data), from) != RH_ROUTER_ERROR_NONE)
      Serial.println("sendtoWait failed");
  }
}
