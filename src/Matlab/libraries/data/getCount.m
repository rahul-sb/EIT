function count = getCount(data)
% Gets the count of how much data is collected by counting the timestamp.

fields = fieldnames(data);
count_found = false;

for i=1:numel(fields)
    if data.(fields{i}).len == 1
        count = data.(fields{i}).idx - 1;
        count_found = true;
        break;
    end
end

if ~count_found
    error("Timestamp data not available");
end

end