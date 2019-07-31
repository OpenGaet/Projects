function  [] = plotTensionForces(D, Lab, W, Tmax)
Lac = calcLacMin(D,Lab,W,Tmax):.01:6.7;
Leng = length(Lac);
Tab = zeros(1,Leng);
Tac = zeros(1,Leng);
for i = 1:Leng
    theta = acos((D^2 + Lab^2 - Lac(i)^2)/(2*D*Lab));
    phi = asin(Lab*sin(theta))./Lac(i));
    Matrix = [-cos(theta) cos(phi);sin(theta) sin(phi)];
    B = [0:W];
    T = (inv(Matrix))*B;
    Tab(1,i) = T(1);
    Tac(1,i) = T(2);

end

figure
plot(Lac,Tab)
title('Tab Versus Lac')
ylabel('Tab (lb)')
xlabel('Lac (ft)')
end