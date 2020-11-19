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
    const int ID = 50;
    const int Group_ID = 300;
    
    #define MaxSignals 10 // Decides how many tries before giving up and moving on (to conserve energy)
     
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
      while(!Serial); // Open the monitor first
    }
     
    int16_t packetnum = 0;  // packet counter, we increment per xmission
    bool SentSucess = false;
    bool ReceiveSucess = false;
    
    void loop()
    {
      String reading, info;
      int IDR;
      int SSN=0, RSN = 0, Poll = 0, StayOn = 1;
      bool temp;
      
      delay(500); // Setup time
      Serial.println("Start of Loop");
      // In case the current node is the mother node, follow this behaviour
      while(!SentSucess && ID < 200){
        if(Poll == 0) {
          Serial.print("What is the ID: ");
          
          IDR = 420; // Read what sensor you would like
        }
        SentSucess = sendPoll(IDR);
        Serial.println("Fail, .5 s to next try");
        delay(500); // Wait before sending the next radio signal
        Poll++;
        if(Poll >= MaxSignals)
        {
          Serial.println("Poll Cancelled");
          break;
        }
        if(SentSucess)
        {
          info = receiveInfo();
         Serial.println(info);
          
        }
      }

      // The inner nodes waiting for a signal
      while(StayOn && ID >= 200){
        receivePoll();
      }
      delay(10000); // 10 sec CD

      
    }

  // Function that will send information.
  bool sendInfo(String info)
  {
      String temp;
      int mSize = RH_RF95_MAX_MESSAGE_LEN;
      delay(500); // allows a little setup time

      // Using the collect function only for sending information
      char radiopacket[RH_RF95_MAX_MESSAGE_LEN];
      for(int i = 0; i< RH_RF95_MAX_MESSAGE_LEN; i++)
      {
       radiopacket[i] = info[i];
       if(info.length() == (i+1)){
        mSize = i+1;
        break;
       }
      }

      for(int i = 0; i < 10; i++) { // Send the signal 10 times
        rf95.send((uint8_t *)radiopacket, mSize);
        rf95.waitPacketSent();
      }

      return true;
  }

  // Receive information
  void receivePoll(){
    char IDC[40], IDP[40], sendS[RH_RF95_MAX_MESSAGE_LEN];
    String IDCS, IDPS;
    int sizeM;
    itoa(ID, IDC, 40);
    delay(500);
    Serial.println("Waiting For signal...");
    if (rf95.available())
      {
        // Should be a message for us now
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);     
        if (rf95.recv(buf, &len))
        {
        
          delay(200); // To let the receiver to ramp up from sleeping
          // Send a reply
          for(int i = 0; i < 40; i++)
            IDP[i] = buf[i];
          IDCS = IDC;
          IDPS = IDP;
          if(ID == IDPS.toInt()) {
            Serial.println("That's me!!");
            delay(100); // Wait before sending the data
            IDCS = collectData();
            sendS[0] = '*';
            for(int i = 0; i < RH_RF95_MAX_MESSAGE_LEN; i++)
            {
              if(i == IDCS.length())
              {
                sizeM = i+1;
                break;
              }
              sendS[i+1] = IDCS[i];
            }
            for(int i = 0; i < 10; i++)
            {
              rf95.send((uint8_t *) sendS, sizeM);
              rf95.waitPacketSent();
            }
          }
          else
          {
            Serial.println("Not me, will wait for data now...");
            while(sendPoll(IDPS.toInt()));
            IDCS = receiveInfo();
            sendInfo(IDCS);
            Serial.println("Found, Sent the Info for my job, Sleep now...");
            delay(30000); // Sleep for 30 seconds
          }
        }
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

bool sendPoll(int IDR)
  {
      delay(500); // allows a little setup time

      // Using the collect function only for sending information
      char radiopacket[40];
      itoa(IDR,radiopacket,10);
      rf95.send((uint8_t *)radiopacket, 40);
      rf95.waitPacketSent();
      // Now wait for a reply
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
     
      Serial.println("Waiting for reply...");
      if (rf95.waitAvailableTimeout(3000)) // Wait 3 s before timing out
      { 
        // Should be a reply message for us now   
        if (rf95.recv(buf, &len))
        {
          return true;
        }
        else
        { // Receiving failed
          return false;
        }
      }
      else
      { // This is failing to get a reply
        //Serial.println("No reply, is there a listener around?");
        return false;
      }
  }

  String receiveInfo()
  {
    String results = "None";
    bool STOP = false;
    while(!STOP){
      if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);     
        if (rf95.recv(buf, &len))
        {
          if(buf[0] == '*'){
            results = (char *) buf;
            STOP = true;
          } 
        }
      }
    }

    return results;
  }
