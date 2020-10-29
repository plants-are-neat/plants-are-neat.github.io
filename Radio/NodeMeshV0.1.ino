    // Feather9x_RX
    // -*- mode: C++ -*-
    // Example sketch showing how to create a simple messaging client (receiver)
    // with the RH_RF95 class. RH_RF95 class does not provide for addressing or
    // reliability, so you should only use RH_RF95 if you do not need the higher
    // level messaging abilities.
    // It is designed to work with the other example Feather9x_TX
     
    #include <SPI.h>
    #include <RH_RF95.h>
     
    /* for Feather32u4 RFM9x
    #define RFM95_CS 8
    #define RFM95_RST 4
    #define RFM95_INT 7
    */
     

    #define RFM95_CS 8
    #define RFM95_RST 4
    #define RFM95_INT 3
    #define LED 13
     
    // Change to 434.0 or other frequency, must match RX's freq!
    #define RF95_FREQ 868.0
     
    // Singleton instance of the radio driver
    RH_RF95 rf95(RFM95_CS, RFM95_INT);

    struct Board
    {
      bool isLeader = false;  //To know which is the mothernode
      int BoardNum = -1;      //Board number, default is -1 for when it enters network
      int LookUp[4][2];       //Keeps 4 other nodes in memory with their path values
    };

    struct Packet
    {
      int SourceNode = -1;         //Node that sent the packet
      int PacketNum = -1;          //Packet number, used for book keeping
      float Readings[3] = {-1,-1,-1};      //The readings from the 3 sensors
      int Ack = -1;                //Acknowledgment, last packet received
      int ErrorFlag = -1;          //Used in cases of errors
    };

    struct Net
    {
      int Nodes;
      int BrokenNodes[1];
    };
    enum ErrorCodes
    {
      RadioInit,  //Radio did not initialize, restard board
      Frequency   //Frequency did not initialize or is mismatched, check code and restart board
    };

    Board Me;
    void setup()
    {
      pinMode(LED, OUTPUT);
      digitalWrite(LED, LOW);
      Serial.begin(115200);
      pinMode(RFM95_RST, OUTPUT);
      digitalWrite(RFM95_RST, HIGH);
     
      // manual reset
      digitalWrite(RFM95_RST, LOW);
      delay(10);
      digitalWrite(RFM95_RST, HIGH);
      delay(10);
     
      while (!rf95.init()) {
          ErrorDetected(RadioInit);
      }
     
      // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
      if (!rf95.setFrequency(RF95_FREQ)) {
          
      }
      
     
      // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
     
      // The default transmitter power is 13dBm, using PA_BOOST.
      // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
      // you can set transmitter powers from 5 to 23 dBm:
      rf95.setTxPower(23, false);
      JoinNet();
    }
     
    void loop()
    {
      
    }


    //Going to be used to display the error using the built in LED on the board
    void ErrorDetected(int i)
    {
      for(int j =0; j<=i;j++)
      {
        digitalWrite(LED,HIGH);
        delay(250);
        digitalWrite(LED,LOW);
        delay(250);
      }
    }

    void JoinNet()  //Func to join net
    {
      struct Packet Hello;
      struct Board InitPacket;

      while(!rf95.recv((uint8_t*) InitPacket, sizeof(struct Board))) //Continuosly runs until it receives an appropriate message
      {
        rf95.send(Hello, sizeof(struct Packet)); //Keeps transmitting the message until response
      }
        Me= InitPacket; //Once it breaks, copy the board info
        Hello.Ack = 0; //Confirming packet received
        rf95.send((uint8_t)Hello, sizeof(struct Board)); // Sends ack
        // 3 cases
        // 1: Both nodes receive, nothing special
        // 2: New node does not send ack, as long as it got to the step it got a node number so it's ok to not send ack
        // 3: Old node does not send data, new node repeats until further notice
    }
    
