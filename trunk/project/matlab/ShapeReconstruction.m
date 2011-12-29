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
        for i = 1:5
            switch i
                case 1
                    ptmp = 1;
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
                2, 2, 2, ...          % n1, n2, n3
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
    
    centerX = bestParameters(10);
    centerY = bestParameters(11);
%     Output = zeros(0);
    
    a = bestParameters(1);
    b = bestParameters(2);
    n1 =bestParameters(3);
    n2 =bestParameters(4);
    n3 =bestParameters(5);
    m = bestParameters(6);
    offset = bestParameters(8);
    phi = 0:0.1:2*pi;
    r = 1./(((abs(1./a.*cos(m./4.*phi))).^n2 + (abs(1./b.*sin(m./4.*phi)).^n3)).^(1./n1));
    phi = phi + offset;
    x = r.*cos(phi) + centerX;
    y = r.*sin(phi) + centerY;
    Output = [x' y'];
    
%     for i = 0:0.01:6.26
%         r = radius_(i);
%         x = cos(i)*r + centerX;
%         y = sin(i)*r + centerY;
%         Output(end+1,:) = [x, y];
%     end
end