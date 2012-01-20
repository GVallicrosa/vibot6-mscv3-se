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

out = (-1/n1) * ( ((C^n2) / a + (S^n3) / b) ^ (-1/n1 - 1)) * ...
    (-0.25*p*C^n2*(n2*tan(p*theta*0.25/q)) / (a*q) + ...
      0.25*p*S^n3*(n3/tan(p*theta*0.25/q)) / (b*q) );

end

