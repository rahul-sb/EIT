function data = insertData(new_data, data, injection_pattern)
% Insert the values in new_data to data, performing 
% any additional transformations, if necessary (like
% converting the 224 voltage readings to 256 voltage vector
% that the EIDORS "inv_solve" function can accept.

fields = fieldnames(new_data);

for i=1:numel(fields)    
    idx = new_data.(fields{i}).idx;
    
    % Insert only if there's something to insert
    if idx > 1 
        val = new_data.(fields{i}).val(:,1:idx-1);
        len = new_data.(fields{i}).len;
        
        data = insert(val, len, idx, data);

        % Convert to eit voltage vector.
        if len == 224
            val = mcuVoltage2eitVoltage(val, injection_pattern);
            len = size(val,1);
            data = insert(val, len, idx, data);
        end  
    end
end

end

function data = insert(new_val, new_len, new_idx, data)

% Iterate through each field in "data" and update the 
% data when the dimensions match.
fields = fieldnames(data);
for i=1:numel(fields)
    len = data.(fields{i}).len; 

    if len == new_len
        idx = data.(fields{i}).idx;
        data.(fields{i}).val(:, idx : idx+new_idx-2) = new_val;
        data.(fields{i}).idx = idx+new_idx-1;
        break;
    end
end

end