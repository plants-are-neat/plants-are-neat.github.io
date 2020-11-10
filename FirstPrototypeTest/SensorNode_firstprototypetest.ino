#include "Adafruit_VEML7700.h"
#include "Adafruit_BME280.h"
#include "Adafruit_SGP30.h"
#include <SPI.h>
#include <RH_RF95.h>

#define THERM A0;
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define RF95_FREQ 868.0

const int ID = 1;
int lastID;

// Instantiate new sensor and radio objects
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_BME280 bme;
Adafruit_SGP30 sgp = Adafruit_SGP30();
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
	Serial.begin(115200);

	// Setup Analog Pin
	pinMode(THERM, INPUT);
	
	// Setup VEML7700
	veml.begin();
	veml.setGain(VEML7700_GAIN_1);
	veml.setIntegrationTime(VEML7700_IT_25MS);
	
	// Setup BME280
	bme.begin();
	
	// Setup SGP30
	sgp.begin();
	
	// Setup LoRa
	initializeLoRa();
	lastID = -1;
}

void loop() {
	// Activated and listening somehow
	char datapts[RH_RF95_MAX_MESSAGE_LEN];
	String reading;
	bool gotData = listen(datapts);
	
	if (gotData) {
		if ((int)datapts[0] == ID) { // If the ID in the message matches this node's ID
			reading = collectData(); // read data from sensors and...
			char toSend[reading.length()]; 
			reading.toCharArray(toSend, reading.length());
			respond(toSend); // send it out
			lastID = (int)datapts[0]; // TO ADD HERE: need something to make sure that if a node receives the same message twice, it doesn't echo it the second time
		} else { // otherwise...
			respond(datapts); // echo the message for other sensor nodes
		}
	}	
}


String collectData() {
	// Get soil temperature with thermistor
	float resistance = analogRead(THERM);
	resistance = (1023 / resistance) - 1;
	resistance = 10000.0 / resistance;
	float B = 3935; // depends on the NTC model
	resistance = log(resistance / 10000) / B;
	resistance += 1.0 / (25 + 273.15);
	resistance = 1.0 / resistance;
	resistance -= 273.15;
	resistance = resistance * (9.0 / 5.0) + 3;
	int soiltemp = round(resistance);
	
	// Get lux from VEML7700
	int lux = (int)round(veml.readLux());

	// Get temp, pressure, humidity from BME280
	int temp = (int)round(bme.readTemperature() * (9.0 / 5.0) + 32);
	int pressure = bme.readPressure(); // in Pa
	int humidity = (int)round(bme.readHumidity());
	
	// Get eCO2 from SGP30
	int eco2 = (int)round(sgp.eCO2);
	
	// Return data
	String results = String(ID) + "," + String(temp) + "," + String(humidity) + "," + String(pressure) + "," + String(lux) + "," + String(eco2) + "," + String(soiltemp) + "\n";
	return results;
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
	
	rf95.setTxPower(23, false);
}

bool listen(char data[]) {
	if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
     
        if (rf95.recv(buf, &len)) {
			RH_RF95::printBuffer("Received: ", buf, len);
			Serial.print("Got: ");
			Serial.println((char*)buf);
			Serial.print("RSSI: ");
			Serial.println(rf95.lastRssi(), DEC);
     
			data = (char *)buf;
			return true;
        } else {
          Serial.println("Receive failed");
		  return false;
        }
    }
	return false;
}

bool respond(char data[]) {
	Serial.println("Sending...");
	//for (int i = 0; i < 10; i++) {
		delay(10);
		rf95.send((uint8_t *)data, 20);
		delay(10);
		rf95.waitPacketSent();
	//}
}
