/*
	EIT_Settings.h - Library that gets the settings for running the code
	Created by Rahul Subramonian Bama, November 09, 2018
	GNU GPL License
 */

#include "Arduino.h"
#include "EIT_Settings.h"

// Define functions that clear and set the bits in the registry
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))



void EIT_Settings::recordSettings()
{
  num_repeats = Serial.parseInt();
  analog_ref_voltage = Serial.parseInt();
  _prescaler_value = Serial.parseInt();
  sending_order = Serial.parseInt();
  delay_after_mux_on = Serial.parseInt();
  time_period = Serial.parseInt();

  delay(10);

  // Clear the I/P buffer
  Serial.read();

  setPrescalerValue();  
}




void EIT_Settings::waitForSignal()
{
  // Wait till arduino receives a serial data
  while (Serial)
  { 
    if (Serial.available()>0)
        break;    
  }
}




void EIT_Settings::setReferenceVoltage(uint8_t a_ref)
{	
	switch(a_ref)
   {
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
      case 5: 
          analogReference(DEFAULT);
          break;
      default:
          analogReference(DEFAULT);
          break;
   } 
}



void EIT_Settings::setPrescalerValue()
{    
  // Set default prescaler value if we get a zero (or if timeout occurs)
  if (!_prescaler_value)
    _prescaler_value = 7;
  
  // Set the Prescaler Value
  if (bitRead(_prescaler_value,0))
    sbi(ADCSRA,ADPS0);
  else 
    cbi(ADCSRA,ADPS0);
    
  if (bitRead(_prescaler_value,1))
    sbi(ADCSRA,ADPS1);
  else 
    cbi(ADCSRA,ADPS1);
  
  if (bitRead(_prescaler_value,2))
    sbi(ADCSRA,ADPS2);
  else 
    cbi(ADCSRA,ADPS2); 
}

/*
 
                       ARDUINO_PRE_SCALER_VALUE reference table (ONLY for ADC)

  Prescale  ADPS2,1,0  Clock (MHz)  Sampling rate (KHz)   ARDUINO_PRE_SCALER_VALUE
    2           0 0 1   8               615                         1
    4           0 1 0   4               307                         2
    8           0 1 1   2               153                         3
    16          1 0 0   1               76.8                        4 (Max)   Small errors near 5V
    32          1 0 1   0.5             38.4                        5 (Excellent Choice)
    64          1 1 0   0.25            19.2                        6
    128         1 1 1   0.125           9.6 (default)               7

 */