%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         Shape Reconstruction         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                      %
% Project Manager: Angga Reza Fardana  %
% Contributors:                        %
%    - Yukti Suri                      %
%    - Ajad Chhatkuli                  %
%    - Gao Lijia                       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ err ] = ShapeReconstruction(SmoothData, ErrorOfFit)
% [INPUTS]
% SmoothData  : contains the already-processed image data
%               to be optimized and reconstructed
%
% [OUTPUTS]
% ErrorOfFit  : contains the minimum of cost function
%
% [TESTING]
%
% ErrorOfFit = ShapeReconstruction(SmoothData);
% figure(1); imshow(SmoothData);
% title('Smoothed Data');
% figure(2); imshow('?');
%

% Make a reference to the folder containing our required functions.
addpath '.\ShapeReconstruction';

global Parameters;
Parameters(1) = 1;
Parameters(2) = 3;

ErrorOfFit = Optimize2(SmoothData, ...
    false, ... % activate normalization or not
    1); % the potential function (use 1 or 2, 3 possible
        % but not the most stable)
end