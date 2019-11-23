
#include "Settings.h"

// Functions that set and clear a bit in Registery
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


void get(Settings& settings){
	settings.num_repeats = Serial.parseInt();
	settings.inj_pattern = Serial.parseInt();
	settings.adc_ref_voltage = Serial.parseInt();
	settings.adc_prescaler = Serial.parseInt();
	settings.sending_order = Serial.parseInt();
	settings.delay_after_mux_on = Serial.parseInt();

	delay(10);
	Serial.read(); // Clear i/p buffer

	setAdcPrescaler(settings.adc_prescaler);
}


void setAdcRefVoltage(const uint8_t& adc_ref_voltage){
	switch(adc_ref_voltage) {
		case 1:
			analogReference(INTERNAL1V1);
			break;
		case 2: 
			analogReference(INTERNAL2V56);
			break;
		case 3: 
			analogReference(EXTERNAL);
			break;
		case 4:
			analogReference(EXTERNAL);
			break;
		default:
			analogReference(DEFAULT);
			break;
	}
}


void setAdcPrescaler(const uint8_t& adc_prescaler){
	if (bitRead(adc_prescaler,0))
		sbi(ADCSRA,ADPS0);
	else 
		cbi(ADCSRA,ADPS0);

	if (bitRead(adc_prescaler,1))
		sbi(ADCSRA,ADPS1);
	else 
		cbi(ADCSRA,ADPS1);

	if (bitRead(adc_prescaler,2))
		sbi(ADCSRA,ADPS2);
	else 
		cbi(ADCSRA,ADPS2); 
}
