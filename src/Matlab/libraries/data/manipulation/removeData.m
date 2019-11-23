function data = removeData(data, indices)

fields = fieldnames(data);

for i=1:numel(fields)
    if data.(fields{i}).len == 1
        max_index = data.(fields{i}).idx - 1; % Get index of timestamp
        break;
    end
end

out_of_range = (indices < 1) | (indices > max_index);

if any(out_of_range)
    warning("Some indices are out of range. Not removing any data!");
else
    for i=1:numel(fields)
        data.(fields{i}).val(:,indices) = nan; 
        val = rmmissing(data.(fields{i}).val,2);
        idx = size(val,2);
        data.(fields{i}).val = nan(size(data.(fields{i}).val));
        data.(fields{i}).val(:,1:idx) = val;       
        data.(fields{i}).idx = idx + 1;
    end
end

end