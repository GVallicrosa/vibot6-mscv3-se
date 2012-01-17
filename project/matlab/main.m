function main()
% Main file for Matlab code without GUI

%% Clean everything
close all;
clear all;
clc;

%% Global variables
global Options;    % Options for all processing

%% Options (editable)
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


%% Add necessary paths to call all functions
addpath('gui', 'ShapeReconstruction', 'Rotational Offset');
if ~isdir('output')
    mkdir('output');
end

%% Search for images
dirname = '/home/guillem/Desktop/UE1_softwareEng/project/code_original/NHS Code/Images/'; % previous value: '../../vibot6_mscv3/Images/';
files = dir(dirname);
fileIndex = find(~[files.isdir]);

%for i = 1:length(fileIndex)
    
    %% Open image
    fname = 'Different0019.jpg';
    %fname = files(fileIndex(i)).name;
    im = imread([dirname,fname]);
    display(['- ', fname]);
    
    %% Calculate everything
    [nhs,noiseRem,cleanImg,imCE,OutputImg, contOrig,contCE,rotOff,paramGIELIS] = ProcessImage(im);
    
    %% Save all outputs
    % IMAGES
    if(Options.NHS_output)
        filepath= ['output/', fname(1:length(fname)-4),'_nhs.png'];
        imwrite(nhs,filepath,'PNG','BitDepth',1);
    end
    if(Options.POST_out_noise)
        filepath= ['output/', fname(1:length(fname)-4),'_noiseRem.png'];
        imwrite(noiseRem,filepath,'PNG','BitDepth',1);
    end
    if(Options.POST_out_clean)
        filepath= ['output/', fname(1:length(fname)-4),'_cleanImg.png'];
        imwrite(cleanImg,filepath,'PNG','BitDepth',1);
    end
    if(Options.CE_output)
        filepath= ['output/', fname(1:length(fname)-4),'_CE.png'];
        imwrite(imCE,filepath,'PNG','BitDepth',1);
    end
    if(Options.GIELIS_output)
        filepath= ['output/', fname(1:length(fname)-4),'_output.png'];
        imwrite(OutputImg,filepath,'PNG','BitDepth',8);
    end
    % FILES
    if Options.CE_file_before
        sname = 'contOrig';
        saveText(contOrig, fname, sname);
    end
    if Options.CE_file_after
        sname = 'contCE';
        saveText(contCE, fname, sname);
    end
    if Options.RO_output
        sname = 'rotOff';
        saveText(rotOff, fname, sname);
    end
    if Options.GIELIS_param
        sname = 'paramGielis';
        saveText(paramGIELIS, fname, sname);  
    end
    
%     %% Image segmentation
% 		% You can pass arbitrary hue max and min, sat min. e.g.:
% 		% normalize_segmentation(im, 'other', 11, 230, 60);
%     nhs = normalize_segmentation(im, NHS_color);
%     [m n] = size(nhs); % for future use
%     if NHS_output
%         IMname = ['output/',fname(1:length(fname)-3), '_nhs.png'];
%         imwrite(nhs,IMname,'PNG','BitDepth',1);
%     end
%     
%     %% Postprocessing
%     [noiseRem,cleanImg] = Postprocessing(nhs, POST_aspectArea, POST_lowRatio, POST_highRatio);
%     if POST_output
%         IMname = ['output/',fname(1:length(fname)-3), '_noiseRem.png'];
%         imwrite(noiseRem,IMname,'PNG','BitDepth',1);
%         IMname = ['output/',fname(1:length(fname)-3), '_cleanImg.png'];
%         imwrite(cleanImg,IMname,'PNG','BitDepth',1);
%     end
%     
%     %% Label image
%     [L,N] = bwlabel(cleanImg, 8);              %Labeling of the clean image
%     for i=1:N                                  %For each labeled region
%         display(['  -- contour ', num2str(i),'/',num2str(N)])
%         [rows cols] = find(L==i);              %Get the coordinates of the points
%         tmpI = (L==i);                         %Make a temporary binary image of the current label
%         
% 
%         contP = bwtraceboundary(tmpI, [rows(1),cols(1)], 'E', 8, Inf, 'counterclockwise');
%                                                %Get the points in the boundary of the blob       
%         %% Contour extraction
%         valid_contour = contour_extraction(contP, CE_distError);
%         if CE_output
%             % Save contours
%             Fname = ['output/',fname(1:length(fname)-3), '_cont', num2str(i),'.txt'];
%             save(Fname,'valid_contour','-ASCII');
%             % Save contour image
%             imCE = points2image(valid_contour, m, n);
%             IMname = ['output/',fname(1:length(fname)-3), '_cont',num2str(i),'.png'];
%             imwrite(imCE,IMname,'PNG','BitDepth',1);
%         end
%         
%         %% Rotational offset
%         [Radius, Theta] = Cartisian2Polar(valid_contour);
%         [Theta,Permutation_Index] = sort(Theta,'ascend');
%         Radius = Radius(Permutation_Index);
%         Offset = FindMinimum(Radius, Theta);
%         
%         %% Gielis curves reconstruction
%         Output = ShapeReconstruction(valid_contour,Offset, GIELIS_norm, GIELIS_func);
%         if GIELIS_output
%             Fname = ['output/',fname(1:length(fname)-3), '_shape', num2str(i),'.txt'];
%             save(Fname,'Output','-ASCII');
%         end
%         Parameters(8)
%         % Always save the output image (provisional code, ensures same image dimensions)
%         IMname = ['output/',fname(1:length(fname)-3), '_shape',num2str(i),'.png'];
%         f = figure('visible','off'); 
%         subplot('position', [0 0 1 1]);
%         imshow(im); hold on;
%         plot(Output(:,2),Output(:,1),'g','LineWidth', 2); hold off;
%         dpi = 100;
%         set(f, 'paperposition', [0 0 n/dpi m/dpi]);
%         set(f, 'papersize', [n/dpi m/dpi]);
%         print(f, sprintf('-r%d',dpi), '-dpng', IMname);
%         %imwrite(Output,IMname,'PNG');%,'BitDepth',1);
%         
%     end
    display('We are Finished!');
    
%end
end

function saveText(var, fname, sname)
fname = fname(1:length(fname)-4);
for j=1:length(var)
    data = var{j};
    path = ['output/', fname, '_', sname, num2str(j), '.txt']; % save name
    save(path,'data','-ASCII');
end
end