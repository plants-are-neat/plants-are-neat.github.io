    /*
     *  Simple HTTP get webclient test
     */
     
    #include <ESP8266WiFi.h>
     
    char* ssid;
    char* password;
     
    const char* host = "192.168.137.241";
     
    void setup() {
      Serial.begin(9600);
      delay(100);
      while(!Serial);
      // We start by connecting to a WiFi network
     Serial.println("Phase 1");
     WifiSetup();
     Serial.println("Phase 2");
    }
     
    int value = 0;
     
    void loop() {
      delay(5000);
      ++value;
     
      Serial.print("connecting to ");
      Serial.println(host);
      
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 8080;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      
      // We now create a URI for the request
      String url = "/testwifi/index.html";
      Serial.print("Requesting URL: ");
      Serial.println(url);
      
      // This will send the request to the server
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n\r\n");
      delay(500);
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      
      Serial.println();
      Serial.println("closing connection");
    }


 bool WifiSetup()
 {

      String ssidtemp, passtemp;
      Serial.setTimeout(10000);
      Serial.println("Enter your ssid (name of the wifi network): ");
      ssidtemp = Serial.readStringUntil(13);
      Serial.print(ssidtemp);
      
      Serial.println("Enter the password for your wifi network : ");
      passtemp = Serial.readString();
      Serial.print(passtemp);
      
      WiFi.begin(ssid, password);
      int error = 0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if(error == 10)
        {
          
        }
        error++;
      }
     
      Serial.println("");
      Serial.println("WiFi connected");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
 }
