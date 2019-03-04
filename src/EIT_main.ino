/**
 * Electrical Impedance Tomography Data collection
 * 
 *
 * Author  : Rahul Subramonian Bama
 * E-Mail  : sb.rahul@hotmail.com
 * Date    : 15th November 2018
 *
 *
 * Note: This code works on an Arduino Mega only.
 * 
 * See ReadMe.txt for more details.
 */

#include<EIT_Circuit.h>

// =====================================================================================
// Enter Circuit Details
// =====================================================================================
 

// Enter the digital pins for the indicator pins and muxes
uint8_t INDICATOR_PINS[] = {7,6};
uint8_t CONTROL_PINS_MUXES[][5] = {{47,49,51,53,45},    //Format: {A0,A1,A2,A3,EN}    
                                  {12,11,10,9,13},     //        {LSB,...,MSB,ENABLE} (of Mux)
                                  {24,26,28,30,22}};

#define SERIAL_BAUD_RATE 2000000

// =====================================================================================
// DO NOT change anything below!!!
// =====================================================================================

// Get the number of rows of CONTROL_PINS_MUXES
#define NUM_ROWS sizeof(CONTROL_PINS_MUXES)/sizeof(CONTROL_PINS_MUXES[0])

// Create a model of the circuit
EIT_Circuit eit_circuit(CONTROL_PINS_MUXES, INDICATOR_PINS, NUM_ROWS);	

// =====================================================================================

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	eit_circuit.getSettings();
}

// =====================================================================================

void loop()
{
  // Turn off the 2nd LED to denote the start of data collection
	eit_circuit.turnIndicatorOnOff(1,0);

  // Collect the relevant data by switching the muxes
	eit_circuit.getData();

  // Turn on the 2nd LED to denote that the data has been collected
	eit_circuit.turnIndicatorOnOff(1,1);

  // Send the appropriate data
	eit_circuit.sendData();
}

// =====================================================================================
