function [ r, T ] = RobustInit( Data )
%ROBUSTINIT Finding the centre and radius of the circle to be used in the
% initial assumption of Gielis curve
% r= radius of the circle which is also the avg radius of polar coordinates
% of the data set
% T is the translation for the centre of the circle
% Angular Histogram is calculated for finding the average radius
% Get_p() gets the value of p whicha is the total no. of sectors in which we divide 2*pi radians.

global Parameters;
% a = Parameters(1);
% b = Parameters(2);
% pval = Parameters(6);
% xOffset = Parameters(10);
% yOffset = Parameters(11);

newdata = Data;
histavg = 0;

stop = false;

it = 1;
while it <= 30 && stop==false
    
    Hist = zeros(1, Parameters(6));                          %implement code for Get_p()
    T = zeros(1, 2);
    r = 0;
    
    for i=1:length(newdata)
        P = newdata(i,:);                           %change to P(:)
        T = T + P;
        angle = atan2(P(2), P(1));
        if angle < 0
            angle = angle + 2*pi;
        end
        
        for index = 1:length(Hist)
            if ((index-1) * (2*pi)/Parameters(6) <= angle && angle < index * (2*pi)/Parameters(6)) 
                    Hist(index) =  Hist(index)+1;
            end
        end
    end
    
    T = T / length(newdata);

    r = 0;

    for i = 1: length(newdata)
        r = r + norm(newdata(i) - T);
    end

    r = r / length(newdata);

    oldhistavg = histavg;
    histavg = mean(Hist);

    if histavg == oldhistavg 
        stop = true;
    end

    for i = 1 : length(Hist)
        %go through angular sectors

        amin = (i-1)*2*pi/Parameters(6);

        for j = 1 : (histavg - Hist(i))
            % generate a random angle in [amin, amax];
            angle = amin + pi/Parameters(6);

            flood = [(-T(1)+r*cos(angle)) (-T(2)+r*sin(angle))];
            newdata(end+1,:) = flood;
        end
    end
    
    
    Parameters(10) = T(1);
    Parameters(11) = T(2);
    Parameters(1) = r;
    Parameters(2) = r;
    
    it = it + 1;
end

