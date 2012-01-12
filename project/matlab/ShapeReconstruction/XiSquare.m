function [ChiSquare, alpha, beta] = XiSquare(Data, alpha, beta, init_on, robust, functionused, Normalization, update)
% This function computes the cost function
%  Input:           Data is the contour points
%  Input/Output:    alpha is hessian matrix : JtJ
%  Input/Output     beta is the -Jt
%  Input            init_on the initialization flag
%  Input            robust, using robustInit or not
%  Input            functionused, number of potential field function
%  Input            update, whether tha value of alpha and beta are updated
    
    global Parameters;
    global EPSILON;
    
    % dj= zeros(8,1); uncomment this in case of problems
    dTrdx0 = [0 , 0 , -1 ; 0 , 0 , 0 ;  0 , 0 , 1];
    dTrdy0 = [0 , 0 , 0 ;  0 , 0 , -1 ; 0 , 0 , 1];
    
    %Df = zeros(0,0);
    
    if(init_on==true)
        % RobustInit() is called to find center and radius 
        % of the circle to be used in the initial assumption of Gielis curve
        if(robust==true)
            RobustInit(Data);
        else
            Center = [0, 0];
            for i = 1:length(Data)
                Center = Center + Data(i,:);
            end
            
            Center = Center/length(Data);                
            
            amin = 1e30;
            
            for i = 1:length(Data)
                amin = min(amin, norm((Data(i,:)-Center)));
            end
                        
            %#global Set_a(amin);
            Parameters(1) =abs(amin);
            %#global Set_b(amin);
            Parameters(2) =abs(amin);          
            %#global Set_xoffset(Center(0));  
            Parameters(10) =Center(1);
            %#golbal Set_yoffset(Center(1));
            Parameters(11) =Center(2);
        end        
    end
    
    tht0 = Parameters(8);
    x0 = Parameters(10);
    y0 = Parameters(11);
    
    % Clean memory
    if (update==true)
        alpha = zeros(5,5);
        beta = zeros(5,1);
    end
    
    % Evaluate ChiSquare components for matrices alpha & beta
    ChiSquare = 0;
    
    % initialization not required here
        %Tr= zero(); 
        %Rot.setZero(); 
        %dRotdtht0.setZero(); 
        
    % Assigning values directly
    % Assuming 3*3 matrices for Tr Rot... ??
    Tr = [1, 0, -x0;
          0, 1, -y0;
          0, 0,  1];
    
    % get partial derivatives
    % then define inverse rotation,
    % -i.e. transposed rotation
    Rot= [cos(tht0), sin(tht0), 0;
         -sin(tht0), cos(tht0), 0;
          0,                 0, 1];
    
    % get partial derivatives
    dRotdtht0 = [-sin(tht0),  cos(tht0), 0;
                 -cos(tht0), -sin(tht0), 0;
                          0,          0, 1];
    
    for i = 1:length(Data)
        % global inverse transform is T * R
        dum = Data(i,:);
        dum2 = [dum(1); dum(2); 1];
        % apply inverse transform
        dum3 = Rot * (Tr * dum2);
        % get partial derivatives for canonical point
        dPdx0 = ( Rot * (dTrdx0 * dum2) );
        dPdy0 = ( Rot * (dTrdy0 * dum2) );
        dPdtht0 = (dRotdtht0 * (Tr * dum2) );
        % the canonical point
        P = [dum3(1), dum3(2)];

        %simplify notations
        dxdx0 = dPdx0(1);
        dxdy0 = dPdy0(1);
      	dxdtht0 =  dPdtht0(1);
      	
        dydx0 = dPdx0(2);
      	dydy0 = dPdy0(2);
       	dydtht0 =  dPdtht0(2);
        
        x = P(1); 
        y = P(2);
        
        % avoid division by 0 ==> numerical stability
        if (norm(P)<EPSILON)
            continue; % avoids division by zero
        end
        
        tht = atan2(y,x);
        if (tht<0) 
            tht = tht + 2*pi;
        end
        
        R = radius_(tht);
        
        % theta = arctan(y/x)
        % -y / (x*x+y*y)
        dthtdx = -sin(tht);
        dthtdy = cos(tht);
        
        % partial derivatives of theta regarding x offset, y offset,
        % and angular offset
        dthtdx0 = dthtdx * dxdx0 + dthtdy * dydx0;
        dthtdy0 = dthtdx * dxdy0 + dthtdy * dydy0;
        dthtdtht0 = dthtdx * dxdtht0 + dthtdy * dydtht0;
        
        % avoid non-differentiable cases
        drdth = DrDtheta_(tht);
        
        %switch statements to choose among a certain implicitfunction among 3
        switch functionused
            case 1
                [f, Df] = ImplicitFunction1_(P);
            case 2
                [f, Df] = ImplicitFunction2_(P);
            case 3
                [f, Df] = ImplicitFunction3_(P);
        end
        
        % compute elements beta[i][0] and alpha[i][j]
        % ==> requires partial derivatives!!
        % dF/dR stored at index 3 in array Df
        % -during the call to ImplicitFunction1-2-3
        %dj is the Jacobian transpose J^T. As the cost function is being
        %optimized wrt to 5 variables only a,b,n1,n2,n3, the dimension of
        %dj=5 and that of alpha=5*5
        DfDr= Df(3);
        
        dj = zeros(5, 1);
        dj(1) = DfDr * DrDa(tht);
        dj(2) = DfDr * DrDb(tht);
        dj(3) = DfDr * DrDn1(tht);
        dj(4) = DfDr * DrDn2(tht);
        dj(5) = DfDr * DrDn3(tht);
        %dj(6) = DfDr * drdth*dthtdx0;
        %dj(7) = DfDr * drdth*dthtdy0;
        %dj(8) = DfDr * drdth*dthtdtht0;
        
        %df/dx + df/dy
        nablamagn = Df(1)*Df(1) + Df(2)*Df(2);
        
        h = f*(f*f + nablamagn)^(-0.5);
        
        if(Normalization == true)
            ChiSquare = ChiSquare + h*h;
        else
            ChiSquare = ChiSquare + f*f;
        end
        
        % scale factor for the gradient
        % if normalizing the function
        if(update == true)
            if(Normalization == true)
                if(abs(f)>EPSILON || nablamagn>EPSILON)
                    for idum= 1:5
                        dj(idum) = dj(idum) * h*(1-h*h)/f;
                    end
                else
                    dj = zeros(5, 1);
                end
                
                %##C++ code slightly edited to remove redundant if
                
                if(f~=0)
                    for k = 1:5
                        beta(k) = beta(k) - h * dj(k);
                    end
                end
            else
                for k = 1:5
                    beta(k) = beta(k) - f*dj(k);
                end
            end
            
            % compute approximation of Hessian matrix
            for k = 1:5
                for j = 1:5
                    alpha(k,j) = alpha(k,j) + dj(k) * dj(j);
                end
            end
        end %end if(update==true)
    end %end for i=1:length(Data)
    alpha(isnan(alpha))=0;
    beta(isnan(alpha))=0;
    Parameters(8) = tht0;
    Parameters(10) = x0;
    Parameters(11) = y0;
end %end function XiSquare()
