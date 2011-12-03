function [Radius, Theta] = Cartisian2Polar(Contour)

%Calculate the centre of mass
[Mean_Value] = mean(Contour);
Xc = Mean_Value(1);
Yc = Mean_Value(2);

%Translation of origin to centre of mass
X = Contour(:,1) - Xc;
Y = Contour(:,2) - Yc;

%Cartisian to polar conversion
[Theta, Radius] = cart2pol(X,Y);

%Make sure that theta lies in the range of 0 to 2*pi
for i = 1: size(Theta)
    if(Theta(i) < 0)
        Theta(i) = Theta(i) + (2 * pi);
    end
end