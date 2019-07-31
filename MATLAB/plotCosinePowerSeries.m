function  [] = plotCosinePowerSeries(xmin,xmax,n)
x = xmin:.01:xmax;
approx=0;
for k = 0:1:n
    approx = approx + ((-1)^k)*(x.^(2*k))./(factorial( 2*k ));
end
plot(x,approx);
xlabel('x')
ylabel('Approx of cos(x)')
end

