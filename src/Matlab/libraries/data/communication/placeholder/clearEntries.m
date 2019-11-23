function placeholder = clearEntries(placeholder)
% Sets all the values in 'val' (for all fields) to nan.
% Sets 'idx' (for all fields) to 1.
% Leaves 'len' as it is.
% Does all the above only if the placeholder is not empty.

if ~isempty(placeholder)
    fields = fieldnames(placeholder);

    for i=1:numel(fields)
        data_size = size(placeholder.(fields{i}).val);

        placeholder.(fields{i}).val = nan(data_size);
        placeholder.(fields{i}).idx = 1;
    end
end

end