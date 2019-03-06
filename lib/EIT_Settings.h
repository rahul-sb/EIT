/*
	EIT_Settings.h - Library that gets the settings for running the code
	Created by Rahul Subramonian Bama, November 09, 2018
	GNU GPL License
 */

#ifndef EIT_SETTINGS_H
#define EIT_SETTINGS_H

#include "Arduino.h"

class EIT_Settings
{
public:
	// List of settings collected
	uint8_t num_repeats;
	uint8_t inj_pattern;
	byte analog_ref_voltage; // 1 to 5
	byte sending_order;	// 0,1 or 2
	unsigned int delay_after_mux_on;	
	unsigned long time_period;	

	// Collect settings via serial communication
	void recordSettings();

	// The arduino stops everything till it receives
	// a signal via serial
	void waitForSignal();

	// Set A_ref value of Arduino
	void setReferenceVoltage(uint8_t a_ref);
		

private:
	byte _prescaler_value; // For ADC
	void setPrescalerValue();
};	

#endif
