function model = setup_model()
%% Specify Main Model Details

model_type = 'd2c4c'; % See below (in appendix) for more details
hyperparameter = 0.00002;
current_supply = 0.00007; % in Amps
max_color_reading = 50; % For Image
image_resolution = 128; % For Image


%% Specify Other Model details

num_electrodes = 16;
is_2d = 1; % 0 or 1
injection_pattern = [0 1]; % [0 1] specifies adjacent electrodes, see appendix
measurement_pattern = [0 1]; % [0 1] specifies measurement for adjacent electrodes, see appendix
options = {'no_meas_current','no_rotate_meas'};  % See appendix below for more details



%% Package into struct
model = var2struct(model_type, hyperparameter, current_supply, max_color_reading,...
                   image_resolution, num_electrodes, is_2d, injection_pattern, ...,
                   measurement_pattern, options);

end




%% Appendix

%{
    Injection pattern and Measurement pattern:
        If you are changing the injection pattern, you may have to increase 
        the battery voltage to 18V in order to maintain the current, as now 
        the current flows through different electrodes. Also you may have to
        increase the reference voltage in arduino in order to accomodate the 
        increase in voltage.
  
        Currently there is no support for specifying different measurement
        patterns other tha [0 1].
%}

%{
    Options:
       'no_meas_current' -  Dont make measurements on current carrying electrodes
       'no_rotate_meas'  -  Dont rotate the image after each measurement
%}

%{ 
    Model type:
        a-j => mesh density
        2d  => 2d Distmesh model
        0-4 => element refinement
        c   => circular mesh
%}