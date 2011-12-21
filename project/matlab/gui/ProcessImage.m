%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         GUI                          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                      %
%    Suman Raj Bista(Manager)          %
%    Pablo Speciale	                   %
%    Nolang Fanani	                   %
%    Nathanael Lemessa Baisa	       %
%    Taman Upadhaya	                   %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [nhs,noiseRem,cleanImg,imCE] = ProcessImage(inputImage)
    %,O5,O6,O7,O8,outputImage
   
 % does road sign detection from input image
 %integration of other codes for GUI takem from main.m
 
 %% Image segmentation
  [nhs] = normalize_segmentation(inputImage);
  
   %% Postprocessing
  [noiseRem,cleanImg] = Postprocessing(nhs); 

%% Label image
  [L,N] = bwlabel(cleanImg, 8)              %Labeling of the clean image
  imCE = zeros(size(cleanImg));
  if N>0
    for i=1:N                                  %For each labeled region
        [rows cols] = find(L==i);              %Get the coordinates of the points
        tmpI = (L==i);                         %Make a temporary binary image of the current label
        contP = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');
                                               %Get the points in the boundary of the blob
                                           
 %% Contour extraction
        outputCE = true;
        distanceError = sqrt(2);
        valid = contour_extraction(contP, distanceError);
            imCE = zeros(size(cleanImg));
            for k=1:length(valid)
                imCE(valid(k,1), valid(k,2)) = 1;
            end
    end
 end
    
 end
