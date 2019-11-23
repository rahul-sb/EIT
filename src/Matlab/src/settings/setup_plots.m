function plots = setup_plots(varargin)
% Args: Plot handles (max. 2)

if nargin==0
    plots = gobjects(2,1);
    plots(1) = subplot(1,2,1);
    plots(2) = subplot(1,2,2);
    
elseif nargin==2
    plots = gobjects(2,1);
    plots(1) = varargin{1};
    plots(2) = varargin{2};
    
else
    error("Only 2 plots are to be created!");
end


end

