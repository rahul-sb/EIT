function new_data = convertData(new_data, conversion_factor)
% Conversion of data from micrcontroller to real-world units (voltage,
% resistance and seconds) is done by this function. 
% This function doesn't transform these voltages to the differences of 
% voltages that is fed into the EIDORS "inv_solve" function. To see that
% conversion see "insertData.m" file.

fields = fieldnames(new_data);

for i=1:numel(fields)
    idx = new_data.(fields{i}).idx;
    
    % Convert only if there's something to convert.
    if idx > 1
        val = new_data.(fields{i}).val;
        len = new_data.(fields{i}).len;
        
        % Convert to voltages, resistances and time.
        val = convert(val, len, conversion_factor, "voltage");
        val = convert(val, len, conversion_factor, "resistance");
        val = convert(val, len, conversion_factor, "time");
        
        % Note: In the above 4 lines there has to be fields voltage,
        %       resistance and time in the conversion factor variable.
        %       If you want to remove that hard association then uncomment
        %       the below code and comment out the above code.
%         conversion_fields = fieldnames(conversion_factor);
%         for j=1:numel(conversion_fields)
%             val = convert(val, len, conversion_factor, conversion_fields(j));
%         end

        new_data.(fields{i}).val = val;
    end    
end

end

function val = convert(val, len, conversion_factor, entity)
% Converts only when there is correct association: see "conv_fact" variable
% in "setup_file.m" file.

% Check if the lengths match. Note: Lengths are used as IDs.
match = any(len == conversion_factor.(entity).len);    
if any(match)
    val = conversion_factor.(entity).fn(val);
end

end
