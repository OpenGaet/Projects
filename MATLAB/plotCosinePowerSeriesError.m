function  [] = plotCosinePowerSeriesError(xmin,xmax,n)
x = xmin:.01:xmax;
error = abs(cos(x) - cosinePowerSeries(x,n));
plot(x,error);

end

