%% Test Program for Shape Reconstruction
% This program is used to do the low-level testing of the program
%

SmoothData = zeros(0);
ErrofOfFit = zeros(0);

err = Optimize2(SmoothData, ErrorOfFit, ...
    false, ... % activate normalization or not
    1); % the potential function (use 1 or 2, 3 possible
        % but not the most stable)