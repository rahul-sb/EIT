

#include "Data.h"


void clearAll(Data& data){
	clearVoltage(data);
	clearResistance(data);
}


void clearVoltage(Data& data){
	for (uint8_t i = 0; i < 14; i++)  {
		for (uint8_t j = 0; j < 16; j++)  {
		  data.voltage[i][j] = 0;
		}
	}
}


void clearResistance(Data& data){
	for (uint8_t i = 0; i < 120; i++)	{
		data.resistance[i] = 0;
	}
}


void printVoltage(const Data& data){
	for (uint8_t i = 0; i < 16; i++)	{
		for (uint8_t j = 0; j < 14; j++)	{
			Serial.print(data.voltage[j][i]);
			Serial.print(' ');
		}
	  }
	Serial.println(); 
}


void printResistance(const Data& data){
	for (uint8_t i = 0; i < 120; i++)	{
		Serial.print(data.resistance[i]);
		Serial.print(' ');
	}	  
	Serial.println();
}


void readVoltage(Data& data, const Settings& settings, const Pins& pins, const ReferenceTable& table){
	// Set the reference voltage
	setAdcRefVoltage(settings.adc_ref_voltage);

	// Turn on the Muxes 0 and 1 and turn off Mux 2, to collect voltage data
	turnMuxOnOff(pins,0,1);
	turnMuxOnOff(pins,1,1);
	turnMuxOnOff(pins,2,0);

	// Collect for each of the 16 channels through which current passes
	for (uint8_t channel = 0; channel < 16; channel++)  {
		// Turn on the right switches in both the muxes
		switchMuxState(pins, 0, table, table.mux_vector[channel]);
		switchMuxState(pins, 1, table, table.mux_vector[channel + settings.inj_pattern]);

		// Give a delay before reading analog-in pins.
		delayMicroseconds(settings.delay_after_mux_on);

		// Read the analog pins from the other (turned off) electrodes (for 'num_repeats' number of times)
		for (uint8_t i = 0; i < settings.num_repeats; i++) {
			uint8_t rem_pins = 0;
			for(uint8_t in_pins = 1; in_pins < settings.inj_pattern; in_pins++){
				data.voltage[rem_pins][channel] += analogRead(table.mux_vector[in_pins + channel]);
				rem_pins++;
			}

			for (uint8_t out_pins = settings.inj_pattern + 1; out_pins<16; out_pins++){
				data.voltage[rem_pins][channel] += analogRead(table.mux_vector[out_pins + channel]);
				rem_pins++;
			}
		}
    }
}


void readResistance(Data& data, const Settings& settings, const Pins& pins, const ReferenceTable& table){
	// Set the reference voltage
	setAdcRefVoltage(5);

	// Turn off Mux 0 and turn on Muxes 1 and 2, to collect resistance data
	turnMuxOnOff(pins,0,0);
	turnMuxOnOff(pins,1,1);
	turnMuxOnOff(pins,2,1);

	// Zero the index to store the data in the correct location in the data.resistance
	uint8_t index_count = 0;

	// Switch and collect all combinations of data for resistance
	for (uint8_t mux2_pos = 0; mux2_pos < 16; mux2_pos++)	{ 
		for (uint8_t mux1_pos = mux2_pos + 1; mux1_pos < 16; mux1_pos++)	{
			// Turn on the right switches in both the muxes
			switchMuxState(pins, 2, table, mux2_pos);
			switchMuxState(pins, 1, table, mux1_pos);          

			// Give a delay before reading analog-in pins.
			delayMicroseconds(settings.delay_after_mux_on);

			// Read the Difference in voltage values
			for (uint8_t i = 0; i < settings.num_repeats; i++) {
				data.resistance[index_count] += analogRead(mux2_pos) - analogRead(mux1_pos);
			}
			index_count++;            
		}                
	}
}
