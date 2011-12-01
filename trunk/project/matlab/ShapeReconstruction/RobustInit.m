function [ r, T ] = RobustInit( Data )
%ROBUSTINIT Finding the centre and radius of the circle to be used in the
% initial assumption of Gielis curve
% r= radius of the circle which is also the avg radius of polar coordinates
% of the data set
% T is the translation for the centre of the circle
% Angular Histogram is calculated for finding the average radius
% Get_p() gets the value of p whicha is the total no. of sectors in which we divide 2*pi radians.

newdata = Data;

stop = false;

it = 1;
while it <=30 && stop==false
    
    Hist = zeros(Get_p());                          %implement code for Get_p()
    T = zeros(2);
    r = 0;
    
    for i=1:size(newdata)
        P = newdata(i,:);                           %change to P(:)
        T = T + P;
        angle = atan2(P(2), P(1));
        if angle < 0
            angle = angle + 2*pi;
        end
        
        for index = 1:size(Hist)
            if index * (2*pi)/Get_p() <= angle && angle < (index+1) * (2*pi)/Get_p() 
                    Hist(index) =  Hist(index)+1;
            end
        end
        
        T = T / size(newdata);
        
        for j = 1: size(newdata)
            r = r + norm(newdata(j) - T);
            r = r / size(newdata) ;
        end
        
        histavg=0;
        oldhistavg = histavg;
        histavg = mean(Hist);
        
        if histavg == oldhistavg 
            stop = true;
        end
        
        for j = 1 : size(Hist)
            %go through angular sectors

            amin = i*2*pi/Get_p();

            for k = 1 : (histavg - Hist(j))
                % generate a random angle in [amin, amax];
                angle = amin + pi/Get_p();
                
                flood = [(-T(1)+r*cos(angle)) (-T(2)+r*sin(angle))];
                newdata = [newdata; flood];
            end
        end
    end
    
    Set_xoffset(T[0]);
    Set_yoffset(T[1]);
    Set_a(r);
    Set_b(r);
    
end

