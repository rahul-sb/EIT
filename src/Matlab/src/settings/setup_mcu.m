function [mcu ,conversion_factor, model] = setup_mcu(model)


%% Main microcontroller settings
mcu_type = 0; % 0 or 1 :ArduinoMega or STM32
com_port = 'COM3';
ref_voltage = 5.0; % 1.1V or 2.56V or 3.3V or 5.0V
res_in_series = 1980; % in Ohms. Series resistance for the 3rd MUX

num_repeats = 16;
sampling_frequency = 25; % in Hz (approx)
adc_pre_scalar_value = 4;   % see appendix below
sending_order = 2; % 0 - noise tests, 1 - only voltage data, 2 - both voltage and resistance data
delay_after_mux_on = 20; % in microseconds

%% Less improtant microcontroller settings

baud_rate = 2000000;
ip_buf_sz = 51200; % in bytes, which are stored from serial in MATLAB
op_buf_sz = 51200;

%% Package mcu

init = var2struct(com_port, baud_rate, ip_buf_sz, op_buf_sz);

settings = var2struct(num_repeats, ref_voltage, adc_pre_scalar_value,...
                      sending_order, delay_after_mux_on, sampling_frequency,...
                      res_in_series);

to_send.data = [num_repeats;...
               model.injection_pattern(2);...
               floor(ref_voltage);...
               adc_pre_scalar_value;...
               sending_order;...
               delay_after_mux_on];
           
to_send.type = repmat("%d\n", size(to_send.data,1),1);

mcu = var2struct(mcu_type, init, settings, to_send);

%% Get conversion factors

% For voltage
if mcu_type==0
    ref_voltage_res = 5; % Ref. voltage for resistance calc. in Arduino is taken as 5V
    num_bits_analog_read = 10; % Arduino gives ADC values in range [0,1023].
    
    Adc2Volts_eit = ref_voltage/num_repeats/(2^num_bits_analog_read);
    Adc2Volts_res = ref_voltage_res/num_repeats/(2^num_bits_analog_read);
    
    voltage.fn = @(ADC) ADC .* Adc2Volts_eit; 
    resistance.fn = @(ADC) abs(ADC .*Adc2Volts_res .*res_in_series ./(ref_voltage_res - ADC.*Adc2Volts_res));
elseif mcu_type==1
    ref_voltage_res = 3.3; % Ref. voltage for resistance calc. in ARM is taken as 3.3V
    voltage.fn = @(V) V * 1; % ARM directly gives voltages.
    resistance.fn = @(V) abs(V .*res_in_series ./(ref_voltage_res - V)); % Voltage divider circuit.
else
    error("Wrong mcu type specified");
end

% For resistance it is the voltage divider circuit.
time.fn = @(current) current * 1e-3; % Assuming time is given in milliseconds.

conversion_factor = var2struct(voltage, resistance, time);

end

%% Appendix
%{ 
                       ARDUINO_PRE_SCALER_VALUE reference table (ONLY for ADC)

  Prescale  ADPS2,1,0  Clock (MHz)  Sampling rate (KHz)   ARDUINO_PRE_SCALER_VALUE
    2 	        0 0 1 	8 	            615                         1
    4 	        0 1 0 	4 	            307                         2
    8 	        0 1 1 	2 	            153                         3
    16 	        1 0 0 	1 	            76.8                        4 (Max)   Small errors near 5V
    32 	        1 0 1 	0.5 	        38.4                        5 (Excellent Choice)
    64 	        1 1 0 	0.25 	        19.2                        6
    128 	    1 1 1 	0.125 	        9.6 (default)               7
%}