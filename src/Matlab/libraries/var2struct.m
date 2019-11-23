function out_struct = var2struct(varargin)

% CallerWS = evalin('caller','whos');

inputNames = cell(1,nargin);
for iArgin = 1:nargin
  inputNames{iArgin} = inputname(iArgin);
end

for i=1:nargin
    out_struct.(inputNames{i}) = varargin{i};
end

end