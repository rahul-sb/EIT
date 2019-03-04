/*
	EIT_Mux.h - Library that controls the mux
	Created by Rahul Subramonian Bama, November 08, 2018
	GNU GPL License
 */

#include "Arduino.h"
#include "EIT_Mux.h"




EIT_Mux::EIT_Mux(uint8_t (*ptr)[5], uint8_t num_rows)
{
	_num_muxes =  num_rows;
	_control_pins = ptr;
	
	createBinaryTable();
	setMuxPins();
}




void EIT_Mux::createBinaryTable()
{
  for (uint8_t i = 0; i < 16; i++)	{
    for (uint8_t j = 0; j < 4; j++)	{
      _binary_table[j][i] = bitRead(i,3-j);
    }
  }
}




void EIT_Mux::setMuxPins()
{
  // Define all the contol pins of Mux as Output.
  for (uint8_t i = 0; i < 5; i++)  {
    for (uint8_t j = 0; j < _num_muxes; j++)  {
      pinMode(_control_pins[j][i], OUTPUT);
      digitalWrite(_control_pins[j][i],0);
    }
  } 
}




void EIT_Mux::turnMuxOnOff(uint8_t mux_no, uint8_t on_or_off)
{
	digitalWrite(_control_pins[mux_no][4], on_or_off);
}




void EIT_Mux::switchMux(uint8_t mux_no, uint8_t binary_index)
{
	for (uint8_t pins = 0; pins < 4; pins++) {
      digitalWrite(_control_pins[mux_no][3-pins], _binary_table[pins][binary_index]);
	}
}
