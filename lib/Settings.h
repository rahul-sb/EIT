
#include "stdint.h"
#include "Arduino.h"

#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct{ // Variables are ordered for smallest size (of struct)
	uint16_t delay_after_mux_on;// [0, 1, ..., 65535] in milliseconds
	uint8_t num_repeats;		// [0, 1, ..., 255]
	uint8_t inj_pattern;		// [1, 2, ..., 15]
	uint8_t adc_ref_voltage;	// [1, 2, ..., 5]
	uint8_t adc_prescaler;		// [1, 2, ..., 7]
	uint8_t sending_order;		// [0, 1 ,2]		
} Settings;

void get(Settings& settings);
void setAdcRefVoltage(const uint8_t& adc_ref_voltage);
void setAdcPrescaler(const uint8_t& adc_prescaler);

#endif

/* 
	Order of data received:
		num_repeats
		inj_pattern
		adc_ref_voltage
		adc_prescaler
		sending_order
		delay_after_mux_on
*/
/*
 
						  adc_prescaler: reference table

  Prescale  ADPS2,1,0  Clock (MHz)  Sampling rate (KHz)    	  adc_prescaler
    2           0 0 1   8               615                         1
    4           0 1 0   4               307                         2
    8           0 1 1   2               153                         3
    16          1 0 0   1               76.8                        4 (Max)   Small errors near 5V
    32          1 0 1   0.5             38.4                        5 (Excellent Choice)
    64          1 1 0   0.25            19.2                        6
    128         1 1 1   0.125           9.6 (default)               7

 */
