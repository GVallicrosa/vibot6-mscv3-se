function [out] = DrDtheta_(theta)
%DRDTHETA_ Summary of this function goes here
%   Detailed explanation goes here
    n1 = Get_n1();
    n2 = Get_n2();
    n3 = Get_n3();
    m = Get_p();
    q = Get_q();
    a = Get_a();
    b = Get_b();

    c = cos(m*0.25*theta / q);
    C = abs(c);
    s = sin(m*0.25*theta / q);
    S = abs(s);

    if C<EPSILON || S<EPSILON
        out = 0;
        return;
    end

    out = (-1/n1) * ( ((C^n2) / a + (S^n3) / b) ^ (-1/n1 - 1)) * ...
        (-0.25*m*C^n2*(n2*tan(m*theta*0.25/q)) / (a*q) + ...
          0.25*m*S^n3*(n3/tan(m*theta*0.25/q)) / (b*q) );

end

