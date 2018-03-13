#PARTS LIST.

= Introduction =

The gear position indicator is a custom hardware project that interfaces with a 2003-2009 suzuki SV 650 and 
possibly V Strom 650s (not yet tested) to display the gear position, current ambient temperature and charging system voltage.

The display is designed to look like the stock gauge cluster, it is a very slim light weight unit with an LCD display. The yellow 
backlight color was specificaly chosen to match the OEM backlighting. 

Power draw of the unit is less than 1/4 watt so there is almost no load to the bikes charging system.

A nokia 5110 LCD screen was chosen as this display offers a low price point combined with an excellent full sun readability.

Wiring interface is a simple three wire setup - ignition power, ground, and gear position level.

= Components =

This is the list of components required to build the gear position indicator.

These parts should be easily obtainable from your favorite parts supplier:
  * ATTiny44a - microcontroller
  * LM2937 / LM2936 - 3.3 volt regulator
  * nokia 5110 lcd display
  * mcp9700 temperature sensor
  * Capacitor 10uF ceramic
  * 2 Capacitor 0.1uF ceramic
  * 3 x Resistor 10K ohm
  * Resistor 47k ohm
  
