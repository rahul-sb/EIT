/*
	EIT_DataCollector.h - Library that collects data for EIT Imaging by switching the mux
	Created by Rahul Subramonian Bama, November 09, 2018
	GNU GPL License
 */


#include "Arduino.h"
#include "EIT_DataCollector.h"
#include "EIT_Mux.h"
#include "EIT_Settings.h"
#include "EIT_Circuit.h"


EIT_DataCollector::EIT_DataCollector(uint8_t (*ptr)[5], uint8_t num_rows) : EIT_Mux(ptr, num_rows)
{
	createMuxVector();
	zeroVoltageData();
	_current_through_pin = 0;
}




void EIT_DataCollector::switchCurrentChannel()
{
	_current_through_pin = _mux_vector[_current_through_pin + 1];
}




void EIT_DataCollector::createMuxVector()
{
  /**
   * Define the number the corresponding mux must point to in the corresponding loop 
   * 
   * Structure of mux_vector (32x1 vector):
   * [0 ,1 ,... , 15, 0, 1, ..., 15]'
   */
  for (uint8_t i = 0; i < 32; i++)
  {
    if (i < 16)
      _mux_vector[i] = i;
    else
      _mux_vector[i] = i - 16;
  }
}




void EIT_DataCollector::zeroVoltageData()
{
  for (uint8_t i = 0; i < 14; i++)  {
    for (uint8_t j = 0; j < 16; j++)  {
      _voltage_data[i][j] = 0;
    }
  }
}




void EIT_DataCollector::collectVoltageData()
{
  // Set the reference voltage
  setReferenceVoltage(analog_ref_voltage);

  // Turn on the Muxes 0 and 1 and turn off Mux 2, to collect voltage data
  turnMuxOnOff(0,1);
  turnMuxOnOff(1,1);
  turnMuxOnOff(2,0);
  
  // Assign zero initial values to the voltage readings 
  zeroVoltageData();
  
  // Collect from just one channel or all channels depending on sending_order
  if (sending_order == 0) 
    readVoltageForChannel(_current_through_pin);
  else {
    for (uint8_t channel = 0; channel < 16; channel++)  {
        readVoltageForChannel(channel);
    }
  }
}




void EIT_DataCollector::readVoltageForChannel(uint8_t channel_no)
{
  // Turn on the right switches in both the muxes
  switchMux(0, _mux_vector[channel_no]);
  switchMux(1, _mux_vector[channel_no + 1]);

  // Give a delay before reading analog-in pins.
  delayMicroseconds(delay_after_mux_on);

  uint8_t from_pin = _mux_vector[channel_no + 2];

  // Read the analog pins from the other (turned off) electrodes (for 'n' number of times)
  for (uint8_t i = 0; i < num_repeats; i++) {
      for (uint8_t remaining_pins = 0;remaining_pins < 14; remaining_pins++)  {
          _voltage_data[remaining_pins][channel_no] += analogRead(_mux_vector[remaining_pins + from_pin]);
      }
  }
}




void EIT_DataCollector::printVoltageData()
{
	uint8_t starting_channel, ending_channel;

  // Print according to sending order
	if (sending_order == 0)
	{
		starting_channel = _current_through_pin;
		ending_channel = _mux_vector[_current_through_pin + 1];
	}
	else
	{
		starting_channel = 0;
		ending_channel = 16;
	}

	for (uint8_t i = starting_channel; i < ending_channel; i++)	{
	    for (uint8_t j = 0; j < 14; j++)	{
	      Serial.print(_voltage_data[j][i]);
	      Serial.print(' ');
	    }
	  }
	   Serial.println(); 
}




void EIT_DataCollector::printResistanceData()
{
	for (uint8_t i = 0; i < 120; i++)	{
	    Serial.print(_resistance_data[i]);
	    Serial.print(' ');
	  }	  
	  Serial.println();
}




void EIT_DataCollector::collectResistanceData()
{
  // Set the reference voltage
  setReferenceVoltage(5);

  // Turn on the Muxes 1 and 2 and turn off Mux 0, to collect voltage data
  turnMuxOnOff(1,1);
  turnMuxOnOff(2,1);
  turnMuxOnOff(0,0);

  // Zero the resistance data before collecting any values
  zeroResistanceData();

  // Zero the index to store the data in the correct location in the _resistance_data
  uint8_t index_count = 0;

  // Switch and collect all combinations of data for resistance
  for (uint8_t mux2_position = 0; mux2_position < 16; mux2_position++)	{ 
      for (uint8_t mux1_position = mux2_position + 1; mux1_position < 16; mux1_position++)	{

        // Turn on the right switches in both the muxes
        switchMux(2, mux2_position);
        switchMux(1, mux1_position);          

        // Give a delay before reading analog-in pins.
        delayMicroseconds(delay_after_mux_on);
        
        // Read the Difference in voltage values
        for (uint8_t i = 0; i < num_repeats; i++) {
        	_resistance_data[index_count] += analogRead(mux2_position) - analogRead(mux1_position);
        }
        index_count++;            
      }                
    }
}




void EIT_DataCollector::zeroResistanceData()
{
	for (uint8_t i = 0; i < 120; i++)	{
		_resistance_data[i] = 0;
	}
}




void EIT_DataCollector::recordStartTime()
{
  _start_time = millis();
}




void EIT_DataCollector::printStartTime()
{
  Serial.println(_start_time);
}




unsigned long EIT_DataCollector::getStartTime()
{
  return _start_time;
}
