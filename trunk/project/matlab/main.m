%% Main file for Matlab code
%  Implemented for:
%    - Post processing
%    - Contour extraction

%% Clean everything
close all;
clear all;

%% Search for images
dirname = '/home/guillem/Desktop/UE1_softwareEng/project/code_original/NHS Output/';
files = dir(dirname);
fileIndex = find(~[files.isdir]);

for i = 1:length(fileIndex)
    
    %% Open image
    fname = files(fileIndex(i)).name;
    im = imread([dirname,fname]);
    
    %% Postprocessing
    [noiseRem,cleanImg] = Postprocessing(im);
    
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
        valid = contour_extraction(contP, distanceError);
        % Optional outputs
        if (outputCE)
            % Save contours
            save(['outputCE/',fname,'_cont',num2str(i),'.txt'],'valid','-ASCII');
            % Save contour image
            [m n] = size(cleanImg);
            imCE = zeros(m,n);
            for k=1:length(valid)
                imCE(valid(k,1), valid(k,2)) = 1;
            end
            IMname = ['outputCE/',fname,'_cont',num2str(i)];
            imwrite(imCE,IMname,'PNG','BitDepth',1);
        end
        
        %% Rotational offset
        
        %% Gielis curves reconstruction
        
    end
end