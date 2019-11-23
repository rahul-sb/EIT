function [eit, model] = initModel(model)

% Create model
eit = mk_common_model(model.model_type,...
                      model.num_electrodes);

% Define the stimulation pattern structure:
[stim, meas_select] = mk_stim_patterns(model.num_electrodes,...
                                       model.is_2d, ...
                                       model.injection_pattern, ...
                                       model.measurement_pattern, ...
                                       model.options, ...
                                       model.current_supply);

% Build model:
eit.fwd_model.stimulation = stim;
eit.fwd_model.meas_select = meas_select;

% Specify the solver details
eit.solve = @nodal_solve;
eit.RtR_prior = @prior_laplace;      
    

% % Define jacobian background
% eit.jacobian_bkgnd.value = model.jacobian_bkgnd;
    
% % Remove Errorenous electrodes
% eit.meas_icov = meas_icov_rm_elecs(eit, ...
%                                    model.rm_elecs);

end