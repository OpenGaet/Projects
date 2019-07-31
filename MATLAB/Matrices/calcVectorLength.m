function [ res ] = calcVectorLength( arg )
v = size(arg);
if isVector(arg)==1
    if v(1)==1 && v(2)==3
        res=((arg(1))^2 + (arg(2))^2 + (arg(3))^2)^.5;
    elseif v(1)==3 && v(2)==1
        res=((arg(1))^2 + (arg(2))^2 + (arg(3))^2)^.5;
    else res=NaN;
    end
else res=NaN;
end
end

