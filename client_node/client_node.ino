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

// set up manager
RHMesh manager(driver, CLIENT_ADDRESS);

void setup() 
{
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("init failed");
    
  driver.setTxPower(23, false);
  driver.setFrequency(RF95_FREQ);
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
      Serial.println("No reply, is rf95_mesh_server1 running?");
    }
  }
  else
     Serial.println("sendtoWait failed. Are the intermediate mesh servers running?");
}
