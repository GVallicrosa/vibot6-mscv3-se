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
function [nhs,noiseRem,cleanImg,valid_contour,imCE,Offset,Output] = ProcessImage(inputImage,color,aspectRatio,lowRatio,highRatio,distanceError,GIELIS_norm, GIELIS_func)
    %,O5,O6,O7,O8,outputImage
   
 % does road sign detection from input image
 %integration of other codes for GUI takem from main.m
 global currfilename;
 %% Image segmentation
  nhs = normalize_segmentation(inputImage,color);
  [m n] = size(nhs);
   %% Postprocessing
  [noiseRem,cleanImg] = Postprocessing(nhs,aspectRatio,lowRatio,highRatio); 

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
        distanceError = sqrt(2);
        valid_contour = contour_extraction(contP, distanceError);
%         generate contour image
          imCE = zeros(size(cleanImg));
         for i=1:length(valid_contour)
             imCE(valid_contour(i,1), valid_contour(i,2)) = 1;
         end
          IMname = ['output/',currfilename(1:length(currfilename)-3),num2str(i),'_countoutImg.png'];
          imwrite(imCE,IMname,'PNG','BitDepth',1); 
          Fname = ['output/',currfilename(1:length(currfilename)-3),num2str(i),'_cont','.txt'];
          save(Fname,'valid_contour','-ASCII');
  %% Rotational offset
   x = valid_contour(:,2);
y = valid_contour(:,1);
valid_contour(:,1) = x;
valid_contour(:,2) = y;

        [Radius, Theta] = Cartisian2Polar(valid_contour);
        [Theta,Permutation_Index] = sort(Theta,'ascend');
        Radius = Radius(Permutation_Index);
        Offset = FindMinimum(Radius, Theta);
   
  %% Gielis curves reconstruction
     Output = ShapeReconstruction(valid_contour, Offset, GIELIS_norm, GIELIS_func);
%      Output = imCE;
    end
  else  %to avoid error if no contour is found
      valid_contour = [-1,-1];
      Offset = 0;
      Output = [0,0];
  end
      
 end
