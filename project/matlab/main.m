%% Main file for Matlab code without GUI
% Currently edited by:
% - Guillem
% - Angga

%% Clean everything
close all;
clear all;
clc;

%% Options
% NHS
NHS_color  = 'red';      % color to segment 'blue' or 'red'
NHS_output = true;       % save output for this function
% PostProcessing
POST_aspectArea = 1500;  % imageArea/aspectArea thresholding (default 1500)
POST_lowRatio   = 0.25;  % height/width lowest value (default 0.25)
POST_highRatio  = 1.3;   % height/width highest value (default 1.3)
POST_output     = true;  % save output for this function
% Contour extraction
CE_distError = sqrt(2);  % maximum distance from the convex hull to take contour pixels (default sqrt(2))
CE_output    = true;     % save output for this function
% Gielis recontruction
GIELIS_norm = true;      % use normalization (default true)
GIELIS_func = 1;         % cost function to use 1, 2 or 3 (default 1)
GIELIS_output = true;    % save output for this function


%% Add necessary paths to call all functions
addpath('gui', 'ShapeReconstruction', 'Rotational Offset');
if ~isdir('output')
    mkdir('output');
end

%% Search for images
dirname = '../../vibot6_mscv3/Images/';
files = dir(dirname);
fileIndex = find(~[files.isdir]);

%for i = 1:length(fileIndex)
    
    %% Open image
    fname = 'Different0035.jpg';
    %fname = files(fileIndex(i)).name;
    im = imread([dirname,fname]);
    display(['- ', fname]);
    
    %% Image segmentation
    nhs = normalize_segmentation(im, NHS_color);
    [m n] = size(nhs); % for future use
    if NHS_output
        IMname = ['output/',fname(1:length(fname)-3), '_nhs.png'];
        imwrite(nhs,IMname,'PNG','BitDepth',1);
    end
    
    %% Postprocessing
    [noiseRem,cleanImg] = Postprocessing(nhs, POST_aspectArea, POST_lowRatio, POST_highRatio);
    if POST_output
        IMname = ['output/',fname(1:length(fname)-3), '_noiseRem.png'];
        imwrite(noiseRem,IMname,'PNG','BitDepth',1);
        IMname = ['output/',fname(1:length(fname)-3), '_cleanImg.png'];
        imwrite(cleanImg,IMname,'PNG','BitDepth',1);
    end
    
    %% Label image
    [L,N] = bwlabel(cleanImg, 8);              %Labeling of the clean image
    for i=1:N                                  %For each labeled region
        display(['  -- contour ', num2str(i),'/',num2str(N)])
        [rows cols] = find(L==i);              %Get the coordinates of the points
        tmpI = (L==i);                         %Make a temporary binary image of the current label
        contP = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');
                                               %Get the points in the boundary of the blob
                                           
        %% Contour extraction
        valid_contour = contour_extraction(contP, CE_distError);
        if CE_output
            % Save contours
            Fname = ['output/',fname(1:length(fname)-3), '_cont', num2str(i),'.txt'];
            save(Fname,'valid_contour','-ASCII');
            % Save contour image
            imCE = points2image(valid_contour, m, n);
            IMname = ['output/',fname(1:length(fname)-3), '_cont',num2str(i),'.png'];
            imwrite(imCE,IMname,'PNG','BitDepth',1);
        end
        
        %% Rotational offset
        [Radius, Theta] = Cartisian2Polar(valid_contour);
        [Theta,Permutation_Index] = sort(Theta,'ascend');
        Radius = Radius(Permutation_Index);
        Offset = FindMinimum(Radius, Theta);
        
        %% Gielis curves reconstruction
        Output = ShapeReconstruction(valid_contour, Offset, GIELIS_norm, GIELIS_func);
        if GIELIS_output
            Fname = ['output/',fname(1:length(fname)-3), '_shape', num2str(i),'.txt'];
            save(Fname,'Output','-ASCII');
        end
        % Always save the output image (provisional code, ensures same image dimensions)
        IMname = ['output/',fname(1:length(fname)-3), '_shape',num2str(i),'.png'];
        f = figure('visible','off'); 
        subplot('position', [0 0 1 1]);
        imshow(im);
        hold on;
        % Output = valid_contour; % for testing
        plot(Output(:,2),Output(:,1),'g','LineWidth', 3);
        hold off;
        [H,W,D] = size(im);
        dpi = 100;
        set(f, 'paperposition', [0 0 W/dpi H/dpi]);
        set(f, 'papersize', [W/dpi H/dpi]);
        print(f, sprintf('-r%d',dpi), '-dpng', IMname);
        %imwrite(Output,IMname,'PNG');%,'BitDepth',1);
    end
%end