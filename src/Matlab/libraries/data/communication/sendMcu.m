function sendMcu(serial_port, value, value_type)
% ArgIn: (serial_port, 1D array, 1D string array)
for i=1:length(value_type)
    fprintf(serial_port, value_type(i), value(i));
end
end