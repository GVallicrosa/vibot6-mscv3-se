function [ outDrDb ] = DrDb( tht )
%DrDb Calculates derivative of radius w.r.t. b
    %Input: tht   - The angle parameter
    %Output: out  - The derivative, i.e., a scalar value

    %#global variables used here to store parameters
    global Parameters;
    p = Parameters(6);
    q = Parameters(7);
    n1 = Parameters(3);
    n2 = Parameters(4);
    n3 = Parameters(5);
    a = Parameters(1);
    b = Parameters(2);

    C = abs(cos(p*tht*0.25/q));
    S = abs(sin(p*tht*0.25/q));
    aCn2 = (C ^ n2) / a;
    bSn3 = (S ^ n3) / b;

    %new variable name used here to store returning value
    outDrDb = bSn3 * ( (aCn2 + bSn3) ^ (-1/n1 - 1) )/ (n1 * b);
end

