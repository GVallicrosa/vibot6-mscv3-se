function valid = contour_extraction(contP, convHull, distanceError)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%         Contour extraction         %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   ---Responsibles---
%   Guillem Vallicrosa
%   Marc Barnada
%   Igor Jovancevic
%   Jorge Zavala
%   Muhammed Safiul Azam
%
%   ---Inputs---
%   contP          : structure containing contour points of each region
%                    in a image sorted CCW
%   convHull       : structure containing the indexs of the contour
%                    points that are in the convex hull
%   distanceError  : parameter to set the maximum distance to the convex
%                    hull polygon to take the points
%
%   ---Outputs---
%   valid          : structure containing all the valid points at a
%                    distance less or equal to 'distanceError' of the
%                    convex hull polygon
%
%   ---Testing---
%   TODO

valid = []; % variable to save near points
allx = points(:,1);
ally = points(:,2);
for i=1:(length(K)-1)
    % convex hull indexs
    ko = K(i);
    kf = K(i+1);
    % convex hull points
    xo = allx(ko);
    yo = ally(ko);
    xf = allx(kf);
    yf = ally(kf);
    % contour points to check
    if (kf>ko)
        xs = allx(ko:kf-1);
        ys = ally(ko:kf-1);
    else
        xs = [allx(1:kf-1) allx(ko:length(allx))];
        ys = [ally(1:kf-1) ally(ko:length(ally))];
    end
    %% Check all the points distance to line
    for j = 1:length(xs)
        % obtain point
        xp = xs(j);
        yp = ys(j);
        % obtain distance
        dist = abs((xf-xo)*(yp-yo)-(yf-yo)*(xp-xo))/sqrt((xf-xo)^2+(yf-yo)^2);
        % if dist < error, include point
        if (abs(dist) < distanceError)
            valid = [valid; xp, yp];
        end
    end
end