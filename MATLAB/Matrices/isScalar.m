function [ res ] = isScalar( input_args )
if strcmp(class(input_args),'double')
    if size(input_args) == [1 1]
        res=1;
        else res=0;
    end
else res=0;

end

