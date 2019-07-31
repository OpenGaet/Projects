function [ T, Z, W ] = projectile1D( Zo, Wo, Tf, dt )
%Firt I set the initial height, velocity, and time vector using the inputs.
%Then I looped Euler's method over the time.
W(1) = Wo;
Z(1) = Zo;
time_p = 0:dt:Tf;
g = 9.81;
for i = 1:(size(time_p,2))-1
    Z(i+1) = Z(i) + W(i)*dt;
    W(i+1) = W(i) - g*dt;
    T(i+1) = dt*i;
end

