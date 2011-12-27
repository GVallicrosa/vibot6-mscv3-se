%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         Post-Processing          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Responsibles:
%    - Corina Barbalata
%    - Federico Camposeco
%    - Eduardo Tusa
%    - Joel Vidal

function [noiseRem,cleanImg] = Postprocessing(BW_im, aspectArea, lowRatio, highRatio)
%   ---Inputs----
%   BW_im      : a segmented binary image [mxn bool]
%   aspectArea : threshold to eliminate objects with area less
%                than imageArea/aspectArea;
%   lowRatio   : lowest value for aspect ratio (height/width)
%   highRatio  : highest value for aspect ratio (height/width)
%
%   ---Outputs---
%   noiseRem: contains the noise removed image (after morphological
%             operations). [mxn bool]
%   cleanImg: contains the object elimination image. [mxn bool]
%
%   ---Testing---
%   [noiseRem,cleanImg,convHull,contP]=Postprocessing(NHS_output);
%   figure(1);imshow(noiseRem)
%   title('Noise Removal');
%   figure(2);imshow(cleanImg)
%   title('Object Elimination');
%   figure(3);imshow(cleanImg)
%   title('Recovery of Deformed Road Sign');
%   hold on
%   for i=1:length(contP)
%       plot(contP{i}(convHull{i},2),contP{i}(convHull{i},1),'.')
%   end

%% Image-Cleaning
%Remove the noise, restore the contours and fill incomplete data.

sd = strel('disk',4);                   %Create a structuring element
cleanImg = imdilate(BW_im, sd);         %Dilate the image
cleanImg = imfill(cleanImg,8,'holes');  %Fill the holes
cleanImg = imerode(cleanImg,sd);        %Erode the image

for i = 1:5                             %Noise removal using a median filter
    cleanImg = medfilt2(cleanImg,[5 5]);
end

noiseRem = cleanImg;                    %Output of noise removed

%% Elimination 
%Elimination of insignicant objects according two methods : area and aspect ratio

[L,N] = bwlabel(cleanImg, 8);           %Label connected components: 8-connected blobs      
totalSize= size(BW_im,1)*size(BW_im,2); %size(I,1): rows, size(I,2): columns                                
BB = regionprops(L,'BoundingBox');      %smallest rectangle containing the object
P  = regionprops(L,'PixelIdxList');     %Vector containing the linear indices that represent an object
A  = regionprops(L,'Area');             %Number of pixels in the object

for i = 1:N                             %For each region in the image do:
    B = BB(i).BoundingBox;
    ratio = B(3)/B(4);                  %Compute aspect ratio. B(3): width, B(4): height
                                        %Conditions are set for elimination objects                                   
    if (A(i).Area < totalSize/aspectArea) || (ratio > highRatio || ratio < lowRatio)
        cleanImg(P(i).PixelIdxList) = 0;
    end
end             

%% TO ERASE
% %% Convex hull
% % Obtaining the points in the segmented region that correspond to the CH
% 
% [L,N] = bwlabel(cleanImg, 8);              %Labeling of the latest clean image
% contP = cell(N);
% convHull = cell(N);                        %Output data structure will be in cell form
% for i=1:N                                  %For each labeled region
%     [rows cols] = find(L==i);              %Get the coordinates of the points
%     tmpI = (L==i);                         %Make a temporary binary image of the current label
%     contP{i} = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');
%                                            %Get the points in the boundary of the blob
%     convHull{i} = convhull(contP{i}(:,1),contP{i}(:,2));
%                                            %Convex hull operation of the contour (as index of contP)
% end
