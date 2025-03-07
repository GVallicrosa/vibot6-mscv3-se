function [potential_field_value, Dffinal] = ImplicitFunction2_(P)
%IMPLICITFUNCTION2_ This function computes the potential field.
%It also calculates the derivatives of the field.
%The input/output specifications are:
%1.  Input  : P, Single Data Point after translation/rotation
%2.  Output : Dffinal, Partial derivatives of potential field
%3.  Output : return, the potential field value for one point
global Parameters;

q = Parameters(7);

Dffinal = zeros(1, 3);

% if the point is zero
if P(1) == 0 && P(2) == 0
    % Df/Dx, Df/Dy, Df/Dr set to zero. We don't need to zero its elements
    % because it was done on line 9, so we'll just get out of this
    % function.
    %for i = 1:3
    %    Dffinal(i) = 0;
    %end;
    return;
end;

% dimension of f = q, because later on there will be a loop
% which assign values to f by q times.
f = zeros(1, q);
Ddum = 0;

x = P(1);
y = P(2);
PL = norm(P);
PSL = PL^2;
dthetadx = -y/PSL;
dthetady = x/PSL;

% assert angular values between [0, 2q*Pi]
theta = atan2(y, x);
thetabase = theta;

if theta < 0
    theta = theta + 2*pi;
end

Df = zeros(q, 3); % Workaround for the following loop

%% compute all intersections and associated partial derivatives
for i = 1 : q
    theta = thetabase + (i-1)*2*pi;

    R = radius_(theta);
    
    f(i) = (R - PL);  % original code: f.push_back(R - PL);

    % store partial derivatives
    drdtheta = DrDtheta_(theta);
    Df(i, 1) = -( x*R/PL - drdtheta*dthetadx*PL )/(R*R) ;    % df/dx
    Df(i, 2) = -( y*R/PL - drdtheta*dthetady*PL )/(R*R) ;    % df/dy
    Df(i, 3) = PL/(R*R);                       % df/dr
end

%% bubble sort, not really efficient but acceptable for such small arrays
for i = 1:q-1
    for j= (i+1) : q
        if f(i) < f(j)
            % swap values of f[i] and f[j]
            temp = f(i);
            f(i) = f(j);
            f(j) = temp;
            % swap(f(i),f(j));
        	% swap rows Df[i] and Df[j]
            temp = Df(i,:);
            Df(i,:) = Df(j,:);
            Df(j,:) = temp;
        end
    end
end

%% Compute resulting Rfunction

% Df1: vector for df/dxi

%iterative evaluation of:
%      -the resulting R-functions
%      -the associated partial derivatives

% f1,fdum declared here in C++

f1 = f(1);      % first value of f
Df1 = Df(1,:);  % first associated row with partial derivatives

% combine functions as (...((F1 v F2) v F3 ) v F4) v ...)

for i = 2:q % for all intersections
    % compute R-function, sets all partial derivatives
    % fdum and Ddum temporary results of the union from F1 to Fi

    [fdum, Ddum] = RpUnion_(f1, f(i), Df1, Df(i));

    % update results in f1 and Df1, and iterate
    f1 = fdum;
    Df1 = Ddum;
end

%% final partial derivatives df/dxi after R-functions
Dffinal = Df1;

potential_field_value = f1;
end

