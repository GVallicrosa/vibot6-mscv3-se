function [ f, Df ] = RpUnion_( f1, f2, Df1, Df2 )
%RPUNION_ Summary of this function goes here
%   Detailed explanation goes here
Df = zeros(length(Df1), 1);

f = f1+f2+sqrt(f1*f1+f2*f2);

if f1 ~= 0 || f2 ~= 0  % function differentiable
    for i = 1:length(Df1)
        Df(i) = ( Df1(i) + Df2(i) +  (f1*Df1(i)+f2*Df2(i))/sqrt(f1*f1+f2*f2));
    end
% else function not differentiable, set everything to zero
end    
    
end

