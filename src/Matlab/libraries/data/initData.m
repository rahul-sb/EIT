function data = initData(data_info)
% data : (struct)
%    - field_name : (struct) 
%              - val : actual data is stored
%              - idx : index where next data should be put
%              - len : length of vector i.e., num of rows
%    - next_field_name
%              - val
%              - idx
%              - len
%       ... so on


% Generate a struct with field names according to excel sheet names 
% defined in setup_file function.

for i=1:size(data_info.names,1)
    field_name   = data_info.names(i);
    field_fn     = data_info.fn;
    field_size   = data_info.sizes(i,:);
    
    data.(field_name).idx = 1;
    data.(field_name).len = field_size(1);
    data.(field_name).val = field_fn(field_size);
end

end