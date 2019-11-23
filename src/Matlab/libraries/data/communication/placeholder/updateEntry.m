function placeholder = updateEntry(placeholder, values)
% Inserts values to the placeholder if the dimensions match.
% If there is a dimension mis-match then it throws out an error.

fields = fieldnames(placeholder);
dimension_match = false;
len = size(values,1); % vector length of values

for i=1:numel(fields)
    % Check if the vector length matches
    match = (len == placeholder.(fields{i}).len);
    dimension_match = (dimension_match | match);
    
    % If it matches insert into matrix
    if match
        num_values = size(values,2);
        idx = placeholder.(fields{i}).idx;
        
        placeholder.(fields{i}).val(:, idx:idx+num_values-1) = values;
        placeholder.(fields{i}).idx = idx + num_values;
        
        % Note: Later on add code to increase num of columns for 'val'
        %       if by inserting 'values' crosses 'idx'.
        
        break; 
    end
end

% If the dimension doesn't match with any of the existing 'val' in all the 
% fields.
if ~dimension_match
    error("Cannot find the correct entry to store the data from MCU");
end


end