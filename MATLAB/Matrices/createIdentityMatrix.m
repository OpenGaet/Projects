function [ res ] = createIdentityMatrix( nrows, mcols )

matrix = createMatrix(nrows, mcols, 0);
var = 1;

while var <= nrows && var <= mcols
  matrix(var,var) = 1;
  var = var + 1;
end

res = matrix;
end

