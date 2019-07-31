function [] = plotSpringCompressionFallingWeight( W,k1,k2,d,hmin,hmax )
h = hmin:.01:hmax;
compression = calcMaxSpringCompression( W, k1, k2, d, h );
plot(compression,h);
xlabel('Compression(m)')
ylabel('Height of Falling Weight(m)')

end