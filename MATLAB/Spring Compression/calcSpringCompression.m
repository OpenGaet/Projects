function  [compression] = calcSpringCompression( W,k1,k2,d )
n = numel(W);
for c = 1:1:n
    r = W(c);
    xi=r/k1;
    if xi < d
        compression(1,c) = r/(k1);
    elseif xi >= d 
        compression(1,c)=(r + 2*k2*d)/(k1 + 2*k2);
    end
end
end