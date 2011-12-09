function [ChiSquare, alpha, beta] = XiSquare(Data, alpha, beta, init_on, robust, functionused, Normalization, update)
%MATLAB version for function XiSquare() of C++
%   Detailed explanation goes here
    
    dj= zeros(1,8);
    dTrdx0 = [0 , 0 , -1 , 0 , 0 , 0 ,  0 , 0 , 1];
    dTrdy0 = [0 , 0 , 0 ,  0 , 0 , -1 , 0 , 0 , 1];
        
    if(init_on==true)
        if(robust==true)
%   RobustInit() is called to find center and radius 
%   of the circle to be used in the initial assumption of Gielis curve
            RobustInit(Data);
        else
            Center = [0, 0];
            for i = 1:length(Data)
                Center =Center + Data(i,:);
            end
            Center =Center/length(Data);                
            
            amin= 1e30;
            for i = 1:length(Data)
                amin= min(amin, norm((Data(i,:)-Center)));
            end
                        
            %#global Set_a(amin);
            Parameters(1) =abs(amin);
            %#global Set_b(amin);
            Parameters(2) =abs(amin);          
            %#global Set_xoffset(Center(0));  
            Parameters(10) =Center(0);
            %#golbal Set_yoffset(Center(1));
            Parameters(11) =Center(1);
        end        
    end

    %#global a =Get_a();
    a =Parameters(1); 
    %#global b =Get_b();
    b =Parameters(2);
    ChiSquare =1e15;
    f =0;
    h =0;
    df =0;
    %#global n1 =Get_n1();
    n1 =Parameters(3);
    %#global n2 =Get_n2();
    n2 =Parameters(4);
    %#global n3 =Get_n3();
    n3 =Parameters(5);
    %#global p =Get_p();
    p =Parameters(6);
    %#global q =Get_q();
    q =Parameters(7);
    %#global x0(Get_xoffset())
    x0 =Parameters(10);
    %#global y0(Get_yoffset())
    y0 =Parameters(11);
    %#global tht0(Get_thtoffset())
    tht0 =Parameters(8);
    
    if (update==true)
        alpha =zeros(8,8);
        beta =zeros(8,1);
    end
    
    ChiSquare =0;
    
    %initialization not required here
        %Tr= zero(); 
        %Rot.setZero(); 
        %dRotdtht0.setZero(); 
        
    %Assigning values directly
    %Assuming 3*3 matrices for Tr Rot... ??
    Tr =[1,0,-x0;...
         0,1,-y0;...
         0,0, 1];
     
    Rot=[cos(tht0), sin(tht0), 0;...
         -sin(tht0), cos(tht0) ,0;...
          0 ,         0 ,       1];
    
    dRotdtht0 =[-sin(tht0), cos(tht0), 0;...
                -cos(tht0), -sin(tht0), 0;...
                         0,        0 ,  1];
    
    for i =1:length(Data)
        dum = Data(i);
        dum2 = [dum(0) dum(1) 1];
        dum3 = Rot * (Tr * dum2);
        dPdx0 = ( Rot * (dTrdx0 * dum2) );
        dPdy0 = ( Rot * (dTrdy0 * dum2) );
        dPdtht0 = (dRotdtht0 * (Tr *dum2) );
        P = [dum3(0), dum3(1)];
                
        dxdx0 = dPdx0(0);
        dxdy0 = dPdy0(0);
      	dxdtht0 =  dPdtht0(0);
      	dydx0 = dPdx0(1);
      	dydy0 = dPdy0(1);
       	dydtht0 =  dPdtht0(1);
        
        x = P(0); 
        y = P(1);
        % avoid division by 0 ==> numerical stability
        if (P.norm()<EPSILON) continue; % avoids division by zero
            tht = atan2(y,x);
        end
        if (tht<0) 
            tht= tht+2*pi;
        end
        %radius() not yet implemented
        R= radius(tht);
        
        dthtdx= -sin(tht);
        dthtdy= cos(tht);
        
        %partial derivatives
        %partial derivatives of theta regarding x offset, y offset, and angular offset
        dthtdx0 = dthtdx * dxdx0 + dthtdy * dydx0;
        dthtdy0 = dthtdx * dxdy0 + dthtdy * dydy0;
        dthtdtht0 = dthtdx * dxdth0 + dthtdy * dydtht0;
        
        drdth= DrDtheta(tht);
        
        %switch statements to choose among a certain implicitfunction among 3
        switch functionused
            case 1
                ImplicitFunction1(P, Df);
            case 2
                ImplicitFunction2(P, Df);
            case 3
                ImplicitFunction3(P, Df);
        end
        
        DfDr= Df(2);
        
        dj= zeros(8,1);
        
        dj(1) = DfDr * DfDa(tht);
        
        dj(2) = DfDr * DrDb(tht);
        
        dj(3) = DfDr * DrDn1(tht);
        
        dj(4) = DfDr * DrDn2(tht);
        dj(5) = DfDr * DrDn3(tht);
        dj(6) = DfDr * drdth*dthtdx0;
        dj(7) = DfDr * drdth*dthtdy0;
        dj(8) = DrDr * drdth*dthtdtht0;
        
        
        
        nablamagn = Df(0)*Df(0) + Df(1)*Df(1);
        
        h = f*(f*f + nablamagn)^(-0.5);
        
        if(Normalization == true)
            ChiSquare = ChiSquare + h*h;
        else
            ChiSquare = ChiSquare + f*f;
        end
        
        if(update==true)
            if(Normalization ==true)
                if(abs(f)>EPSILON || nablamagn>EPSILON)
                    for idum= 1:8
                        dj(idum) = dj(idum) * h*(1-h*h)/f;
                    end
                else
                    dj = zeros(8,1);
                end
                
                %##C++ code slightly edited to remove redundant if
                
                for k = 1:8
                    if(f~=0)
                        beta(k) = beta(k)- h * dj(k);
                    end
                end
            else
                for k=1:8
                    beta(k)= beta(k)- f*dj(k);
                end
            end 
            
            for k = 1:8
                for j = 1:8
                    alpha(k,j) = dj(k) * dj(j);
                end
            end
        end %end if(update==true)
    end %end for i=1:length(Data)
end %end function XiSquare()
