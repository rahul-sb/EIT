function conversion_factor = associateConversionFactor(conversion_factor, data_info)

conversion_factor.voltage.len    = data_info.conv_fact.voltage;
conversion_factor.resistance.len = data_info.conv_fact.resistance;
conversion_factor.time.len       = data_info.conv_fact.time;

end