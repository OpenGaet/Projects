function [ res ] = createMatrix( nrows, mcols, value)
matrix = rand(nrows,mcols);
matrix(matrix > 0) = value;
res = matrix;
end

