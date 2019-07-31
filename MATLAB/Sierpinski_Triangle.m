function [] = createSierpinski(p1, p2, p3, n)
p = [p1;p2;p3];
currentPos = [0 0];
for k = 3:1:n+3
    [p(k+1,:), currentPos]  = deal((currentPos +  p(randi([1 3]),:))./2);
end
scatter(p(:,1),p(:,2),20,'.')
end