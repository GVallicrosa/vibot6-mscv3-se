%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         Shape Reconstruction         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                      %
% Project Manager: Angga Reza Fardana  %
% Contributors:                        %
%    - Yukti Suri                      %
%    - Ajad Chhatkuli                  %
%    - Gao Lijia                       %
%    - Andru Putra Twinanda            %
%    - Ozan Tonkal                     %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ Output ] = ShapeReconstruction(Data, RotOffsets, Normalization, functionused)
% [INPUTS]
% Data        : contains the already-processed image data
%               to be optimized and reconstructed
% RotOffsets  : contains the rotational offset value
%
% [OUTPUTS]
% Output      : 
%
% [TESTING]
%
%

    % Make a reference to the folder containing our required functions.
    addpath('ShapeReconstruction');
    global EPSILON;
    global Parameters;
    
    EPSILON = 1e-9;
    
    mError = 1e30;
    
    for j = 1:length(RotOffsets)
        for i = 2:5
            switch i
%                 case 1
%                     ptmp = 1;
                case 2
                    ptmp = 3;
                case 3
                    ptmp = 4;
                case 4
                    ptmp = 6;
                case 5
                    ptmp = 8;
            end
            
            % Initialise global variable Parameters
            Parameters = [1, 1, ...   % a, b (a == b)
                1, 2, 2, ...          % n1, n2, n3
                ptmp, 1, ...          % p, q
                RotOffsets(j), 0, ... % rotationalOffset, phiOffset
                0, 0, 0];             % centerX, centerY, centerZ 
            
            Error = Optimize(Data, Normalization, functionused);
            
            if (Error < mError)
                mError = Error;
                bestParameters = Parameters;
            end
        end
    end
    
    if exist('bestParameters')
        Parameters = bestParameters;
    end
    
    centerX = Parameters(10);
    centerY = Parameters(11);
    Output = zeros(0);
    
    
    for i = 0:0.01:2*pi
        r = radius_(i);
        x = cos(i + Parameters(8))*r + centerX; 
        y = sin(i + Parameters(8))*r + centerY;
        Output(end+1,:) = [x, y];
    end
end