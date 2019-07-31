function [] =  drawPortalFrame(num_of_stories, story_height, num_of_bays, bay_width)
for p = 1:1:num_of_stories
for i = 1:1:num_of_bays,
    sarray(i,1) = (i-1)*bay_width;
    earray(i,1) = i*bay_width;
    sarray(i,2) = p*story_height;
    earray(i,2) = p*story_height;
end
x=[sarray(:,1),earray(:,1)].';
y=[sarray(:,2),earray(:,2)].';
plot(x,y,'b')
axis([-10 10+(num_of_bays*bay_width) -2*story_height*num_of_stories 2*story_height*num_of_stories]) 
grid on
xlabel('meters')
ylabel('meters')
hold on


for i = 1:1:num_of_bays+1
    sarray3(i,1) = (i-1)*bay_width;
    earray3(i,1) = (i-1)*bay_width;
    sarray3(i,2) = (p-1)*story_height;
    earray3(i,2) = p*story_height;
end
x=[sarray3(:,1),earray3(:,1)].';
y=[sarray3(:,2),earray3(:,2)].';
plot(x,y,'b')
hold on
end
end