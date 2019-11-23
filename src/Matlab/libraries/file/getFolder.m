function path_name = getFolder(path_name, comment)

persistent dir_path;
if isempty(dir_path) || isnumeric(dir_path)
    dir_path = uigetdir(path_name, comment);
else
    dir_path = uigetdir(dir_path, comment);
end
addpath(genpath(dir_path));

path_name = dir_path;
end