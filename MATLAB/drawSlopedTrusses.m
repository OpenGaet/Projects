function [] = drawSlopedTrusses(num_of_divisions,division_width,height)
var = 1;
for i = 0:1:num_of_divisions*2
x = i*division_width/2;
y = height/2 - ((-1)^i)*(height/2) ;
array(var)=x;
array2(var)=y;
var=var+1;
end
plot(array,array2);
axis([-10 10+(num_of_divisions*division_width) -2*height 2*height]) 
grid on
hold on
xlabel('meters')
ylabel('meters')
for i = 1:1:num_of_divisions
    sarray(i,1) = (i-1)*division_width;
    earray(i,1) = i*division_width;
    sarray(i,2) = 0;
    earray(i,2) = 0;
end
x=[sarray(:,1),earray(:,1)].';
y=[sarray(:,2),earray(:,2)].';
plot(x,y,'b')
hold on

for i = 1:1:num_of_divisions - 1
    sarray2(i,1) = i*division_width - (division_width/2);
    earray2(i,1) = i*division_width + (division_width/2);
    sarray2(i,2) = height;
    earray2(i,2) = height;
end
x=[sarray2(:,1),earray2(:,1)].'; 
y=[sarray2(:,2),earray2(:,2)].';
plot(x,y,'b')
hold on

end