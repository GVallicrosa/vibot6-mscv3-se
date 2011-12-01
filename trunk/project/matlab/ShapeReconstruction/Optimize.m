function [ err ] = Optimize( Data , Normalization , functionused )
%OPTIMIZE2 Summary
% /* Team U Comments
% // This function implements the Levenberg-Marquardt algorithm
% // Input		: Data is the contour points
% // Output     : err the minimum of cost function (XiSquare)
% // Input		: Normalization, whether normalization is used in computing cost function
% // Input		: functionused, the number of the potential field function [0,1,2]
%   
    ChiSquare = 1e15;
    OldChiSquare = 1e15;

            %implement logging feature if needed

    STOP = false;
    oldparams = zeros(16,1);
    LAMBDA_INCR = 10;
    lambda = LAMBDA_INCR ^ (-12);
    oldbeta = zeros(8,1);

    %Matrix3d Tr,Rot, dTrdx0, dTrdy0, dRotdtht0;
    %double a, b, R, x, y, tht, dthtdx, dthtdy,dthtdx0, dthtdy0, drdth;
    %double h;

    itnum = 1;
    global Parameters;
    while itnum <= 1000 && STOP == false
        
        oldparams = Parameters;
        alpha = zeros(8,8);
        alpha2 = zeros(8,8);
        beta = zeros(8,1);
        beta2 = zeros(8,1);
        outofbounds = false;
        [ChiSquare alpha beta] = XiSquare(Data,...
                             alpha,...
                             beta,...
                             itnum == 0,...    %init x0, y0, and tht0 or not?// Team U: in the first iteration only initialize all the parameters
                             true,...          %robust or not?// Team U: It will never be false. If true this would call RobustInit() which is for
                             ...               %initial assumption circle for Gielis curve. RobustInit() is called only for itnum==0
                             functionused,...  %implicit function1  //Team U:Which potential field function is being used
                             Normalization,... %activate normalization or not
                             true);            %update vectors //Team U: if false the values of alpha and beta won't be changed in the function and
                                               %alpha n beta wud have the last values only
        for k = 1:8
		    %alpha(k,k) = alpha * ( 1. + lambda);   %multiplicative factor to make diagonal dominant   //Team U: dnt knw whether to keep this step or not
		    alpha(k,k) = alpha + lambda;    %additive factor to avoid rank deficient matrix      //Team U: same as (J^T*J + lambda*I)
        end
        
        %logfile << alpha << endl<<endl;
        %logfile << beta << endl<<endl; i 
        LSopts.SYM = true;
        LSopts.POSDEF = true;
        
        %linsolve uses the best method for given A in A*x=b. By specifying
        %properties of A in LSopts variable we direct it to chose the best
        %method. Here alpha is positive definite symmetric Hermitian
        %matrix, and so Cholesky decomposition would be chosen by
        %MATLAB...... Not sure though
        beta = linsolve(alpha, beta, LSopts);   %subsititution for solveInPlace() in C++. This solves alpha*h=beta and then we assign beta=h
        
        %chk if coefficients a and b in [0.01, 100]

        outofbounds =  Parameters(1) + beta(1) < 0.01  || Parameters(1) + beta(1) > 1000 || ...
                       Parameters(2) + beta(2) < 0.01  || Parameters(2) + beta(2) > 1000 || ...
                       Parameters(3) + beta(3) < 0.1   || Parameters(3) + beta(3) > 1000 || ...
                       Parameters(4) + beta(4) < 0.001 || Parameters(4) + beta(4) > 1000 || ...
                       Parameters(5) + beta(5) < 0.001 || Parameters(5) + beta(5) > 1000;
                       
        %logfile <<"beta apres"               <<endl<< beta <<endl;
        
        if (outofbounds~=0)                       %Team U: If parameters not out of bounds
            %Team U: Set the new parameters. Note again beta is the new h

            Parameters(1) = abs( Parameters(1)+beta(1) );
            
            Parameters(2) = abs( Parameters(2)+beta(2) );
            
            % coefficients n1 in (1., 1000)
            % setting n1<1. leads to strong numerical instabilities
            Parameters(3) = abs( Parameters(3) + beta(3) );
            
            
            % coefficients n2,n3 in (0.001, 1000)
            Parameters(4) = abs( Parameters(4) + beta(4) );
            Parameters(5) = abs( Parameters(5) + beta(5) );
            % coefficients x0 and y0 translational offset
            % truncate translation to avoid huge gaps
            % Team U: x0 and y0 lie in (-0.01,0.01) and rot offset lies in (-PI/10,PI/100)
            beta(6) = min(0.01, max(-0.01,beta(6)));
            beta(7) = min(0.01, max(-0.01,beta(7)));
            beta(8) = min(PI/100., max(-PI/100.,beta(8)));
            %Team U: Translational offset
            Parameters(10) = Parameters(10) + beta(6);
            Parameters(11) = Parameters(11) + beta(7);
            % rotational offset tht0
            Parameters(8)  = Parameters(8)  + beta(8);
            
        end
        
        OldChiSquare = ChiSquare;
        
        %Team U: calculate cost function again for the new parameters and
        %update alpha2 and beta2
        
		[NewChiSquare alpha2 beta2] = XiSquare(Data, alpha2, beta2, false, false, functionused, Normalization, false);
        
        %Same as following function call in C++         
        %         //Team U: calculate cost function again for the new parameters
        % 		 NewChiSquare=XiSquare(Data,
        %                              alpha2,
        %                              beta2,
        %                              false,    //init x0, y0, and tht0 or not? //Team U: dnt initialize again
        %                              false,          //robust or not?          //Team U: dnt call RobustInit() again
        %                              functionused,             //implicitf cuntion1
        %                              Normalization,  //actiate normalization or not
        %                              false);    //Team U: no updation of alpha beta again in the same iteration

        if(	NewChiSquare > 0.999*OldChiSquare )			% Not good result-->restore old params and try with lambda 10 times bigger
                                                        % Team U: no improvement as we need to minimize ChiSquare
		    %Iteration failed
            %logfile<<"0 "<<lambda<<" "<<*this;
			lambda = lambda * LAMBDA_INCR;
            Parameters = oldparams;                     % Team U: No improvement in cost function, then dnt change the parameters
        
        else                                            % successful iteration
		        
%             // huge improvement, something may have been wrong
%             // this may arise during the first iterations
%             // n1 may literally explode, or tend to 0...
%             // in such case, the next iteration is successful but leads to a local minimum
%             // ==> it is better to verify the result with a smaller step
%             // if indeed it was a correct iteration, then it will pass the next time
        
		    if (NewChiSquare <= 0.01*OldChiSquare)      % 99% improvement, impossible Team U: Too good to be true
		        %logfile<<"2 "<<lambda<<" "<<*this;
		        lambda = lambda * LAMBDA_INCR;          % Team U: This is also considered to be 
                                                        % a failed iteration n lambda is still increased and parameters are not changed
                Parameters = oldparams;                 % restore old parameters
            
            
            else
                %correct and realistic improvement
            
                %logfile<<"1 "<<lambda<<" "<<*this;
                lambda = lambda / LAMBDA_INCR;              %Team U: the only succesful acceptable case. Labda decreased, and the updation 
                                                            %to parameters is left as it is
            end
            
        end
        
        if( lambda > 1e15 || NewChiSquare < 1e-5)           % very small displacement ==> local convergence
        %Team U: Note that the stopping condition is different from the given condition in algorithm. NewChiSquare< 1e-15 will actually would be
        %true rarely and hence there is no point in using && instead of  || as suggested in the algo. Practically this condition works better
                
            STOP=true;
        end
        
    end                                                     %end of while
    err = ChiSquare;
    %logfile<<"1 "<<lambda<<" "<<*this;
    %logfile.close();

end                                                         %end of function Optimize2()                                                      

