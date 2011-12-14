function [In] = normalize_segmentation(I)
%% Convert to IHLS and normalize hue-saturation

I2 = rgb2ihls(I);

h = I2(:,:,1);
h_norm = h*255/(2*pi);
h_norm = uint8(h_norm);

s = I2(:,:,3);
s_norm = uint8(s);

%% Color Segmentation - rough thresholding
% red = (s_norm(i,j)>30 && h_norm(i,j)<11) || (s_norm(i,j)>30 && h_norm(i,j)>230) 
% blue = (h_norm(i,j)>=134 && h_norm(i,j)<163) && (s_norm(i,j)>39 && s_norm(i,j)<=215) %blue = h_norm(i,j)>=140 && h_norm(i,j)<160 && s_norm(i,j)>111

In = zeros(size(I,1),size(I,2));
%% BLUE
%p = find((h_norm>=134 & h_norm<163) & (s_norm>39 & s_norm<=215));
%% RED
p = find(s_norm>30 & h_norm>230 | s_norm>30 & h_norm<11);
In(p) = 1;

