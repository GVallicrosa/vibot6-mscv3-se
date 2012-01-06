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
function [nhs,noiseRem,cleanImg,contCE,imCE,rotOff,pointGIELIS,OutputImg] = ProcessImage(inputImage)
% does road sign detection from input image
% integration of other codes for GUI takem from main.m
% global currfilename;

% No need to pass all the parameters using global Options variable
global Options; % All options in a single variable
currfilename = Options.currfilename;

%% Image segmentation
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
contCE = cell(1,N); % structure for points because the length is not equal for all contours
imCE = zeros(size(cleanImg)); % image to overwrite all contours
rotOff = cell(1,N);
pointGIELIS = cell(1,N);

if N>0                          % If we have regions, continue processing
    for i=1:N                       % For each labeled region
        display(['-- Working on region ', num2str(i),'/', num2str(N),'...']);
        [rows cols] = find(L==i);   % Get the coordinates of all the points
        tmpI = (L==i);              % Make a temporary binary image of the current label
        % Get the points in the boundary of the region
        contP = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');              
                                           
        %% Contour extraction
        valid_contour = contour_extraction(contP, Options.CE_distError); % Extract the contour
        contCE{i} = valid_contour;
        % Write contour to the contour image
        for j=1:length(valid_contour)
            imCE(valid_contour(j,1), valid_contour(j,2)) = 1;
        end
        
        %% Rotational offset
        x = valid_contour(:,2);
        y = valid_contour(:,1);
        valid_contour(:,1) = x;
        valid_contour(:,2) = y;
        % Process rotational offset
        [Radius, Theta] = Cartisian2Polar(valid_contour);
        [Theta,Permutation_Index] = sort(Theta,'ascend');
        Radius = Radius(Permutation_Index);
        Offset = FindMinimum(Radius, Theta);
        rotOff{i} = Offset;
        
        %% Gielis curves reconstruction % disabled because is not working
        %% properly
%         Output = ShapeReconstruction(valid_contour, Offset, GIELIS_norm, GIELIS_func);
%         pointGIELIS{i} = Output;
    end
    
    %% Process Gielis image (Guillem: I don't know how to do this without plots)
    f = figure('visible','off');
    subplot('position', [0 0 1 1]);
    imshow(inputImage);
    hold on;
    pointGIELIS = contCE; % only for testing without gielis
    for k=1:length(pointGIELIS)
        points = pointGIELIS{k};
        plot(points(:,2),points(:,1),'g','LineWidth', 3);
    end
    hold off;
    dpi = 100;
    set(f, 'paperposition', [0 0 n/dpi m/dpi]);
    set(f, 'papersize', [n/dpi m/dpi]);
    print(f, sprintf('-r%d',dpi), '-dpng', 'temp.png');
    OutputImg = imread('temp.png');
    delete('temp.png');
    
else  %to avoid error if no contour is found
    OutputImg = NaN;
end

end % function end
