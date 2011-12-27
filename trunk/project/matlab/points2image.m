function im = points2image(points, m, n)
% Function to convert pixel coordinates to a BW image
%
% ---Input---
%    points : xy coordinates in rows for each pixel 
%    m, n   : image size
%
% ---Output---
%    im : output image

im = zeros(m,n); % create image

for i=1:length(valid)
    im(valid_contour(k,1), valid(k,2)) = 1; % write values
end

end