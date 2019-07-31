function compression =  calcMaxSpringCompression(W, k1, k2, d, h)
n = numel(h);
for c = 1:1:n
    r = h(c);
    xi=(W + sqrt(W^2 + 2*k1*W*r))/k1;
    if xi < d
        compression(1,c) = (W + sqrt(W^2 + 2*k1*W*r))/k1;
    elseif xi >= d 
        compression(1,c)=((4*k2*d + 2*W)+sqrt((4*k2*d + 2*W)^2 - 4*(k1 + 2*k2)*(2*k2*(d^2) - 2*W*r)))/(2*(k1 + 2*k2));
    end
end
end