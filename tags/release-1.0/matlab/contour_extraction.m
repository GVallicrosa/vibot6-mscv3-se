function valid = contour_extraction(contP, distanceError)
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
%   contP          : contour points of the region of an image sorted CCW
%   distanceError  : parameter to set the maximum distance to the convex
%                    hull polygon to take the points
%
%   ---Outputs---
%   valid          : valid points at a distance less or equal to
%                    'distanceError' of the convex hull polygon
%
%   ---Testing---
%   TODO

%% Convex hull
% Obtaining the points in the segmented region that correspond to the CH
convHull = convhull(contP(:,1),contP(:,2)); % indexs of points in convex hull, in current contour

%% Data for the region
valid = [];          % variable to save near points
allx = contP(:,1);   % x positions for all points
ally = contP(:,2);   % y positions for all points
    
for i=1:(length(convHull)-1)
    %% Segment of convex hull polygon to check
    % convex hull indexs
    ko = convHull(i);
    kf = convHull(i+1);
    % convex hull points
    xo = allx(ko);
    yo = ally(ko);
    xf = allx(kf);
    yf = ally(kf);

    %% Contour points to check
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