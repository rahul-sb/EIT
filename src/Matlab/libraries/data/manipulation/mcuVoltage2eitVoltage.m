function eit_vectors = mcuVoltage2eitVoltage(voltages, injection_pattern)

persistent placeholder indices vectorize;
    
if isempty(placeholder)
    [placeholder, indices] = getIndices(injection_pattern);
    vectorize = @(mat) reshape(diff([mat;mat(1,:)]), 256,1);
end

eit_vectors = nan(256, size(voltages,2)); % this has more rows

for i=1:size(voltages,2)
    placeholder(indices(:)) = voltages(:,i);
    eit_vectors(:,i) = vectorize(placeholder);    
end

eit_vectors(isnan(eit_vectors)) = 0;

end


function [placeholder, indices] = getIndices(inj)

placeholder = ones(16);
indices = zeros(14,16);

placeholder(1,:) = 0;
placeholder(inj+1, :) = 0;

for i=1:16
    placeholder(:,i) = circshift(placeholder(:,i), i-1);
end

ribbon_mat = [tril(placeholder); triu(placeholder,1)];

for i=1:16
    indices(:,i) = find(ribbon_mat(:,i)) + 16*(i-1);
    idx = indices(:,i)>16*i;
    if any(idx)
        indices(idx,i) = indices(idx,i)-16;
    end
end

placeholder(placeholder==0) = nan;

end