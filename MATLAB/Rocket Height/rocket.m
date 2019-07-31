function [ T, Z, W ] = rocket( dt, Tf )
%I used subfunctions thrust and gravity to determine those traits at
%a specific time and height. I then looped Euler's method over time
%to determine the height and vertical velocity at each point in time.
time_r = 0:dt:Tf;
m = 10;
W(1) = 0;
Z(1) = 0;
    function[Th] = thrust(t)
        %I used if and elseif statements to determine the thrust at
        %the particular time input.
        if t >= 0 && t < 2
            Th = 675;
        elseif t >= 2 && t < 4
            Th = 1362;
        elseif t >= 4
            Th = 0;
        end
    end

    function[g] = gravity(z)
        %I set the gravity according to the given equation and used if and
        %else to determine what g should be used at that height.
        if z >= 0 && z < 10000
            g = 9.81*(1 - (z/10000)^3);
        else
            g = 0;
        end
    end

for i = 1:size(time_r,2)-1
    T(i+1) = dt*i;
    Th = thrust(T(i));
    Z(i+1) = Z(i) + W(i)*dt;
    g = gravity(Z(i));
    W(i+1) = W(i) + (-g + Th/m)*dt;   
end
end