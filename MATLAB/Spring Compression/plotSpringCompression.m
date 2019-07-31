function [] = plotSpringCompression( Wmin,Wmax,k1,k2,d )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
W = Wmin:1:Wmax;
compression = calcSpringCompression( W,k1,k2,d );
plot(compression,W);
xlabel('Compression(m)')
ylabel('Weight(N)')
end

