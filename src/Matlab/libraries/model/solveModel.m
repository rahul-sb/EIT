function [reconstructed_image, raw_matrix] = solveModel(eit, data, current_idx, start_idx)

persistent field_name baseline;

if isempty(field_name)
    field_name = getEitVectorFieldname(data);
    baseline = inv_solve(eit,...
                         data.(field_name).val(:,start_idx),...
                         zeros(256,1));
end

reconstructed_image = inv_solve(eit,...
                                data.(field_name).val(:,current_idx),...
                                data.(field_name).val(:,start_idx));

raw_matrix = calc_slices(reconstructed_image, [Inf,Inf,0]);

% If you need the baseline data uncomment the below code and pass them as
% arguments
% baseline_image = baseline;
% baseline_matrix = calc_slices(reconstructed_image, [Inf,Inf,0]);

end


function field_name = getEitVectorFieldname(data)

fields = fieldnames(data);

for i=1:numel(fields)
    if data.(fields{i}).len == 256
        field_name = fields{i};
        break;
    end
end

end