function [Offset] = FindMinimum(Radius, Theta)

Count = 1;
NoOfPoints = size(Radius);
Neighbourhood = 10;
%Algorithm as suggested in Vega thesis

%Check if the minimum lies in the 1st Neighbours points. If yes then store
%it.
if(min(Radius(2:( 2 * Neighbourhood))) > Radius(1))
   Offset(Count) = Theta(1);
   Count = Count + 1;
end

for i = Neighbourhood + 1: 1: NoOfPoints - Neighbourhood
    Curr = Radius(i);
    % Store the previous Neighbours points and next Neighbours points.
    Neighbours = [Radius(i-10:i-1);Radius(i+1:i+10)];
    %Calculate the minimum of all the neighbouhood points.
    Min = min(Neighbours);
    if(Curr < Min)
        Offset(Count) = Theta(i);
        Count = Count + 1;
    end
end