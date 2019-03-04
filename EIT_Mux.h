/*
	EIT_Mux.h - Library that controls the mux
	Created by Rahul Subramonian Bama, November 08, 2018
	GNU GPL License
 */

#ifndef EIT_MUX_H
#define EIT_MUX_H

#include "Arduino.h"

class EIT_Mux
{
public:
	/* Store the control pins and the number of muxes
	   The 5 control pins for a mux are specified in a row
	   and if we have 3 muxes, then it will be a 3x5 matrix.
	   The control pins are given in this Format: {A0,A1,A2,A3,EN}
	   Which corresponds to {LSB,...,MSB,ENABLE} (of Mux)
	*/ 
	EIT_Mux(uint8_t (*ptr)[5], uint8_t num_rows); 

	//Turns the mux on or off. Note: mux_no is zero indexed
	void turnMuxOnOff(uint8_t mux_no, uint8_t on_or_off);	

	//Switches the mux to a particular value: 0 to 15
	void switchMux(uint8_t mux_no, uint8_t binary_index);

private:
	uint8_t (*_control_pins)[5];	// Stores the address of the pins
	byte _num_muxes;
	byte _binary_table[4][16];	// See below for structure

	// Creates a binary table so that it is easier to switch the mux
	void createBinaryTable();

	// Sets the mux pins as OUTPUT
	void setMuxPins();
};

#endif

/* 
  Structure of Binary Table (4x16 matrix):
  0,0,0,0     1
  0,0,0,0     1
  0,0,1,1     1
  0,1,0,1 ... 1
*/