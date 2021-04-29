# Team 27 README
## Introduction
Hello, welcome to plants are neat! Throughout this project, we developed a mesh sensor network system capable of self-healing and communicating data from node to node. The data is routed through the network of nodes to find an access point to push data to the online database. The online database is a relational database organized through the thingspeak API to track independent channel ids, field ids, field values, and time data. We can pull the most recent data point to create google gauges with the most recent data point with this data. Notifications are handled through server-side react scripts and test the database to ensure data coming in is within given thresholds and that each node that should be in the network is successfully communicating data. Broken nodes are identified by a lack of data received by a gateway node over a 30 min period. This enables users to fix problems in the field without getting constant notifications. Broken nodes check for fixes every 30 minutes.

## Getting Started
Populate the PCB using the schematic and BOM.
Solder wires to the battery breakout and have them ready to connect to the battery, but do not connect the battery yet.
Install the vent plugs into the enclosure.
Drill holes into the enclosure for the external antenna, soil temperature sensor, soil moisture sensor, and the solar panel DC jack.
Thread the soil moisture sensor and the soil temperature sensor through the holes and solder them into place using the schematic.
Screw the PCB into the enclosure.
Using weatherproof epoxy, fix the antenna, soil sensors, and solar panel DC jack into place and let cure overnight.
Upload any relevant code to the Featherboard using the Software_README.md file.
Insert the Featherboard into the PCB.
Connect the solar panel DC jack to the PCB DC jack.
Connect the battery to the battery breakout on the PCB.
Close the enclosure and place it in the proper location.
Open .ino using Arduino IDE
Connect Feather M0 to computer using usb-microusb
Hit ‘Upload’ on upper left-hand corner
Wait for upload to finish
Install NODEJS https://nodejs.org/en/download/
Make sure MotherNode is connected to computer, CANNOT be used by another program, connection will be blocked
Open command prompt
Go to directory where file is
Run command "Node SerialTest.js”
## Current State
The current state of the project features a mother node requesting data from all the outer nodes, and the outer nodes collect data (soil moisture, soil temperature, ambient temperature, humidity, and sunlight intensity) and sending the data to the network. As stated in the introduction, the project also includes a thingspeak API, graphical representation of the collected data, and broken node email notifications.
## Failure Modes and Gotchas
| Gotcha  | Reason | Solution |
| ----------- | ----------- | ----------- |
| Boost converter auto bypass nonfunctional | The MOSFETs’ gate voltages are not defined at startup | Use pullup resistors on the MOSFET gates to Vcc |
| COM port disconnects | COM ports can disconnect after successfully uploading code to the Featherboards | Use the Arduino IDE to pick the new COM port corresponding to the correct Featherboard |
| Delayed email notifications | gmail firewalls | alternative notification system |
| Self healing requires correct installation | If each node is not installed within range of at least two other nodes, the network may not be able to self heal | Make sure that each sensor node is installed within range of at least two other sensor nodes |
| NODEjs crashes upon selecting a port | Serial ports can’t be read using different programs simultaneously | Close all other applications reading or writing to the port, most likely culprit is the Arduino IDE | 
## Possible Future Improvements
- Expanding the node network beyond our test 4, integrating automatic node IDs and being able to toggle between all of these data displays
- Change visualisation page to display multiple nodes at once. 
- Expanded sensor readings
- Use wifi-enabled board so it can run continuously
- Fix boost converter auto-bypass issue
- Use a Raspberry Pi as a continuously running server that can read serial data via USB input
