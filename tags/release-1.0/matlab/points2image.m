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

for i=1:length(points)
    im(points(i,1), points(i,2)) = 1; % write values
end

end