# Suzuki Gear Position Indicator

The gear position indicator is a custom hardware project that interfaces with a 2003-2009 suzuki SV 650 and 
possibly 2004-2011 V Strom 650s (not yet tested) to display the gear position, current ambient temperature and charging system voltage.

The display is designed to look like the stock gauge cluster, it is a very slim light weight unit with an LCD display. The yellow backlight color was specificaly chosen to match the OEM backlighting. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

To build the firmware solution you will need Atmel Studio v. 7

## Deployment

To deploy the firmware you will need an in system programmer (ISP) attached to the microcontrollers using the following programming pins.

![alt text](https://raw.githubusercontent.com/RaysceneNS/SV-650-GearPositionIndicator/master/img/attinyx4.png)

* MOSI
* MISO
* SCK
* RESET
* VCC
* GROUND

## Hardware

The schematic folder contains a Kicad schematic and board layout to build the hardware. Parts are surface mount and designed to be mounted to a single side circuit board for ease of build.

Power draw of the unit is less than 1/4 watt so there is almost no load to the bikes charging system.

A nokia 5110 LCD screen was chosen as this display offers a low price point combined with an excellent full sun readability.

### Installation

Wiring interface is a simple three wire setup - ignition power, ground, and gear position level. The hardware is bolted to the head stock just in from of the stock gauge cluster. The mounting box is a black abs plastic project box.


### Components

This is the list of components required to build the gear position indicator.

  * ATTiny44a - microcontroller
  * LM2937 / LM2936 - 3.3 volt regulator
  * nokia 5110 lcd display
  * mcp9700 temperature sensor
  * Capacitor 10uF ceramic
  * 2 Capacitor 0.1uF ceramic
  * 3 x Resistor 10K ohm
  * Resistor 47k ohm
  * BOX ABS BLACK 1.97"L X 1.97"W
  
### Finished Product

This is what the finished product looks like on the motorcycle.

![alt text](https://github.com/RaysceneNS/SV-650-GearPositionIndicator/blob/master/img/screen%20first%20gear.jpg)

![alt text](https://github.com/RaysceneNS/SV-650-GearPositionIndicator/blob/master/img/screen%20neutral.jpg)

![alt text](https://github.com/RaysceneNS/SV-650-GearPositionIndicator/blob/master/img/screen%20second%20gear.jpg)
