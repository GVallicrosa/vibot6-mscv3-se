%% Main file for Matlab code GUI
% First load all options from a file, if not load default options.
% Then launches the GUI

%% Clean everything
close all;
clear all;
clc;

%% Load last used options
global Options
if exist('opt.mat','file') == 2 % file in Matlab path
    Options = load('opt.mat');
else % load and save default options
    % NHS
    Options.NHS_color  = 'red';      % color to segment 'blue' or 'red'
    Options.NHS_output = true;       % IMAGE: save output image for this function
    % PostProcessing
    Options.POST_aspectArea = 1500;  % imageArea/aspectArea thresholding (default 1500)
    Options.POST_lowRatio   = 0.25;  % height/width lowest value (default 0.25)
    Options.POST_highRatio  = 1.3;   % height/width highest value (default 1.3)
    Options.POST_out_noise  = true;  % IMAGE: save output image for this function
    Options.POST_out_clean  = true;  % IMAGE: save output image for this function
    % Contour extraction
    Options.CE_distError = 5;        % maximum distance from the convex hull to take contour pixels (default sqrt(2))
    Options.CE_output    = true;     % IMAGE: save output image for this function
    Options.CE_file_before = true;   % FILE: save contour before processing it
    Options.CE_file_after = true;    % FILE: save contour after processing it
    % Rotational offset
    Options.RO_output    = true;     % FILE: save output for this function
    % Gielis recontruction
    Options.GIELIS_func = 1;         % cost function to use 1, 2 or 3 (default 1)
    Options.GIELIS_output = true;    % IMAGE: save output for this function
    Options.GIELIS_param = true;     % FILE: save gielis parameters
    % Last folder opened
    Options.lastFolder = cd;         % last used folder
    % General output
    Options.intermediate_output = true; % checkbox in main GUI
    % Create the options file
    save opt.mat -struct Options;
end

%% Add necessary paths to call all functions
addpath('gui', 'ShapeReconstruction', 'Rotational Offset');
if ~isdir('output')
    mkdir('output');
end

%% Initializations
Options.DisplayIndex = 1; % current index of the image displayed
Options.Processflag = 0;  % tells if the image is processed or not

%% Execute GUI
gui_matlab;
