function [ out ] = Radius_( angle )
%RADIUS_ Summary of this function goes here
%   Detailed explanation goes here
global Parameters;

a = Parameters(1);
b = Parameters(2);

n1 = Parameters(3);
n2 = Parameters(4);
n3 = Parameters(5);

p = Parameters(6);
q = Parameters(7);

tmp_angle = p * angle * 0.25 / q;

tmp1 = cos(tmp_angle);
tmp2 = sin(tmp_angle);

tmp1 = (abs(tmp1)^n2) / a;
tmp2 = (abs(tmp2)^n3) / b;

if tmp1 + tmp2 ~=0
    out = (tmp1 + tmp2)^ (-1.0/n1);
else
    out = 0;
end
end

