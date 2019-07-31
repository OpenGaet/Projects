clc;clear all;close all;format long;
x_guess = [10;10];
toll = 10^-5;
[ F,f ] = f_matrix( x_guess );
x_root = newton_raphson(x_guess,f,F, toll );