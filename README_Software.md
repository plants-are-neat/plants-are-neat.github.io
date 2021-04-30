# Team 27 Software README
## Introduction
The smart agriculture sensor network is comprised of sensor nodes and one gateway node. The project requires a node.js server to communicate with the ThingSpeak IoT channels. While the project was developed in a Windows operating system environemnt, most of the files can be run in different operating system except for the batch files. Correct installion of node.js is also required to allow the project to work properly. The outer microprocessors (nodes) are flashed with a program that allows them to collect sensor data when not sleeping to conserve energy. The node.js server receives information from the mother microprocessor via serial communication to be sent to a remote website.

## Files
CODE ON ARDUINO IDE:
Adafruit Library installed
 1. Sensor_Node_X.ino:
 
This program serves as the outer node code for nodes with sensors out in the field. It starts off by initializing the radio unit on the onboard feather and setting pins as outputs and inputs for the sensores on board. Each sensor runs through its own initialization. The radio unit has failsafes that does not let the code run if it does not initialize properly. The code then executes. It takes the reading from its sensors, puts the readings in a string, and then sends the string through the radio. The radio library allows for rerouting in the case of a broken node and self healing as well. The message gets routed all the way to the mothernode. After sending the message the board activates its sleep protocol and 'sleeps' turning off most systems to conserve power. It wakes up after a set amount of time and repeats.
    -C++/C
    -Dependencies
      -RadioHead.h             // For the onboard radio
      -SPI.H                   // For on board communication with the sensors 
      -Adafruit_VEML7700.h     // Ambient light sensor
      -Adafruit_SHT31.h        // Humidity sensor
      -Adafruit_SleepyDog.h    // Sleep protocol for the board

 2. client_node.ino

This program serves as the mother node of the network. Starts off by initializing the radio. Periodically polls the other nodes with sensors and polls them to give it information. Upon receiving a message, it prints it out to the COM port for the computer to read. Repolls and repeats.
    -C++/C
    -Dependencies
      -RadioHead.h             // For the onboard radio
      -SPI.H                   // For on board communication with the sensors 
--TO RUN--
1. Open .ino using Arduino IDE
2. Connect Feather M0 to computer using usb-microusb
3. Hit Upload on upper left-hand corner
4. Wait for upload to finish

CODE ON DEKSTOP
NODEJS installed
  1. SerialTest.js

SerialJS does a few things for the mesh. Firstly, it uses an external library to prompt the user for which active port to use. IMPORTANT: The code cannot execute if another program, such as the Arduino IDE is using the COM port, meaning if the Serial Monitor is open and listening in on the COM port it will terminate with an error. Terminating with an error should be avoided. It then reads from the port and saves it within the code to use the second library to send it over to the ThingSpeak IoT channel. The message from the arduino is then chopped up between the partitions, in this case commas, and sent to their specific fields. It then repeats this and sends whenever there is a new message. Messages that are way below and above certain thresholds are deleted and ignored.
     -NODEJS
     -Dependencies
        -ThingSpeakClient    // For updating the ThingSpeak channel
        -serialport          // For reading the serial COM port
        -prompt-sync         // Used for prompting the user
--TO RUN--
1. Install NODEJS https://nodejs.org/en/download/
2. Make sure MotherNode is connected to computer, CANNOT be used by another program, connection will be blocked
3. Open command prompt
4. Go to directory where file is
5. Run command "Node [FILENAME]" where [FILENAME] is the name of the file with the extension

CODE ON WINDOWS
1. PANinstall.bat

_TITLE Dependencies Installation
ECHO Installing dependencies, please wait!                              //Echo just prints out a statement
npm install thingspeakclient                                            //installs the thingspeak client library
npm install serialport                                                  //installs the serial reader
npm install prompt-sync                                                 //installs the prompt to ask the user
ECHO All dependencies downloaded, thank you for your patience!          //Prints out the final message
PAUSE                                                                   //Waits for a user input to move_

This batch file is made for the simplicity of the user. Since the developing team consisted of only window users, the installation and batch file was developed only for a windows environment. As a result, future improvement include the implementation of the batch file to work in different operating systems like iOS, Linux, Unix, etc. This batch file installs all NODEjs libraries. It executes and downloads the libraries and waits for the user to close the command prompt.

2. runMESH.bat

_node SerialTest.js                                                     //THis is how a js script is ran
PAUSE                                                                   //Leaves the prompt open, do not stop the prompt_

This batch file is made for the simplicity of the user. On the team, there are only windows users so we could not develop this for other operating systems. This batch file executes the SerialTest.js. It runs and the promp should be left open for the code to keep executing.

  -Dependencies
    -SerialTest.js
--TO RUN--
1. Open any command prompt
2. Go to where the .bat files are (they should be grouped together, with the NODEJS code)
3. Double click PANinstall.bat first, runMESH may not run if done out-of-order
4. Double click runMESH.bat

## Libraries
The main two libraries used in this project are RH_RF95 and RHMesh. Both libraries are part of a larger library named RadioHead that allows the use of long range (LoRa) radio communication installed in the Arduino. 

RH_RF95 allows the use of two radio wave (868 and 915) lengths. To comply with US regulations, we chose 915 as the default wave length of the network. This can be changed to 868 at the top of each file depending on the user’s country’s regulations. The library is the foundation of the other libraries since the library grants the ability to send and receive packages of information via an object-oriented format. The library consists of drivers and managers to modulate the communication requirements of different microprocessors. While this library allows the communication between nodes, further libraries are needed to achieve the mesh network behavior of route discovery, creation and maintenance of a routing table, and self-healing capabilities. As a result, the next library is needed. Further documentation can be found here: https://github.com/kenbiba/RH-RF95 

RHMesh allows the use of a mesh network behavior into the microprocessors instead of naive communication protocols. It includes automatic route discovery when initializing the drivers and managers as well as multi-hop routing capabilities thus allowing self-healing if a node is corrupted/broken. This is achieved via the use of routing tables first initialized when initializing the manager and sending the first message to the desired address/location. Even if the message fails to send to the desired node/location, the intermediate nodes are registered into the routing table for an up to date information on the most efficient route to each node. A future improvement to the library can be to decrease the situations that create races. The library is an extension of a different library called RHRouter which is the foundation of the RHMesh library. Further documentation can be found here: https://www.airspayce.com/mikem/arduino/RadioHead/classRHMesh.html 

