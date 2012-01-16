%% Main file for Matlab code without GUI
% Currently edited by:
% - Guillem
% - Angga

%% Clean everything
close all;
clear all;
clc;

%%Just for checking-Yukti
global Parameters;

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
CE_distError = 7;  % maximum distance from the convex hull to take contour pixels (default sqrt(2))
CE_output    = true;     % save output for this function
% Gielis reconstruction
GIELIS_norm = false;      % use normalization (default true)
GIELIS_func = 1;         % cost function to use 1, 2 or 3 (default 1)
GIELIS_output = true;    % save output for this function

% Suppress the warnings for subsequent calculations in ShapeReconstruction
warning off all;

%% Add necessary paths to call all functions
addpath('gui', 'ShapeReconstruction', 'Rotational Offset');
if ~isdir('output')
    mkdir('output');
end

%% Search for images
dirname = '/home/safir/Desktop/Temporary/Images-Red/'; % previous value: '../../vibot6_mscv3/Images/';
files = dir(dirname);
fileIndex = find(~[files.isdir]);

%for i = 1:length(fileIndex)
    
    %% Open image
    fname = 'octogonal0018.jpg';
    %fname = files(fileIndex(i)).name;
    im = imread([dirname,fname]);
    display(['Processing ', fname]);
    
    %% Image segmentation
		% You can pass arbitrary hue max and min, sat min. e.g.:
		% normalize_segmentation(im, 'other', 11, 230, 60);
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
    display(['- No. of detected shapes: ', num2str(N)]);
    for i=1:N                                  %For each labeled region
        display(['  -- Detecting contour ', num2str(i),'/',num2str(N)])
        [rows cols] = find(L==i);              %Get the coordinates of the points
        tmpI = (L==i);                         %Make a temporary binary image of the current label
        

        contP = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');
                                               %Get the points in the boundary of the blob
%          img_canny= edge(tmpI,'canny');
%          contP=find(img_canny==1);
         
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
        x = zeros(size(valid_contour,1));
        y = zeros(size(valid_contour,1));
        valid_contour1 = zeros(size(valid_contour));
        x = valid_contour(:,2);
        y = valid_contour(:,1);
        valid_contour1(:,1) = x;
        valid_contour1(:,2) = y;
        [Radius, Theta] = Cartisian2Polar(valid_contour1);
        [Theta,Permutation_Index] = sort(Theta,'ascend');
        Radius = Radius(Permutation_Index);
        Offset = FindMinimum(Radius, Theta);

        %% Gielis curves reconstruction
        display(['     --- Shape Reconstruction ', num2str(i),'/',num2str(N)])
        Output = ShapeReconstruction(valid_contour, Offset, GIELIS_norm, GIELIS_func);
        if GIELIS_output
            Fname = ['output/',fname(1:length(fname)-3), '_shape', num2str(i),'.txt'];
            save(Fname,'Output','-ASCII');
        end
        % Always save the output image (provisional code, ensures same image dimensions)
        IMname = ['output/',fname(1:length(fname)-3), '_shape',num2str(i),'.png'];
        f = figure('visible','off'); 
        subplot('position', [0 0 1 1]);
        imshow(im); hold on;
        plot(Output(:,2),Output(:,1),'g','LineWidth', 1); hold off;
        dpi = 100;
        set(f, 'paperposition', [0 0 n/dpi m/dpi]);
        set(f, 'papersize', [n/dpi m/dpi]);
        print(f, sprintf('-r%d',dpi), '-dpng', IMname);
        %imwrite(Output,IMname,'PNG');%,'BitDepth',1);
        
    end
    display('All done!');
%end
