function  [approx] = cosinePowerSeries(x,n)
approx=0;
for k = 0:1:n
    approx = approx + ((-1)^k)*(x.^(2*k))./(factorial( 2*k ));
end
end