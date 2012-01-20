function [ outDrDn1 ] = DrDn1( tht )
%DrDn1 Calculates derivative of radius w.r.t. n1
    %Input: tht   - The angle parameter
    %Output: out  - The derivative, i.e., a scalar value

    %#global n1 parameter used
    global EPSILON;
    global Parameters;
    n1 = Parameters(3);
    n2 = Parameters(4);
    R = radius_(tht);
    rn1 = R ^(-n1);
    
    %#global constant EPSILON used here
    if(R > EPSILON)
        outDrDn1 = log(rn1) * R / (n1*n2);
    else
        outDrDn1 = 0;
    end

end

