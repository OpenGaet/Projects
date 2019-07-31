function [err] = cosinePowerSeriesError(x,n)
err = abs(cos(x) - cosinePowerSeries(x,n));
end

