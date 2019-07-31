function [ res ] = isMatrix( arg )
v = size(arg);
if v(1)>=0 
    if v(2)>=0
        if strcmp(class(arg),'double')
            res=1;
            else res=0;
        end
        else res=0;
    end
else res=0;
end

