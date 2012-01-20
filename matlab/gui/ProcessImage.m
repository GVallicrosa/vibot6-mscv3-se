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
function [nhs,noiseRem,cleanImg,imCE,OutputImg, contOrig,contCE,rotOff,paramGIELIS] = ProcessImage(inputImage)
% does road sign detection from input image

global Options; % All options in a single variable

%% Image segmentation
% You can pass arbitrary hue max and min, sat min. e.g.:
% normalize_segmentation(im, 'other', 11, 230, 60);
nhs = normalize_segmentation(inputImage, Options.NHS_color);
[m n] = size(nhs);
% Test save
% IMname = ['output/',currfilename(1:length(currfilename)-3),'_nhs.png'];
% imwrite(nhs,IMname,'PNG','BitDepth',1); 

%% Postprocessing
[noiseRem,cleanImg] = Postprocessing(nhs, Options.POST_aspectArea, Options.POST_lowRatio, Options.POST_highRatio);
% Test save
% IMname = ['output/',currfilename(1:length(currfilename)-3),'_noiseRem.png'];
% imwrite(noiseRem,IMname,'PNG','BitDepth',1); 
% IMname = ['output/',currfilename(1:length(currfilename)-3),'_cleanImg.png'];
% imwrite(cleanImg,IMname,'PNG','BitDepth',1);

%% Label image
[L,N] = bwlabel(cleanImg, 8);   % Labeling of the clean image to recognize regions

%% Empty image and variables to save all together
% IMAGE
imCE = zeros(size(cleanImg)); % image to overwrite all contours
% FILES
contOrig = cell(1,N);    % save contours before contour extraction
contCE = cell(1,N);      % save contours after contour extraction
rotOff = cell(1,N);      % save rotational offsets obtained
paramGIELIS = cell(1,N); % save gielis parameters
% Extra for drawing gielis image
pointGIELIS = cell(1,N);

if N>0                          % If we have regions, continue processing
    for i=1:N                       % For each labeled region
        display(['-- Working on region ', num2str(i),'/', num2str(N),'...']);
        [rows cols] = find(L==i);   % Get the coordinates of all the points
        tmpI = (L==i);              % Make a temporary binary image of the current label
        % Get the points in the boundary of the region
        contP = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');
        contOrig{i} = contP;
                                           
        %% Contour extraction
        valid_contour = contour_extraction(contP, Options.CE_distError); % Extract the contour
        contCE{i} = valid_contour;
        % Write contour to the contour image
        for j=1:length(valid_contour)
            imCE(valid_contour(j,1), valid_contour(j,2)) = 1;
        end
        
        %% Rotational offset
        [Radius, Theta] = Cartisian2Polar(valid_contour);
        [Theta,Permutation_Index] = sort(Theta,'ascend');
        Radius = Radius(Permutation_Index);
        Offset = FindMinimum(Radius, Theta);
        rotOff{i} = Offset;
        
        %% Gielis curves reconstruction
         warning off all; % Suppress the warnings for subsequent calculations in ShapeReconstruction
         Output = ShapeReconstruction(valid_contour, Offset, false, Options.GIELIS_func);
         pointGIELIS{i} = Output;
         global Parameters;
         paramGIELIS{i} = Parameters;
         warning on all;
    end
    
    %% Process Gielis image (Guillem: I don't know how to do this without plots)
    f = figure('visible','off');
    subplot('position', [0 0 1 1]);
    imshow(inputImage);
    hold on;
    %pointGIELIS = contCE; % only for testing without gielis
    for k=1:length(pointGIELIS)
        points = pointGIELIS{k};
        plot(points(:,2),points(:,1),'g','LineWidth', 1);
    end
    hold off;
    dpi = 100;
    set(f, 'paperposition', [0 0 n/dpi m/dpi]);
    set(f, 'papersize', [n/dpi m/dpi]);
    print(f, sprintf('-r%d',dpi), '-dpng', 'temp.png');
    OutputImg = imread('temp.png');
    delete('temp.png');
    
else  %to avoid error if no contour is found dispaly input image
    OutputImg = inputImage;
end

end % function end
