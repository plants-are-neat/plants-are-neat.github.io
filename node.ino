// Include header files and define analog pin
#define THERM A2
#define VH400 A5
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHT31.h"

// Instantiate new sensor objects
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_SHT31 sht = Adafruit_SHT31();

void setup() {
	Serial.begin(9600);
	// Setup Analog Pins
	pinMode(THERM, INPUT);
	pinMode(VH400, INPUT);
	
	// Setup VEML7700
	veml.begin();
	veml.setGain(VEML7700_GAIN_1_8); // largest light range
	veml.setIntegrationTime(VEML7700_IT_100MS);
	
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
	moisture = round(soiltemp * 10) / 10; //one decimal place
	
	// Get lux from VEML7700
	float lux = veml.readLux(); // units of Lux
	lux = round(lux * 10) / 10; // one decimal place

	// Get temp and humidity from SHT31-D
	float temp = sht.readTemperature() * (9.0 / 5.0) + 32; // units of degrees F
	temp = round(temp * 10) / 10; // one decimal place
	float humidity = sht.readHumidity(); // units of %
	humidity = round(humidito * 10) / 10; // one decimal place
	
	// Return data
	String results = String(temp) + "," + String(humidity) + "," + String(lux) + "," + String(moisture) + "," + String(soiltemp);
	return results;
}
