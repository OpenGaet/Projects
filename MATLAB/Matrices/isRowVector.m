function [ res ] = isRowVector( input_args )
v = size(input_args);
if v(1)==1 
    if v(2)>=0
        if strcmp(class(input_args),'double')
            res=1;
            else res=0;
        end
        else res=0;
    end
    else res=0;
end

