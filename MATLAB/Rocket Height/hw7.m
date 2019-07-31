clear all;close all;clc;

name='Gaetan Ingrassia';


vec(1:10)=2;
vecSup(1:9)=-1;
vecSub(1:9)=-1;
a = diag(vec) + diag(vecSup,1) + diag(vecSub,-1);
b = [1 0 0 0 0 0 0 0 0 -1]';
p1 = inv(a)*b;


load f.mat

ftemp = repmat(f,3,1);
s = size(f,2);
p2a = reshape(ftemp,1,s*3);

ftemp2 = repmat(f,1,2);
s2 = size(ftemp2,2);
p2b = ftemp2(s-2:s2-3);

a = find(f > 1);
b = find(f < 2);
c = sort([a b]);
d = diff(c);
e = find(~d);
f2 = c(e);
p2c = f;
p2c(f2) = 0;
p2d = f(f2);


Wo = 0;
Zo = 500;
time = 0:0.01:10;
g = 9.81;
p3a = evalc('help projectile1D');

p3b = (-g/2)*(time.^2) + 500;

[p3c, p3d, p3e] = projectile1D(500, 0, 10, 1);
[p3f, p3g, p3h] = projectile1D(500, 0, 10, .01);

l=1:20:1001;

figure(1)
plot(time,p3b,'b');hold on;
plot(p3c,p3d,'*r',p3f(l),p3g(l),'ok','MarkerFaceColor','k')
legend('exact','dt = 1','dt = 0.01')
xlabel('time (s)')
ylabel('Height (m)')
title('Height of a falling ball: Data for dt = 0.01 is plotted for every other 20 points')
p3i = 'See figure 1';


[ T, Z, W ] = rocket( .0001, 120 );
p4a = evalc('help rocket');
p4b = evalc('help rocket>gravity');
p4c = evalc('help rocket>thrust');
p4d = W(end);
p4e = Z(end);
figure(2)
plot(T,W,'-r','LineWidth',2)
xlabel('time (s)')
ylabel('Vertical velocity (m/s)')
title('Rocket velocity during the 120-sec flight')
p4f = 'See figure 2';
figure(3)
plot(T,Z,'-r','LineWidth',2)
xlabel('time (s)')
ylabel('Altitude (m)')
title('Altitude during the 120-sec flight')
p4g = 'See figure 3';
