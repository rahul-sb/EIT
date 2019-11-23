function new_data = getData(serial_port, conversion_factor)

new_data = getMcu(serial_port);

% The new_data can have only a max of 3 fields in any case
fields = fieldnames(new_data);
if numel(fields)>3
    % Either restart code or check 'values' in getMcu.m file
    warning("Data not correctly processed!"); 
end

new_data = convertData(new_data, conversion_factor);

end