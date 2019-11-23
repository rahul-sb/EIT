function experiment = config_experiment()

%% Specify Experiment Details
sensor_name = 'HU20190519';
done_by = "Rahul"; % Name of person doing the experiment
notebook_details = "3, 130"; % Book No., Page No.
duration_to_collect = 90;   % in seconds

experiment = var2struct(sensor_name, done_by, notebook_details, duration_to_collect);
end


