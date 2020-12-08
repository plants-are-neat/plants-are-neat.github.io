//starting to figure out how this mesh thingy works

#include <RHMesh.h>     //for the mesh network
#include <RH_RF95.h>    //for the LoRa radio

#define RF95_FREQ 868.0 //frequency we're operating on, default 434 MHz not what we're using

//radio driver
RH_RF95 rf95;

//just setting up a nodeID for now, since it's needed as an input to set up the mesh manager
uint8_t nodeID = 1;

//set up manager
RHMesh manager(rf95, nodeID);

void setup() {
  Serial.begin(9600);
  if (!manager.init()){
    Serial.println("RF95 initialization failed");
  }

  rf95.setFrequency(868.0);
  rf95.setTxPower(23, false);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
