function [out] = DrDtheta_(theta)
%DrDtheta Calculates derivative of radius w.r.t. theta
    %Input: theta   - The angle parameter
    %Output: out    - The derivative, i.e., a scalar value

global EPSILON;
global Parameters;

a = Parameters(1);
b = Parameters(2);

n1 = Parameters(3);
n2 = Parameters(4);
n3 = Parameters(5);

p = Parameters(6);
q = Parameters(7);

c = cos(p*0.25*theta / q);
C = abs(c);
s = sin(p*0.25*theta / q);
S = abs(s);

if C<EPSILON || S<EPSILON
    out = 0;
    return;
end

Line1 = -1/n1;
Line2 = ( (C/a)^n2 + (S/b)^n3 ) ^ (-1/n1 - 1);
Line3 = ( -n2 * (C/a)^n2 * p*0.25*q*tan(p*0.25*theta / q) + ...
    n3*(S/b)^n3 * p/(4*q*tan(p*0.25*theta / q)) );
out = Line1 * Line2 * Line3;

end

