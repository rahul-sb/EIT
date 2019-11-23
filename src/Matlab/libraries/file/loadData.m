function [settings, data, path_name] = loadData(path_name, comment, file_format)
% file_format is a 1D string array and is for the data not settings.
% example: "*.csv" or "*.xlsx"

dir_path = getFolder(path_name, comment);

% Read the settings file
settings_file = dir(dir_path + "\" + "*json*");
json = readcell(settings_file.name);
settings = jsondecode(char(json{1}));

% Read the data
filenames = dir(dir_path + "\" + file_format);
for i=1:numel(filenames)
    val = readmatrix(filenames{i}.name);
    len = size(val,1);
    idx = settings.sheets.sizes(:,1) == len;
    fieldname = settings.sheets.names(idx);
    
    data.(fieldname).val = val;
    data.(fieldname).idx = size(val, 2) + 1;
    data.(fieldname).len = len;
    
    % Check if any value is missing:
    if settings.sheets.sizes(idx, 2) ~= size(val, 2)
        warning("Some data might be missing!");
    end
end

% If no data is available
if ~any(settings.sheets.sizes(:, 2) > 0)
    data = [];
    warning("No data available!");
end

end