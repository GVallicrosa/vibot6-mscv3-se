%% Main file for Matlab code without GUI
%  Implemented for:
%    - Post processing
%    - Contour extraction

%% Clean everything
close all;
clear all;

%% Options
% 

%% Add necessary paths to call all functions
addpath('gui', 'ShapeReconstruction', 'Rotational Offset');

%% Search for images
dirname = '/home/guillem/Desktop/UE1_softwareEng/project/code_original/NHS Code/Images/';
files = dir(dirname);
fileIndex = find(~[files.isdir]);

%for i = 1:length(fileIndex)
    
    %% Open image
    fname = 'circular0016.jpg';
    %fname = files(fileIndex(i)).name;
    im = imread([dirname,fname]);
    
    %% Image segmentation
    nhs = normalize_segmentation(im);
    
    %% Postprocessing
    [noiseRem,cleanImg] = Postprocessing(nhs);
    
    %% Label image
    [L,N] = bwlabel(cleanImg, 8);              %Labeling of the clean image
    for i=1:N                                  %For each labeled region
        [rows cols] = find(L==i);              %Get the coordinates of the points
        tmpI = (L==i);                         %Make a temporary binary image of the current label
        contP = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');
                                               %Get the points in the boundary of the blob
                                           
        %% Contour extraction
        outputCE = true;
        distanceError = sqrt(2);
        valid_contour = contour_extraction(contP, distanceError);
        % Optional outputs
        if (outputCE)
            % Save contours
            save(['outputCE/',fname,'_cont',num2str(i),'.txt'],'valid','-ASCII');
            % Save contour image
            [m n] = size(cleanImg);
            imCE = zeros(m,n);
            for k=1:length(valid)
                imCE(valid_contour(k,1), valid(k,2)) = 1;
            end
            IMname = ['outputCE/',fname,'_cont',num2str(i)];
            imwrite(imCE,IMname,'PNG','BitDepth',1);
        end
        
        %% Rotational offset
        [Radius, Theta] = Cartisian2Polar(valid_contour);
        [Theta,Permutation_Index] = sort(Theta,'ascend');
        Radius = Radius(Permutation_Index);
        Offset = FindMinimum(Radius, Theta);
        
        %% Gielis curves reconstruction
        
    end
%end