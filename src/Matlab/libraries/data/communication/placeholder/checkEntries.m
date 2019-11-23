function dimension_match = checkEntries(placeholder, len)
% Checks the vector length of each field in 'placeholder' and
% returns true if 'len' matches with at least any one of the 
% vector length.

dimension_match = false;

if ~isempty(placeholder)
    fields = fieldnames(placeholder);
    for i=1:numel(fields)
        match = (len == placeholder.(fields{i}).len);
        dimension_match = (dimension_match | match);
    end
end

end