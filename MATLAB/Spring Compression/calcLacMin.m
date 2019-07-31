function  lacmin = calcLacMin(D, Lab, W, Tmax)
theta=1;
Lac = sqrt(D^2 + Lab^2 - 2*D*Lab*cos(theta));
phi= asin((Lab*sin(theta))/Lac);
Matrix = [-cos(theta) cos(phi);sin(theta) sin(phi)];
C = [0;W];
T = (inv(Matrix))*C;
Tab = T(1);
Tac = T(2);
while (Tab > Tmax || Tac > Tmax)
    theta = theta + 1;
end
lacmin=Lac;
end

