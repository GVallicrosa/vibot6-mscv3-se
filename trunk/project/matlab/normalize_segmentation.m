function [In] = normalize_segmentation(I, color)
% Converts the image to IHLS and does the segmentation
%
% INPUT
%    I      : image to process
%    color  : 'blue' or 'red'
%
% OUTPUT
%    In     : normalized and segmented image

%% Convert to IHLS and normalize hue-saturation
I2 = rgb2ihls(I);       % convert to IHLS

h = I2(:,:,1);          % take hue channel
h_norm = h*255/(2*pi);  % rearrange from 0:2*pi to 0:255
h_norm = uint8(h_norm); % convert to uint8

s = I2(:,:,3);          % take saturation channel
s_norm = uint8(s);      % convert to uint8

%% Color Segmentation
In = zeros(size(I,1), size(I,2)); % create image to store output
if strcmp(color, 'blue')
    p = find((h_norm>=134 & h_norm<163) & (s_norm>39 & s_norm<=215));
elseif strcmp(color, 'red')
    p = find(s_norm>30 & h_norm>230 | s_norm>30 & h_norm<11);
end
In(p) = 1; % write found pixels

end
