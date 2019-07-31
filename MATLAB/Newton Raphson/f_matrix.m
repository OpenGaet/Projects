function [F,f] = f_matrix(x)
%
f(1)=x(1)^2 + x(2)^2-1.0; %function1
f(2)= x(2) - x(1)^2; %function2
f=f';
F(1,1)=2.0*x(1); %derivative of func1 wrt x1
F(1,2)=2.0*x(2); %derivative of func1 wrt x2
F(2,1)= -2.0*x(1); %derivative of func2 wrt x1
F(2,2)=1.0; %derivative of func2 wrt x2
end