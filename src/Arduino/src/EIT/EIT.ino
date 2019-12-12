
#include "stdint.h"
#include "Settings.h"
#include "ReferenceTable.h"
#include "Pins.h"
#include "Data.h"

// Digital pins for LED and Mux 
// Note: These are defined as "static const" so that these are stored in flash memory and not RAM. 
static const uint8_t LED_PINS[2] = {18,19};
static const uint8_t CONTROL_PINS_MUXES[3][5] = {{12,11,10,9,13},    //Format: {A0,A1,A2,A3,EN}    
                                  				{46,48,50,52,44},    //        {LSB,...,MSB,ENABLE} (of Mux)
                                  				{26,28,30,32,34}};

static const uint32_t SERIAL_BAUD_RATE = 2000000;


// Functions (defined in end of file):
void waitForSignal(); 
void debugSignal(const Pins&);

static Data data;
static ReferenceTable ref_table;
static Pins pins;
static Settings settings;


void setup(){
  
	clearAll(data);
	set(ref_table);
	set(pins, LED_PINS, CONTROL_PINS_MUXES);
	
	Serial.begin(SERIAL_BAUD_RATE);

	// Wait till arduino gets the settings 
	waitForSignal();
	get(settings);

	// Turn on the 1st LED to denote that serial connection has been established
	turnLedOnOff(pins,0,1);	

	// Wait for any signal to start collecting data
	waitForSignal();
	Serial.parseInt();
	Serial.read();
}



void loop(){
	// Turn off the 2nd LED to denote the start of data collection
	turnLedOnOff(pins,1,0);

	// Print start time in milli seconds
	uint32_t start_time = millis();
	Serial.println(start_time);

	switch(settings.sending_order){
		case 0:
				readVoltage(data, settings, pins, ref_table);	
        turnLedOnOff(pins,1,1);	// Turn on the 2nd LED to denote that the data has been collected
				printVoltage(data);
				clearVoltage(data);
				break;
		case 1: 
				readResistance(data, settings, pins, ref_table);
				turnLedOnOff(pins,1,1);  // Turn on the 2nd LED to denote that the data has been collected
				printResistance(data);
				clearResistance(data);
				break;
		default:
				readVoltage(data, settings, pins, ref_table);
				readResistance(data, settings, pins, ref_table);
				turnLedOnOff(pins,1,1);  // Turn on the 2nd LED to denote that the data has been collected
				printVoltage(data);
				printResistance(data);
				clearAll(data);
	}

	// Send all the data before proceeding
	Serial.flush();

	// Check if any instruction is available else proceed to next iteration
	if (Serial.available() > 0){

		// Turn off all the muxes to conserver power
		turnMuxOnOff(pins,0,0);
		turnMuxOnOff(pins,1,0);
		turnMuxOnOff(pins,2,0);

		debugSignal(pins);

		// Turn all muxes back on for next iteration
		turnMuxOnOff(pins,0,1);
		turnMuxOnOff(pins,1,1);
		turnMuxOnOff(pins,2,1);
	}
}




// Supporting functions:

void waitForSignal(){
	while(!Serial.available()){};
}


void debugSignal(const Pins& pins){
	bool proceed_next_iteration = false;

	while(!proceed_next_iteration){

		int8_t val = Serial.parseInt();
		Serial.read();
		/* 
			 1	: Resume after pause 
		   	-1	: Stop  
			-2	: Stop because of possible circuit failure
			def : Pause
		*/
		switch(val){
			case  1:
					turnLedOnOff(pins,1,1);
					proceed_next_iteration = true;
					break;
			case -1:
					turnLedOnOff(pins,0,0);
					turnLedOnOff(pins,1,0);
					while(1){}; // Never proceed
					break;
			case -2:
					while(1){ 	// Blink both led's indefinitely
						turnLedOnOff(pins,0,0);
						turnLedOnOff(pins,1,0);
						delay(1000);
						turnLedOnOff(pins,0,1);
						turnLedOnOff(pins,1,1);
						delay(1000);
					}
					break;
			default:			// Pause
					turnLedOnOff(pins,1,0);
					waitForSignal();
		}
	}
}
