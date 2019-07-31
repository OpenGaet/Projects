function  [] = drawVerticalTrusses(num_of_divisions, division_width, height)
var = 1;

for i = 0:1:num_of_divisions
x = i*division_width;
y = height/2 - ((-1)^i)*(height/2) ;
array(var)=x;
array2(var)=y;
var = var + 1;
end
plot(array,array2);
axis([-10 10+(num_of_divisions*division_width) -4*height 4*height]) 
grid on
hold on
xlabel('meters')
ylabel('meters')

var = 2 + num_of_divisions;
for i = 0:1:num_of_divisions
x = (division_width*num_of_divisions) - i*division_width;
y = height/2 - ((-1)^i)*(height/2) ;
array(var)=x;
array2(var)=height - y;
var = var + 1;
end
plot(array,array2);
hold on

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

for i = 1:1:num_of_divisions
    sarray2(i,1) = (i-1)*division_width;
    earray2(i,1) = i*division_width;
    sarray2(i,2) = height;
    earray2(i,2) = height;
end
x=[sarray2(:,1),earray2(:,1)].'; 
y=[sarray2(:,2),earray2(:,2)].';
plot(x,y,'b')
hold on

for i = 1:1:num_of_divisions+1
    sarray3(i,1) = (i-1)*division_width;
    earray3(i,1) = (i-1)*division_width;
    sarray3(i,2) = 0;
    earray3(i,2) = height;
end
x=[sarray3(:,1),earray3(:,1)].';
y=[sarray3(:,2),earray3(:,2)].';
plot(x,y,'b')
hold on

end