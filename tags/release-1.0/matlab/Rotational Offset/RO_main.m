clear all;
clc;

%Test Vector
for i = 1:167
    
    filename  =strcat('1 (',int2str(i),').txt');
    
    Contour = load(filename);

    plot(Contour(:,1),Contour(:,2));

    [Radius, Theta] = Cartisian2Polar(Contour);

    %Sort radius based on theta.

    %Get the permutation from theta array
    [Theta,Permutation_Index] = sort(Theta,'ascend');

    %Arrange the radius array according to the permutation
    Radius = Radius(Permutation_Index);
   % plot(Theta,Radius,'--rs','MarkerFaceColor','g');
    Offset = FindMinimum(Radius, Theta)
    Offset = uint8(FindMinimum(Radius, Theta))
    clc;
end