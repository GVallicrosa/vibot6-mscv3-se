function [ outDrDn1 ] = DrDn1( tht )

    %DrDn1 Calculates derivative of r w.r.t. n1

    %#global n1 parameter used
    n1 = Parameters(3);
    R = radius(tht);
    rn1 = R ^(-n1);
    
    %#global constant EPSILON used here
    if(R > EPSILON)
        outDrDn1 = log(rn1) * R / (n1*n2);
    else
        outDrDn1 = 0;
    end

end

