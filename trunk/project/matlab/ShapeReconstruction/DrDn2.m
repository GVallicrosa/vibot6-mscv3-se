function [ outDrDn2 ] = DrDn2( tht )

    %DrDn2 Calculates derivative of r w.r.t. n2

    %#global variables used here to store parameters
    p = Parameters(6);
    q = Parameters(7);
    n2 = Parameters(4);
    n3 = Parameters(5);
    a = Parameters(1);
    b = Parameters(2);

    C = abs(cos(p*tht*0.25/q));
    S = abs(sin(p*tht*0.25/q));
    aCn2 = (C ^ n2) / a;
    bSn3 = (S ^ n3) / b;
    
    if(C < EPSILON)
        outDrDn2 = 0;
    else
        outDrDn2 = -((aCn2 + bSn3)^(-1/n1 - 1)) * log(C) * aCn2/n1;
    end
    
end

