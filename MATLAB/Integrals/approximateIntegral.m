function res = approximateIntegral( dx, refinement, precision )
y = round(1/dx);
first = 0;
for x = 1:(1/y):2
    first = first + (sin(x)/x)*(1/y);
end
prevRes = first;
res = 0;
pos = 1;

while abs(res-prevRes)>= 10^(-1*precision)
    dx=refinement*dx;
    res = 0;
    y = round(1/dx);
    for x = 1:(1/y):2
    prevRes = res;
    res = res + (sin(x)/x)*(1/y);
    end
    fprintf('%0.8f %0.8f \n',dx,res)
    array(pos,2)= res;
    array(pos,1)=dx;
    pos = pos + 1;
end
res=array;
