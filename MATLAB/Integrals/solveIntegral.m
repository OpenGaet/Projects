function res = solveIntegral( dx )
y = round(1/dx);
res = 0;
for x = 1:(1/y):2
    res = res + (sin(x)/x)*(1/y);
end
end

