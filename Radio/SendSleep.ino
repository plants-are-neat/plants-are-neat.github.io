    // Feather9x_TX
    // -*- mode: C++ -*-
    // Example sketch showing how to create a simple messaging client (transmitter)
    // with the RH_RF95 class. RH_RF95 class does not provide for addressing or
    // reliability, so you should only use RH_RF95 if you do not need the higher
    // level messaging abilities.
    // It is designed to work with the other example Feather9x_RX
     
    #include <SPI.h>
    #include <RH_RF95.h>
    #include <Adafruit_SleepyDog.h>
     
    /* for feather32u4 
    #define RFM95_CS 8
    #define RFM95_RST 4
    #define RFM95_INT 7
    */
     
    #define RFM95_CS 8
    #define RFM95_RST 4
    #define RFM95_INT 3

    // Change to 434.0 or other frequency, must match RX's freq!
    #define RF95_FREQ 868.0
     
    // Singleton instance of the radio driver
    RH_RF95 rf95(RFM95_CS, RFM95_INT);
    
    
    void setup() 
    {
      
      pinMode(RFM95_RST, OUTPUT);
      digitalWrite(RFM95_RST, HIGH);
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, HIGH); // Show we're awake
      Serial.begin(115200);

     
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
      
      // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
     
      // The default transmitter power is 13dBm, using PA_BOOST.
      // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
      // you can set transmitter powers from 5 to 23 dBm:
      rf95.setTxPower(23, false);
    }
     
    int16_t packetnum = 0;  // packet counter, we increment per xmission
    
    bool SentSucess = false;
    bool ReceiveSucess = false;
    
    void loop()
    {
      delay(500); // Setup time
      //sleepTimer();
      //Serial.println("Checking Loop");
      while(!SentSucess){
        SentSucess = sendInformation();
        //Serial.println("NO More sending!");
      }
      Serial.println("Finish Sending...");
      Serial.println("Now Receiving...");
      while(!ReceiveSucess){
        ReceiveSucess = receiveInformation();
        //Serial.println("Received!!");
      }
      Serial.println("Finish Receiving...");
      Serial.println("Time For Sleep...zzz");
      //sleepTimer();
     
    }


  bool sendInformation()
  {
      delay(1000); // allows a little setup time
      Serial.println("Transmitting..."); // Send a message to rf95_server
      delay(10);
      char radiopacket[20] = "Hello World #      ";
      itoa(packetnum, radiopacket+13, 10);
      Serial.print("Sending "); Serial.println(radiopacket);
      radiopacket[19] = 0;
      
      Serial.println("Sending...");
      delay(10);
      rf95.send((uint8_t *)radiopacket, 20);
     
      Serial.println("Waiting for packet to complete..."); 
      delay(10);
      rf95.waitPacketSent();
      // Now wait for a reply
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
     
      Serial.println("Waiting for reply...");
      if (rf95.waitAvailableTimeout(3000))
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
        Serial.println("No reply, is there a listener around?");
        return false;
      }
  }

  bool sendInformation(){
    if (rf95.available())
      {
        // Should be a message for us now
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        //Serial.print("Going to sleep...zzz...."); 
     
        if (rf95.recv(buf, &len))
        {
          delay(200); // To let the receiver to ramp up from sleeping
          digitalWrite(LED, HIGH);
          RH_RF95::printBuffer("Received: ", buf, len);
          Serial.print("Got: ");
          Serial.println((char*)buf);
           Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);
     
          // Send a reply
          uint8_t data[] = "And hello back to you";
          rf95.send(data, sizeof(data));
          rf95.waitPacketSent();
          Serial.println("Sent a reply");
          digitalWrite(LED, LOW);
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


    void sleepTimer(void)
{
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println("Sleep..zzz...");
  int sleeptime = Watchdog.sleep(); // about 16s max
  USBDevice.attach();
  Serial.begin(115200);
  while(!Serial); // wait for Arduino Serial Monitor (native USB boards)
  Serial.println("Awake for now..."); // A flag to tell if its awake
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN,LOW);
  delay(100);
  digitalWrite(LED_BUILTIN,HIGH);
}
