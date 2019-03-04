/*
	EIT_Circuit.h - Library that supervises the EIT data collection and 
	transmittion according to the setting received.

	Created by Rahul Subramonian Bama, November 09, 2018
	GNU GPL License
 */

#include "Arduino.h"
#include "EIT_DataCollector.h"
#include "EIT_Mux.h"
#include "EIT_Settings.h"
#include "EIT_Circuit.h"




EIT_Circuit::EIT_Circuit(uint8_t (*ptr)[5], uint8_t ind_ptr[], uint8_t num_rows) : EIT_DataCollector(ptr, num_rows)
{
	_indicator_pins = ind_ptr;
	setIndicatorPins();
	turnIndicatorOnOff(0,0);
	turnIndicatorOnOff(1,0);
}




void EIT_Circuit::setIndicatorPins()
{
	for(uint8_t i = 0; i < 2; i++)	{
		pinMode(_indicator_pins[i], OUTPUT);
	}
}




void EIT_Circuit::turnIndicatorOnOff(uint8_t pin, uint8_t on_or_off)
{
	digitalWrite(_indicator_pins[pin], on_or_off);
}




void EIT_Circuit::getSettings()
{
	// Wait till arduino gets the settings 
	waitForSignal();
	recordSettings();

	// Turn on the 1st LED to denote that serial connection has been established
	turnIndicatorOnOff(0,1);	

	// Wait for any signal to start collecting data
	waitForSignal();
	Serial.parseInt();
	Serial.read();
}




void EIT_Circuit::getData()
{
	// Record the start time and then collect the voltage and/or
	// resistance data based on the sending order
	recordStartTime();

	collectVoltageData();

	if (sending_order == 2)
		collectResistanceData();
}




void EIT_Circuit::sendData()
{
	// Print the start time and then print the voltage and/or
	// resistance data based on the sending order
	printStartTime();

	switch(sending_order)
	{
		case 0:
				printVoltageData();
				break;

		case 1:
				printVoltageData();
				break;

		case 2:
				printResistanceData();
				printVoltageData();
				break;
	}

	// Send all the data before proceeding
	Serial.flush();

	if (Serial.available()>0)	{
		changeCollectionStatus();
	}
	
	//Match the frequency
	long time_delay = time_period - (millis() - getStartTime());

	if (time_delay > 0)
		delay(time_delay);
}




void EIT_Circuit::changeCollectionStatus()
{
	/* While collecting data,
	   Pause :  0
	   Stop  : -1
	   Resume after pause : 1
	   Switch current channel if sending order is 0 : 1
	   Stop because of possible circuit failure     : -2
	*/

	int value_received;

	value_received = Serial.parseInt();
	Serial.read();

	if (sending_order == 0 && value_received == 1)
		switchCurrentChannel();
	else if (value_received == -1)
	{
		turnIndicatorOnOff(0,0);
		turnIndicatorOnOff(1,0);

		// Turn off all the muxes to conserver power
		turnMuxOnOff(0,0);
		turnMuxOnOff(1,0);
		turnMuxOnOff(2,0);
		while(1){};
	}
	else if (value_received == -2)
	{
		// Turn off all the muxes to conserver power
		turnMuxOnOff(0,0);
		turnMuxOnOff(1,0);
		turnMuxOnOff(2,0);

		while(1){

			turnIndicatorOnOff(0,0);
			turnIndicatorOnOff(1,0);

			delay(1000);

			turnIndicatorOnOff(0,1);
			turnIndicatorOnOff(1,1);

			delay(1000);
		};
	}
	else 
	{
		// Turn off all the muxes to conserver power
		turnMuxOnOff(0,0);
		turnMuxOnOff(1,0);
		turnMuxOnOff(2,0);
		
		byte right_signal = 0;

		while(!right_signal)
		{
			waitForSignal();

			byte val = Serial.parseInt();
			Serial.read();

			if (val == 1)
				right_signal = 1;
			else
				right_signal = 0;
		}
	}
}
