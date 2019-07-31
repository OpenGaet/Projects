function [ res ] = reverseVector( arg )
v = size(arg);
if v(1)==1 
    if v(2)>=0
        if strcmp(class(arg),'double')
            res=fliplr(arg);
            else res=NaN;
        end
        else res=NaN;
    end
elseif v(1)>=0
    if v(2)== 1
        if strcmp(class(arg),'double')
            res=flipud(arg);
            else res=NaN;
        end
        else res=NaN;
    end
else res=NaN;
end
end