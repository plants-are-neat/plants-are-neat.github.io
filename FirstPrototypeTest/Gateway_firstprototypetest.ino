#include <SPI.h>
#include <RH_RF95.h>

#define THERM A0;
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define RF95_FREQ 868.0

void setup() {
	Serial.begin(115200);

	// Setup LoRa
	initializeLoRa();
}

void loop() {
	
}

void initializeLoRa() {
	digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
     
    while (!rf95.init()) {
      Serial.println("LoRa radio init failed");
      Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
      while (1);
    }
    Serial.println("LoRa radio init OK!");
     
    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!rf95.setFrequency(RF95_FREQ)) {
      Serial.println("setFrequency failed");
      while (1);
    }
    Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
	
	rf95.setTxPower(23, false)
}

bool listen(char* data[]) {
	if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
     
        if (rf95.recv(buf, &len)) {
			RH_RF95::printBuffer("Received: ", buf, len);
			Serial.print("Got: ");
			Serial.println((char*)buf);
			Serial.print("RSSI: ");
			Serial.println(rf95.lastRssi(), DEC);
     
			data = (char*)buf;
			return true;
        } else {
          Serial.println("Receive failed");
		  return false;
        }
    }
	return false;
}

void poll(int nodeID) {
	String toSend = String(nodeID) + " poll";
	int whitespace = 20 - length(toSend);
	for (int i = 0; i < whitespace; i++) {
		toSend += " ";
	}
	char radiopacket[20] = toSend.toCharArray(toSend, length(toSend));
    //itoa(packetnum++, radiopacket+13, 10);
    Serial.print("Sending "); Serial.println(radiopacket);
    radiopacket[19] = 0;
      
    Serial.println("Sending...");
    delay(10);
    rf95.send((uint8_t *)radiopacket, 20);
     
    Serial.println("Waiting for packet to complete..."); 
    delay(10);
    rf95.waitPacketSent();
}