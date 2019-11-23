function [file, data_info, experiment, mcu] = setup_file(experiment, mcu)

userpath('reset');
% userpath % Uncomment this line to see the user path
%% Enter File Details
dir_path = userpath + "\EIT_Experiments\";
extension = ".csv"; % ".xlsx" or ".csv" only.

% Create Unique ID's for file name
time_format = 'yyyy-mm-dd-HH-MM-SS';
time_stamp = string(datestr(now, time_format));
fileName = @(prefix, file_format) prefix + erase(time_stamp,'-') + file_format;


%% Excel sheet names or CSV file names.  
% Note: This is where all the hard associations between variables (for the
%       for the entire code) is. I'm treating the vector length as a unique
%       identifer across the data.

names = ["SingleChannelVoltages";
         "EITVoltages";
         "AllVoltagesRawMcu";
         "Resistances";
         "Timestamp"];

num_readings = mcu.settings.sampling_frequency*(experiment.duration_to_collect + 10); % tentative

sizes = [ 14, num_readings; % matrix size for names(1) sheet
         256, num_readings; % Note: There is 1 to 1 correspondence 
         224, num_readings; %       with "names" variable.
         120, num_readings;
           1, num_readings];

% Since I'm taking the vector length as a unique ID, while converting the 
% data from microcontroller (mcu), these values are used for voltage readings.
conv_fact.voltage =    [  14; 
                         224];
% 256 is not mentioned here since we don't get a 256 length vector from the
% microcontroller. 

conv_fact.resistance = [ 120];% This ID (vector length) is used for resistance readings
conv_fact.time       = [   1];% This one for time.

fn = @nan;

% The order of conversion doesn't matter. You can convert voltage,
% resistance and time in any order. Since the equations are made 
% independent of the other. See "setup_mcu.m" for declaration of 
% conversion factors.

%% Create appropriate structure
date_num = datenum(time_stamp ,time_format);
file = var2struct(dir_path, extension, time_format, time_stamp, date_num, fileName);

data_info = var2struct(names, sizes, conv_fact, fn);

end
