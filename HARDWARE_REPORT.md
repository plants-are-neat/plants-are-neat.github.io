# Team 27 Hardware README
## Introduction
The smart agriculture sensor network is comprised of sensor nodes and one gateway node. The gateway node is simply a LoRa-equipped microcontroller, namely the Adafruit FeatherM0 LoRa microcontroller board. The sensor nodes also include a LoRa microcontroller, but it is mounted on a PCB that includes all of the power electronics and all of the sensors for the sensor node. All schematics and layouts can be found in the directory "Schematics and Layouts".

## System Design
The gateway node electronics consist solely of the Adafruit FeatherM0 LoRa microcontroller board, which is connected to a laptop or desktop computer via a micro-USB cable. The sensor nodes, however, are more complex. 

The sensor nodes are capable of measuring ambient temperature, humidity, ambient light, soil moisture, and soil temperature. There are breakouts on the PCB so the user can add more sensors that may be more specific to the farming task. The sensor nodes also features solar energy harvesting with a solar panel and a solar battery charging and power path management system. Thus, the requirements met by the hardware of the project are: 
1. The sensor nodes must be powered for one year continuously.
2. For 100 sensor readings in identical conditions, the sensor data should not deviate by more than +- 5% of the mean.

Requirement number 2 is met by the onboard sensors, which use the I2C protocol if they are digital sensors and act as voltage dividers if they are analog sensors. The LoRa Featherboard is natively I2C compatible, so it can poll the digital sensors without any additional components which are not recommended by the sensor manufacturer. The Featherboard also has an on-board analog-to-digital converter (ADC), so there is no need for a separate external chip to allow the processor to communicate with the digital sensors. Requirement number 1 is met by the power path management and solar battery charging system. 

### Sensor Node Equipment BOM
| Item Number  | Item       | Vendor      | Quantity    |
| ----------- | ----------- | ----------- | ----------- |
| 1 |

## PCB Design

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

