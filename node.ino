// Include header files and define analog pin
#define THERM A0
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHT31.h"

// Instantiate new sensor objects
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_SHT31 sht = Adafruit_SHT31();

void setup() {
	Serial.begin(9600);
	// Setup Analog Pin
	pinMode(THERM, INPUT);
	
	// Setup VEML7700
	veml.begin();
	veml.setGain(VEML7700_GAIN_1);
	veml.setIntegrationTime(VEML7700_IT_800MS);
	
	// Setup SHT31-D
	sht.begin(0x44);
	
	// More code goes here
}

void loop() {
	String datapoint = collectData();
	//sendData(); // Not written here
	Serial.println(datapoint);
	// More code goes here
}


String collectData() {
	// Get soil temperature with thermistor
	float resistance = analogRead(THERM);
	resistance = (1023 / resistance) - 1;
	resistance = 10000.0 / resistance;
	float B = 3935; // depends on the NTC model
	float soiltemp = log(resistance / 10000) / B;
	soiltemp += 1.0 / (25 + 273.15);
	soiltemp = 1.0 / soiltemp;
	soiltemp -= 273.15;
	soiltemp = soiltemp * (9.0 / 5.0) + 32;
	
	// Get lux from VEML7700
	float lux = veml.readLux();

	// Get temp and humidity from SHT31-D
	float temp = sht.readTemperature() * (9.0 / 5.0) + 32;
	float humidity = sht.readHumidity();
	
	// Return data
	String results = String(temp) + "," + String(humidity) + "," + String(lux) + "," + String(soiltemp);
	return results;
}
