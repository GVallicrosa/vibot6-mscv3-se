function [ out ] = radius_( angle )
%RADIUS_ Summary of this function goes here
%   Detailed explanation goes here
tmp_angle = Get_p() * angle * 0.25 / Get_q();

tmp1 = cos(tmp_angle);
tmp2 = sin(tmp_angle);

tmp1 = (abs(tmp1)^Get_n2()) / Get_a();
tmp2 = (abs(tmp2)^Get_n3()) / Get_b();

if tmp1 + tmp2 ~=0
    out = (tmp1 + tmp2)^ (-1.0/Get_n1());
else
    out = 0;
end
end

