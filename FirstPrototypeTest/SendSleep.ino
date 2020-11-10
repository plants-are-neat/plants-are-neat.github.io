    /*
     * The current protocol consists in each node sends information (if needed) about it's current environment (using collectData). 
     * The outer nodes will wake up, send information, and go to sleep.
     * Middle nodes will wake up, receive information (and send the received information), and send information again about it's own data
     * The mother node in this program will always be listening once waking up.
     */
    #include "Adafruit_VEML7700.h"
    #include "Adafruit_BME280.h"
    #include "Adafruit_SGP30.h"   
    #include <SPI.h>
    #include <RH_RF95.h>
    #include <Adafruit_SleepyDog.h>
     
    #define THERM A0
    #define RFM95_CS 8
    #define RFM95_RST 4
    #define RFM95_INT 3

    // Change to 434.0 or other frequency, must match RX's freq!
    #define RF95_FREQ 868.0

    // Defines the node ID (This will determine overall behaviour)and class
    const int ID = 320;
    const int Group_ID = 300;
    #define MaxSignals 100
     
    // Singleton instance of the radio driver
    RH_RF95 rf95(RFM95_CS, RFM95_INT);
    
    // Instantiate new sensor and radio objects
    Adafruit_VEML7700 veml = Adafruit_VEML7700();
    Adafruit_BME280 bme;
    Adafruit_SGP30 sgp = Adafruit_SGP30();
    
    void setup() 
    {

      // Setup of pins
      pinMode(RFM95_RST, OUTPUT);
      digitalWrite(RFM95_RST, HIGH);
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, HIGH); // Show we're awake
      pinMode(THERM, INPUT);
      Serial.begin(115200);

        // Setup VEML7700
      veml.begin();
      veml.setGain(VEML7700_GAIN_1);
      veml.setIntegrationTime(VEML7700_IT_25MS);
  
      // Setup BME280
      bme.begin();
  
      // Setup SGP30
      sgp.begin();
     
      delay(100);
     
      Serial.println("Feather LoRa TX Test!");
     
      // manual reset
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
     
    int16_t packetnum = 0;  // packet counter, we increment per xmission
    bool SentSucess = false;
    bool ReceiveSucess = false;
    
    void loop()
    {
      String reading;
      int SSN=0, RSN = 0;
      delay(500); // Setup time

      // When the board needs to send information
      while(!SentSucess && ID >= 300){
        delay(100);
        SentSucess = sendInformation();
        //Serial.println("NO More sending!");
        SSN++;
        if(SSN >= MaxSignals)
          break;
      }
      while(!ReceiveSucess && ID < 300){
        ReceiveSucess = receiveInformation();
        //Serial.println("Received!!");
        SentSucess = false;
        RSN++;
        if(RSN >= MaxSignals)
          break;
      }
      Serial.println("Finish Receiving...");
      while(!SentSucess && ID >= 200 && ID < 300){
        SentSucess = sendInformation();
        //Serial.println("NO More sending!");
      }
      Serial.println("Time For Sleep...zzz");
      //Sleep(x); // Used at then end since of a bug
      // delay(5000); // Will be used for testing purposes
    }

  // Function that will send information.
  bool sendInformation()
  {
      String temp;
      delay(500); // allows a little setup time

      // Using the collect function only for sending information
      char radiopacket[40];
      temp = collectData();
      Serial.println(temp);
      delay(100);
      for(int i = 0; i< 40; i++)
      {
       radiopacket[i] = temp[i];
       if(temp.length() == (i+1))
        break;
      }
      //

      /*
      Serial.println("Transmitting..."); // Send a message to rf95_server
      delay(10);
      char radiopacket[20] = "Hello World #      ";
      itoa(packetnum, radiopacket+13, 10);
      Serial.print("Sending "); Serial.println(radiopacket);
      radiopacket[19] = 0;
      
      Serial.println("Sending...");
      delay(10);
      */
      rf95.send((uint8_t *)radiopacket, 40);
     
      Serial.println("Waiting for packet to complete..."); 
      delay(10);
      rf95.waitPacketSent();
      // Now wait for a reply
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
     
      Serial.println("Waiting for reply...");
      if (rf95.waitAvailableTimeout(3000)) // Wait 3 s
      { 
        // Should be a reply message for us now   
        if (rf95.recv(buf, &len))
        {
          packetnum++;
          Serial.print("Got reply: ");
          Serial.println((char*)buf);
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);
          return true;    
        }
        else
        {
          Serial.println("Receive failed");
          return false;
        }
      }
      else
      {
        //Serial.println("No reply, is there a listener around?");
        return false;
      }
  }

  // Receive information
  bool receiveInformation(){
    delay(500);
    Serial.println("Waiting...");
    if (rf95.available())
      {
        // Should be a message for us now
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        //Serial.print("Going to sleep...zzz...."); 
     
        if (rf95.recv(buf, &len))
        {
          delay(200); // To let the receiver to ramp up from sleeping
          digitalWrite(LED_BUILTIN, HIGH);
          RH_RF95::printBuffer("Received: ", buf, len);
          Serial.print("Got: ");
          Serial.println((char*)buf);
           Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);
     
          // Send a reply
          //uint8_t data[] = "number";
          rf95.send(buf, len);
          rf95.waitPacketSent();
          Serial.println("Sent a reply");
          digitalWrite(LED_BUILTIN, LOW);
          return true;
        }
        else
        {
          Serial.println("Receive failed");
          return false;
        }

      }
      else
      {
        return false;
      }
    
  }
  // Function that puts the micro controller into sleep
  void Sleep(int Time)
  { // Determine the amount of loops needed to reach the goal of sleeping for "Time"
    int rem = 0, stime = 0;
    rem = Time % 10;
    stime = (Time-rem)/10;
    Serial.print(stime);
    if(stime >= 1) // In case sleeping is only needed less than 10 s
    {
      for(int i = 0; i < stime; i++)
      {
        //Watchdog.reset();
        Watchdog.sleep(10000);
      }
    }
    
    if(rem > 0) // In case of remainder being 0 (multiple of 10)
    {
      //Watchdog.reset();
      Watchdog.sleep(rem*1000);
    }
    // Reset variables
    SentSucess = false;
    ReceiveSucess = false;
  }


  // Collects information from sensors and returns them in a string
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
