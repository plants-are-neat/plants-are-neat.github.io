// Include library files and define an analog pin
#define THERM A0;
#include "Adafruit_VEML7700.h"
#include "Adafruit_BME280.h"
#include "Adafruit_SGP30.h"

// Instantiate new sensor objects
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_BME280 bme;
Adafruit_SGP30 sgp = Adafruit_SGP30();

void setup() {
	// Setup Analog Pin
	pinMode(THERM, INPUT);
	
	// Setup VEML7700
	veml.begin();
	veml.setGain(VEML7700_GAIN_1);
	veml.setIntegrationTime(VEML7700_IT_800MS);
	
	// Setup BME280
	bme.begin();
	
	// Setup SGP30
	sgp.begin();
	
	// More code goes here
}

void loop() {
	String datapoint = collectData();
	sendData(); // Not written here
	
	// More code goes here
}


String collectData() {
	// Get soil temperature with thermistor
	float resistance = analogRead(THERM);
	resistance = (1023 / reading) - 1;
	resistance = 10000.0 / resistance;
	float B = 3935; // depends on the NTC model
	float soiltemp = log(resistance / 10000) / B;
	soiltemp += 1.0 / (25 + 273.15);
	soiltemp -= 273.15;
	soiltemp = soiltemp * (9.0 / 5.0) + 32;
	
	// Get lux from VEML7700
	float lux = veml.readLux();

	// Get temp, pressure, humidity from BME280
	float temp = bme.readTemperature() * (9.0 / 5.0) + 32;
	int pressure = bme.readPressure(); // in Pa
	float humidity = bme.readHumidity();
	
	// Get eCO2 from SGP30
	float eco2 = sgp.eCO2;
	
	// Return data
	String results = String(temp) + String(humidity) + Stirng(pressure) + String(lux) + String(eco2) + String(soiltemp);
	return results;
}