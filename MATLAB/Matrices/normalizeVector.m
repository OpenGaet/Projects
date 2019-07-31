function [ res ] = normalizeVector(arg)
v = size(arg);
if isVector(arg)==1
    if v(1)==1 && v(2)==3
        mag=((arg(1))^2 + (arg(2))^2 + (arg(3))^2)^.5;
        res(1)= arg(1)/mag;
        res(2)= arg(2)/mag;
        res(3)= arg(3)/mag;
    elseif v(1)==3 && v(2)==1
        mag=((arg(1))^2 + (arg(2))^2 + (arg(3))^2)^.5;
        res(1)= arg(1)/mag;
        res(2)= arg(2)/mag;
        res(3)= arg(3)/mag;
        res=res';
    else res=NaN;
    end
else res=NaN;
end
end

