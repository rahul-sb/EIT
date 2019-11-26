
#include "Pins.h"


void set(Pins& pins, const uint8_t leds[2], const uint8_t muxes[3][5]){
	// Transfer pin numbers to RAM and set as digital out
	for (uint8_t i = 0; i < 2; i++){
		pins.leds[i] = leds[i];

		pinMode(pins.leds[i], OUTPUT);
		digitalWrite(pins.leds[i],0);
	}

	for (uint8_t i = 0; i < 3; i++){
		for (uint8_t j = 0; j < 5; j++){
			pins.muxes[i][j] = muxes[i][j];

			pinMode(pins.muxes[i][j], OUTPUT);
			digitalWrite(pins.muxes[i][j],0);
		}
	}
}


void turnLedOnOff(const Pins& pins, const uint8_t& led_no, const uint8_t& on_or_off){
	digitalWrite(pins.leds[led_no], on_or_off);
}


void turnMuxOnOff(const Pins& pins, const uint8_t& mux_no, const uint8_t& on_or_off){
	digitalWrite(pins.muxes[mux_no][4], on_or_off);
}


void switchMuxState(const Pins& pins, const uint8_t& mux_no, const ReferenceTable& table, const uint8_t& binary_index){
	for (uint8_t i = 0; i < 4; i++) {
		digitalWrite(pins.muxes[mux_no][3-i], table.binary_table[i][binary_index]);
	}
}