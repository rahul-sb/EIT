/*
	EIT_Circuit.h - Library that supervises the EIT data collection and 
	transmittion according to the setting received.

	Created by Rahul Subramonian Bama, November 09, 2018
	GNU GPL License
 */
	
#ifndef EIT_CIRCUIT_H
#define EIT_CIRCUIT_H

#include "Arduino.h"
#include "EIT_DataCollector.h"
#include "EIT_Mux.h"
#include "EIT_Settings.h"

class EIT_Circuit : private EIT_DataCollector
{
public:
	EIT_Circuit(uint8_t (*ptr)[5], uint8_t ind_ptr[], uint8_t num_rows);

	// Gets settings via serial communication properly
	void getSettings();

	// Gets the voltage and (or) resistance data according to the sending_order requested
	void getData();

	// Send the data according to sending_order
	void sendData();

	// Turns the indicator pins on or off
	void turnIndicatorOnOff(uint8_t pin, uint8_t on_or_off);

private:
	uint8_t *_indicator_pins;
	void setIndicatorPins();
	void changeCollectionStatus();	// Pauses, stops or resumes data collection depending on value received during runtime
};

#endif
