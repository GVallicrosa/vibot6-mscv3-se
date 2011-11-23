function valid = meaningful_contour_extraction(points, K, ERROR)

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
        if (abs(dist) < ERROR)
            valid = [valid; xp, yp];
        end
    end
end