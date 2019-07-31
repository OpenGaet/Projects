function [x_root] = newton_raphson(x_guess,f,F, toll )
format long
err1 = 10;
err2 = 10;
x_old = x_guess;
i = 1;
while err1 >= toll && err2 >= toll
    x_new = x_old - (F\f);
    err1 = abs(x_new(1) - x_old(1))/abs(x_new(1) + x_old(1));
    err2 = abs(x_new(2) - x_old(2))/abs(x_new(2) + x_old(2));
    x_old = x_new;
    [F,f] = f_matrix(x_old);
    x_root1(i) = x_new(1);
    x_root2(i) = x_new(2);
    i = i + 1;
end
p = size(x_root1);
b = 1:p(2);
x_root = x_new;
plot(b,x_root1,b,x_root2)
legend('x1','x2')
xlabel('Iterations')
ylabel('x')
title('Newton-Raphson Method')
end