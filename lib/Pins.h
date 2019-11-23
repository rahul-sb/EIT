
#include "stdint.h"
#include "Arduino.h"

#ifndef PINS_H
#define PINS_H

typedef struct{
	uint8_t muxes[3][5];
	uint8_t leds[2];
} Pins;

void set(Pins& pins, const uint8_t leds[2], const uint8_t muxes[3][5]);

void turnLedOnOff(const Pins& pins, const uint8_t& led_no, const uint8_t& on_or_off);
void turnMuxOnOff(const Pins& pins, const uint8_t& mux_no, const uint8_t& on_or_off);

#endif
