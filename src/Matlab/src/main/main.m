%% How to run this code:
% 1. Make sure the EIT_main.ino code is uploaded to Arudino 
% 2. Specify the necessary settings for all the files in "settings" folder.
% 3. Run the "main.m" code.

clc;
clear;
clear functions;

%% Setup
experiment = config_experiment();
model = setup_model();
[mcu, conversion_factor] = setup_mcu(model);
[file, data_info] = setup_file(experiment, mcu);
plots = setup_plots();
conversion_factor = associateConversionFactor(conversion_factor, data_info); 

%% Create objects
eit = initModel(model); % Create EIDORS object
data = initData(data_info);  % Create empty matrices to store data

disp("Opening microcontroller...");
serial_port = openMcu(mcu); % Establish a connection
clear data_info;

%% Send settings and start data collection
pause(2);   % wait for 2 seconds for mcu to get ready
sendMcu(serial_port, mcu.to_send.data, mcu.to_send.type); % send settings
pause(0.1);

disp("Starting data collection...");
sendMcu(serial_port, 1, "%d\n"); % start data collection
pause(2);   % wait before starting to collect data

%% Get data from mcu, convert it and then store it in 'data'
new_data = getData(serial_port, conversion_factor);
data = insertData(new_data, data, model.injection_pattern);

data = removeData(data, 1); % remove the 1st set of data

%% Keep collecting and displacy reconstruction until time is over
tic;
while toc < experiment.duration_to_collect
    new_data = getData(serial_port, conversion_factor);
    data = insertData(new_data, data, model.injection_pattern);
    
    [reconstructed_image, raw_matrix] = solveModel(eit, data, getCount(data), 1);
    plotImage(raw_matrix, reconstructed_image, plots);
end

%% Close and save
sendMcu(serial_port, -1, "%d\n"); % end data collection
closeMcu(serial_port, mcu);

disp("Saving data...");
save_path = saveData(data, file, experiment, mcu, model);
disp("Data Saved to " + save_path);