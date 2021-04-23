# Team 27 Hardware README
## Introduction
The smart agriculture sensor network is comprised of sensor nodes and one gateway node. The gateway node is simply a LoRa-equipped microcontroller, namely the Adafruit FeatherM0 LoRa microcontroller board. The sensor nodes also include a LoRa microcontroller, but it is mounted on a PCB that includes all of the power electronics and all of the sensors for the sensor node. All schematics and layouts can be found in the directory "Schematics and Layouts".

## System Design
The gateway node electronics consist solely of the Adafruit FeatherM0 LoRa microcontroller board, which is connected to a laptop or desktop computer via a micro-USB cable. The sensor nodes, however, are more complex. 

The sensor nodes are capable of measuring ambient temperature, humidity, ambient light, soil moisture, and soil temperature. There are breakouts on the PCB so the user can add more sensors that may be more specific to the farming task. The sensor nodes also features solar energy harvesting with a solar panel and a solar battery charging and power path management system. Thus, the requirements met by the hardware of the project are: 
1. The sensor nodes must be powered for one year continuously.
2. For 100 sensor readings in identical conditions, the sensor data should not deviate by more than +- 5% of the mean.

Requirement number 2 is met by the onboard sensors, which use the I2C protocol if they are digital sensors and act as voltage dividers if they are analog sensors. The LoRa Featherboard is natively I2C compatible, so it can poll the digital sensors without any additional components which are not recommended by the sensor manufacturer. The Featherboard also has an on-board analog-to-digital converter (ADC), so there is no need for a separate external chip to allow the processor to communicate with the digital sensors. Requirement number 1 is met by the power path management and solar battery charging system. These two subsystems are discussed in detail in the PCB Design section.

The sensor node consists of a populated PCB, an Adafruit FeatherM0 LoRa microcontroller board, an external antenna, a battery pack, an enclosure and other mechanical components, and a solar panel. A bill of materials for one sensor node is presented below. Recall that the gateway node consists solely of one Adafruit Featherboard, and is not listed here. Also included in this BOM is the soil moisture sensor, which is not available at Digikey. All other components for the PCB were purchased at Digikey and are available in the PCB Design section.

### Sensor Node Equipment BOM
| Item Number  | Item       | Vendor      | Link        | Quantity    |
| ----------- | ----------- | ----------- | ----------- | ----------- |
| 1 | FeatherM0 LoRa microcontroller | Adafruit | [Link](https://www.adafruit.com/product/3178) | 1 |
| 2 | Large 6.5V 3W solar panel | Adafruit | [Link](https://www.adafruit.com/product/500) | 1 |
| 3 | ICR18650-30Q Li-Ion Battery Cells | 18650 Battery Store | [Link](https://www.18650batterystore.com/products/samsung-30q) | 4 |
| 4 | Custom PCB | JLCPCB | N/A | 1 |
| 5 | VH400-2M soil moisture sensor | Vegetronix | [Link](https://www.vegetronix.com/Products/VH400/#order) | 1 |
| 6 | Nema 4X enclosure | Solutions Direct | [Link](https://www.solutionsdirectonline.com/hammond-4x7x7-polycarbonate-electrical-enclosure-with-clear-cover-1554wa2gycl) | 1 |
| 7 | Pressure equalization vents | Solutions Direct | [Link](https://www.solutionsdirectonline.com/enclosure-vent-plug-m12-x-15-2-pack--284060-00-p6062.aspx) | 1 |
| 8 | LoRa 900MHz antenna | Adafruit | [Link](https://www.adafruit.com/product/3340) | 1 |

The Featherboard is inserted into the assembled PCB as directed by the silkscreen. The battery is connected to the P1 breakout on the PCB with the battery positive going to the square hole. The solar panel male DC jack is inserted into the female DC jack on the PCB. When populated and programmed, the hardware should be powered, be able to run, collect data, and send and receive packets via LoRa.

The system requires at least 4.5V at the DC jack input to function without battery power, so for testing purposes a 5V wall adapter will work. The maximum input voltage at the DC jack is 10V. Since the solar panel can only output an open circuit voltage maximum of 6.5V, this shouldn't be an issue, but the PCB has a zener diode to clamp the input voltage in case of emergencies. The PCB also has a boost converter for low light conditions, which is discussed in the PCB design section.

The Featherboard uses an external 900MHz antenna for long distance communication with other sensor nodes. It is installed on the outside of the enclosure so that line-of-sight is not obstructed.

Pictures of the assembled system are shown below:

### System Images
![Hardware angle 2](/assets/hardware2.jpg)
![Hardware angle 4](/assets/hardware4.jpg)


## PCB Design
The PCB consists of three main sections:
1. Input power electronics and solar charging and power path management.
2. Featherboard harnessing and breakouts.
3. Digital and analog sensors.

An image of the rendered PCB layout with the above sections marked is shown below:
![PCB Sections](/assets/pcbsections.jpg)

### PCB Section 1: Power Electronics
The largest and most important subsection of section 1 of the PCB is the solar charging and the power path management, which is accomplished using the BQ24074 (item #2 in the PCB BOM). The BQ24074 is a standalone 1.5A linear battery charger with power path management. The design of this IC on the PCB was implemented using the application notes section of the datasheet provided by Texas Instruments and accessed from Digikey. Using the application notes, the Enable pins E1 and E2 were tied high and low respectively, causing the IC to be USB compatible with a maximum input current of 500mA. A temperature sensor for the battery pack was not implemented, so in it's place a 10k resistor was selected as R7 to permanently cause the IC to be active. R5 and R6 were selected to both be 2.2k, which makes the input current limit and the fast charging current to be the same value, and is set to 400mA. The input and output capacitors as well as the bypass capacitors were selected based on the application notes' recommended values. The PCB layout of this subsection was informed by the layout recommendations in the application notes, with special considerations for hand-soldering of SMD parts and the placement of the PCB and battery within the enclosure.

The other main subsection of section 1 is the low-light boost converter, which boosts the solar voltage to a level that will operate the Featherboard and sensors in low-light conditions so that the battery does not have to discharge as much. This is accomplished using the MCP16414 (item #1 in the PCB BOM). The MCP16414 is a low quiescent current low input voltage minimum programmable boost converter. The design of this IC on the PCB was implemented using the application notes section of the datasheet provided by Microchip and accessed from Digikey. R1 and R3 were selected to be 1.5Meg and 360k respectively to give an output voltage of 5V, as specified in the application notes. The switching inductor as well as the input and output capacitor values were also taken from the application notes' recommended values for 5V output operation. The PCB layout of this subsection was informed by the layout recommendations in the application notes, with special considerations for hand-soldering of SMD parts. 

The MCP16414 maximum input voltage is 5.25V, and since the solar panel output can reach 6.5V, an automatic bypass system was implemented using P-channel MOSFETs (Q1 and Q2 in the PCB BOM), which are controlled by the Featherboard. This subsection is currently experiencing a bug where the MOSFET gates are floating on startup, and thus the boost converter is not properly bypassed, which causes the input voltage to short to ground through the boost converter. A better solution for future beta versions of the project would be to implement the automatic bypass with analog electronics, so that the propagation delay of the control signals to the P-channel MOSFETs is much less. Additionally, including pull-up resistors and diodes (to the solar panel input and the battery positive) on the MOSFET gates will establish a gate voltage immediately. This issue was known prior to the end of the semester, but was not implemented in the alpha version of the project due to issues with working remotely and time constraints. The entire system needed to be passed around for other tasks, and in order to have a completed project by the end of the semester this issue was not addressed, although there are proposed solutions presented here.

### PCB Section 2: Harnessing and Breakouts
This section is where the Featherboard is inserted into the PCB. The section also includes 3.3V and ground breakouts to power other sensors, as well as I2C clock and data line breakouts for exteral digital I2C compatible sensors.

### PCB Section 3: Sensors
This section consists of the digital and analog sensors that are polled by the Featherboard. The sensors used are:
* SHT31-D (item #4) to measure ambient temperature and humidity.
* VEML7700 (item #3) to measure ambient light.
* 10k NTC thermistor with K constant 3435K (item #19) to measure soil temperature.
* VH400-2M (item #5 in the system design BOM) to measure soil moisture.

The digital sensors were implemented on the PCB and their layout was done using the application notes in their datasheets provided by Sensirion (SHT31-D) and Vishay (VEML7700) and accessed from Digikey. The VH400 was connected according to the tutorials provided by Vegetronix, and the thermistor was connected in a voltage divider configuration according to a tutorial made by Adafruit. Both the digital and analog sensors are powered from the Featherboard's on-board 3.3V regulator. The analog sensors are grounded via a ground plane that spans the entire PCB, including the power electronics. The digital sensors were grounded separately from the analog sensors, and were grounded directly to the Featherboard ground in order to eliminate any capacitive coupling from the power electronics via the large ground plane.

### PCB BOM
| Item Number | Designator  | Value       | Footprint   | Quantity    | Digikey Part # |
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| 1 | U1 | MCP16414-I/UN | MSOP-10 | 1 | 150-MCP16414-I/UN-ND |
| 2 | U2 | BQ24074RGTR | 16-QFN | 1 | 296-38873-1-ND |
| 3 | U4 | VEML7700-TR | 4-SMD, J-LEAD | 1 | VEML7700CT-ND |
| 4 | U5 | SHT31-D | 8-VFDFN | 1 | 1649-1011-2-ND |
| 5 | Q1,Q2 | STR2P3LLH6 | SOT-23 | 2 | 497-15520-1-ND |
| 6 | J1 | PJ-102B | DC Jack | 1 | CP-102B-ND |
| 7 | D1 | RB161MM-20TR | SOD-123F | 1 | RB161MM-20CT-ND |
| 8 | D2 | MMSZ5233BT1G | SOD-523 | 1 | MM5Z5V1ST1GOSCT-ND |
| 9 | L1 | 4.7uH | 1812 (4532) | 1 | 490-2517-1-ND |
| 10 | R2,R4,R10,R11 | 4.7kR | 0603 (1608) | 4 | CRCW06034K70JNEBCCT-ND |
| 11 | R5,R6 | 2.2kR |  0603 (1608) | 2 | 541-4058-1-ND |
| 12 | R7,R8,R9,R12,R13 | 10kR | 0603 (1608) | 5 | 13-RE0603FRE0710KLCT-ND |
| 13 | R1 | 1.5MR | 0603 (1608) | 1 | 311-1.5MGRCT-ND |
| 14 | R3 | 360kR | 0603 (1608) | 1 | 311-360KGRCT-ND |
| 15 | C1,C4 | 10uF | 0603 (1608) | 2 | 478-10766-1-ND |
| 16 | C2 | 1uF | 0603 (1608) | 1 | 587-1248-1-ND |
| 17 | C3,C5 | 4.7uF | 0603 (1608) | 2 | 1276-1902-1-ND |
| 18 | C6,C7 | 100nF | 0603 (1608) | 2 | 1276-1931-1-ND |
| 19 | P2 | 10k NTC 3435K | 2-LEAD | 1 | BC2647-ND |

## Installation and Instructions for Engineers
Instructions on how to assemble and install a sensor node are listed below:
1. Populate the PCB using the schematic and BOM.
2. Solder wires to the battery breakout and have them ready to connect to the battery, but do not connect the battery yet.
3. Install the vent plugs into the enclosure.
4. Drill holes into the enclosure for the external antenna, soil temperature sensor, soil moisture sensor, and the solar panel DC jack.
5. Thread the soil moisture sensor and the soil temperature sensor through the holes and solder them into place using the schematic. 
6. Screw the PCB into the enclosure.
7. Using weatherproof epoxy, fix the antenna, soil sensors, and solar panel DC jack into place and let cure overnight.
8. Upload any relevant code to the Featherboard using the Software_README.md file.
9. Insert the Featherboard into the PCB. 
10. Connect the solar panel DC jack to the PCB DC jack. 
11. Connect the battery to the battery breakout on the PCB. 
12. Close the enclosure and place in the proper location.

Note: for optimal self-healing performance, place each sensor node within range of at least two other sensor nodes.

To install the gateway node, plug it into a laptop or desktop computer using a micro-USB cable, connect the external antenna, and follow the instructions in the Software_README.md file.