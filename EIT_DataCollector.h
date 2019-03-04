/*
	EIT_DataCollector.h - Library that collects data for EIT Imaging by switching the mux
	Created by Rahul Subramonian Bama, November 09, 2018
	GNU GPL License
 */


#ifndef EIT_DATACOLLECTOR_H
#define EIT_DATACOLLECTOR_H

#include "Arduino.h"
#include "EIT_Settings.h"
#include "EIT_Mux.h"

class EIT_DataCollector : public EIT_Mux, public EIT_Settings
{
public:
	EIT_DataCollector(uint8_t (*ptr)[5], uint8_t num_rows);

	// Functions that collects and prints voltage data for EIT image
	void collectVoltageData();
	void printVoltageData();

	// Functions that collects and prints all combination of resistance values for sensor.
	void collectResistanceData();
	void printResistanceData();

	// Record and print start time (used at the start of each cycle of data collection)
	void recordStartTime();
	void printStartTime();	
	unsigned long getStartTime();

	// If sending_order is 0 and we want to switch the current channel, call this function
	void switchCurrentChannel();	

private:
	uint8_t _mux_vector[32];	// Vector that helps to properly cycle through the mux indices, so that it can be switched properly
	unsigned long _voltage_data[14][16];	// Variable that stores the voltage data collected
	long _resistance_data[120];	// Variable that stores the resistance data collected
	unsigned long _start_time;
	uint8_t _current_through_pin = 0;

	void createMuxVector();
	void zeroVoltageData();
	void zeroResistanceData();
	void readVoltageForChannel(uint8_t channel_no);
};


#endif