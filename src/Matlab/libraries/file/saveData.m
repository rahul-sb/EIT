function save_path = saveData(data, file, experiment, mcu, model)

% Create folder structure
save_path = file.dir_path ...
                + datestr(file.date_num,'yyyy_mm_dd') +"\"...
                + experiment.sensor_name +"\"...
                + datestr(file.date_num,'HH_MM_SS') +"\";

if ~exist(save_path,'dir')
    mkdir(save_path);
end
addpath(genpath(save_path));

% Save Settings
settings_json = getSettingsAsJSON(data, experiment, mcu, model);
writematrix(settings_json, ...
            save_path + file.fileName("settings_json", ".txt")); % matlab cannot write it as json directly.

% Save Data
fields = fieldnames(data);
for i=1:numel(fields)
    % Write only if there is something to write
    if data.(fields{i}).idx > 1
        writematrix(data.(fields{i}).val, ...
                    save_path + file.fileName(fields{i},file.extension));
    end
end

end

function settings_json = getSettingsAsJSON(data, experiment, mcu, model)

fields = fieldnames(data);
sizes = zeros(numel(fields),2);
for i=1:numel(fields)
    sizes(i,:) = [data.(fields{i}).len, data.(fields{i}).idx - 1];
end
names = fieldnames(data);
sheets = var2struct(names,sizes);

settings_json = jsonencode(var2struct(experiment, mcu, model, sheets));
settings_json = string(settings_json);

end