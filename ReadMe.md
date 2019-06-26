
# Electrical Impedance Tomography (EIT) Data Collection using Arduino


## Table of Contents:

* [Introduction](#introduction)
* [Electrical connection](#electrical-connection)
* [How data is collected](#how-data-is-collected)
* [Customizing data collection](#customizing-data-collection)
* [Passing Settings to Arduino](#passing-settings-to-arduino)
* [Function of LED pins](#function-of-led-pins)
* [How data is printed](#how-data-is-printed)
* [Run, Pause and Stop Data collection](#run,-pause-and-stop-data-collection)
* [Important References and Links](#important-references-and-links)
*	[How to run code?](#how-to-run-code?)
 
 
  
## Introduction
 
This code primarily collects the data that is required to generate an Electrical Impedance Tomography (EIT) Image. It also collects the
resistance between any two electrodes.
 
  
## Electrical connection
  
The hardware setup consists of 16 electrodes (arranged evenly in a circular fashion) connected to a sensor / conductive material. 
The electrodes are connected to the 16 analog pins of the arduino mega and also the 16 channels of the three DG407 Muxes. 

The 'D' pin of mux0 is connected to a  constant current source.  
The 'D' pin of mux1 is connected to ground.  
The 'D' pin of mux2 is connected to VCC and a constant resistor. (Used to measure resistance, using a voltage divider circuit)


## How data is collected
 
One can collect two types of data from this code:

1. Voltage Data for EIT Image
     > Current is injected between any two adjacent electrodes and the voltages from the remaining electrodes are measured. Then the current is injected between the next two electrodes. This cycle starts from (electrodes) [0,1] and ends at [15,0]. For this type of data collection the muxes 0 and 1 are enabled. 

2. Resistance Data between any two electrodes (the data that is actually 
   collected is the potential difference) - Using a Voltage divider circuit.
   > To measure the resistances between any two electrodes, (using combinatorics it is nC2, n = 16 electrodes, a total of 120 readings), the muxes 1 and 2 are enabled. This circuit works as a voltage divider circuit. For this we switch the muxes 1 and 2 from (0,1),(0,2)...(0,15)(1,2),...(14,15) and we measure the voltage difference between those pair of electrodes.
       
Note: In both the cases mentioned aboue, the value is printed out as a value from 0 to 1023, one has to convert that data according to the Analog Reference that was given. The analog reference for the voltage divider circuit is 5V and for the EIT, it is what you give initially (more on this in section IV, V and X).


## Customizing data collection:
 
  You can collect data using this code in the following ways:

  For sending order:
 
0     |        1      |        2    |      
------|---------------|-------------|
Voltage_1 |   Voltage_All |  Resistance | 
Voltage_1 |   Voltage_All |  Voltage_All| 
Voltage_1 |   Voltage_All |  Resistance | 
Voltage_1 |   Voltage_All |  Voltage_All| 
.     |       .       |      .      |
.     |       .       |      .      |
.     |       .       |      .      |


where,

    Voltage_1 :   
          Voltage values of the remaining 14 electordes when current is passing through any two adjacent
          electordes. (14x1 vector is sent)
          Note: You can switch the current to the next pair of adjacent electrodes by sending '1' (serial)
          when the code is running.
      
    Voltage_All :   
          The entire voltage matrix (14x16) is sent. This matrix is collected when the current is switched 
          between all 16 pairs of electrodes.  

    Resistance :   
          The potential difference between any two electrodes. All the 120 values are sent.
 
    prescaler_value :
          You can tell arduino at what speed to collect the data. This is determined by the prescaler value 
          of the analog read function in arduino. Refer the table in Section X, to see acceptable values.
   
    analog_ref_voltage  : 
          You can tell arduino the reference value to compare against when performing analog read. For the 
          resistance data collection this value is set to 5V. But for Voltage data collection, you can set
          any of the available reference values. Refer [here](#important-references-and-links) for more details.
   
    num_repeats : 
          You can specify how many times arduino must read the analog read from the remaining electrodes 
          (only for Voltage_All) when current is passed between any two adjacent electrodes.
   
    delay_after_mux_on  :
          You can specify the amount of time (in microseconds) the arduino can start collecting data after 
          the muxes have been turned on.
   
    time_period :
         You can specify the time period in milliseconds that the arduino should collect the data. 
         Note: If you give a time_period that is smaller than the time taken to collect and send data, then 
         there won't de any delay introduced.
 
  
## Passing Settings to Arduino:
 
  The communication with arduino is purely SERIAL. So send these settings via serial and NOT I2C or any other modes of communication.
  You can customize data collection by sending these values in a specific order to the arduino (after it has been connected serially, ofcourse).
  The order in which you have to send is:
  
Setting           |    Range  |
------------------|-----------|
num_repeats        |  [0, 255) |
analog_ref_voltage |  [1, 5]   |
prescaler_value    |  [1, 7]   // Refer [here](#important-references-and-links) |
sending_order      |  [0, 4]   |
delay_after_mux_on |  [0, 65,535) |
time_period			   |  [0, 4,198,400] // In milliseconds |
  
These values may be sent with a ' ' inbetween or a comma inbetween of each variable can be in a separate line.

Note: There are no checks performed on these values, please ensure that you send the approporate values within the bounds.
   
  
## Function of LED pins:
  
The 2 LED pins that are connected to digital pins are mentioned in the variable "indicator_pins".
It is specified as {Green_LED, Red_LED}.

    Green_LED:
        Shows that the serial connection has been established.
        If ON: Active Communication
        If OFF : Communication Disconnected
    Red_LED:
        Denotes the data collection status.
        If ON: Data has been collected
        If OFF: Data is being collected or Arduino is in "Pause" or "Stop" data collection state
 
 

  
## How data is printed:
 
The values of any variable Voltage_1 or Voltage_All or Resistance (see section IV) are sent with a ' ' (space) in between them.
After all the values of the variable are finished printing, then a newline and carraiage return character is sent. This is done to
distinguish between the two subsequent variables that are sent.

The start time (in milliseconds) is sent first (in a separate line) before any other variable is sent.

    Voltage_All:
        The Voltage values of electredes 2 to 15 are sent (0 indexed) - [current passed through 0 and 1] then 
        voltage values of electrodes 3 to 15 and 0 are sent - [current passed through 1 and 2], and so on..., 
        so a total of 14x16 = 226 values are printed with a ' ' (space) between successive values.

    Voltage_1:
        The voltage values of the electrodes corresponding to the channel (in which current is passed through) 
        are printed with a ' ' (space) between successive values.

    Resistance:
        The potential difference between (0,1),(0,2)...(0,15)(1,2),...(14,15) electrodes are printed.

Note: The values of these variables are separated by ' ' (blank space). The variables are itself separated by a newline and carriage return.
  

 
## Run, Pause and Stop Data collection:
  
While the arduino is collecting data to
    
Action| Message to send |
------|-----------------|
Pause |	 0  |
Stop  |	-1  |
Resume after pause |	 1  |
Switch current channel if sending order is 0 | 	 1  |
Warn about possible circuit failure	|   -2   |               

Note: When "-2" is sent the green and red led's flash together at a constant frequency and arduino goes to "Stop" state


## Important References and Links:

Reference Table for Prescaler:

Prescale | ADPS2,1,0 | Clock (MHz) | Sampling rate (KHz) | Data to Send 
---------|-----------|-------------|---------------------|-------------
  2      |     0 0 1 |  8          |     615              |        1         |
  4      |     0 1 0 |  4          |     307              |        2         |
  8      |     0 1 1 |  2          |     153              |        3         |
  16     |     1 0 0 |  1          |     76.8             |        4    (Good Choice, but small errors near 5V)   |
  32     |     1 0 1 |  0.5        |     38.4             |        5    (Excellent Choice) | 
  64     |     1 1 0 |  0.25       |     19.2             |        6         |
  128    |     1 1 1 |  0.125      |     9.6 (default)    |        7    (Default Value) | 
  

List of all analog reference values: (for Arduino MEGA)

    DEFAULT     : Default analog reference of 5 volts (on 5V Arduino boards) or 3.3 volts (on 3.3V Arduino boards)
    INTERNAL1V1 : A built-in 1.1V reference
    INTERNAL2V56: a built-in 2.56V reference
    EXTERNAL    : The voltage applied to the AREF pin (0 to 5V only) is used as the reference 

The value that you have to send to arduino for 

    INTERNAL1V1  : 1
    INTERNAL2V56 : 2
    3.3V         : 3  (Note: This voltage has to be provided to the external pin)
    EXTERNAL     : 4  (Note: This voltage has to be provided to the external pin)
    DEFAULT      : 5


Link to MUX Datasheet:
https://www.intersil.com/content/dam/Intersil/documents/dg40/dg406-407.pdf 
  
For list of baud rates see Page 226 in

http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf
 
## How to run code?
	
Copy and paste the contents of "/lib" to "C:\Users\user-name\Documents\Arduino\libraries\EIT" and then upload the "main.ino" in "/src" folder to Arduino.

To understand the code structure see UML diagram in "/docs" folder.
