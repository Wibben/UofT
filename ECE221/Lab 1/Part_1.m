x = [-1:0.05:1];
y = [-2:0.05:2];

[nr, nc] = size(x);
fprintf("The dimensions for x is %.0f rows x %.0f columns.\n", nr, nc);
fprintf("The number of columns given by length(x) is: %.0f\n", length(x));

[X, Y] = meshgrid(x,y);
fxy = 1./(1+(X.^2+Y.^2).^0.5);
%{ 
X and Y both ate 81 x 41
Number of rows = number of x positions (length x)
Number of cols = number of y positions (length y)
For X each column is the same
For Y each row is the same
%}

% 3-D plot
figure(1);
mesh(X,Y,fxy);
title('The function f_{xy}');
xlabel('x axis');
ylabel('y axis');
zlabel('f_{xy}');

% 2-D plot
[r1, c1] = find(y==-2); % y = -2
[r2, c2] = find(y==0); % y = 0
[r3, c3] = find(y==1.6); % y = 1.6

figure(2);
hold on;
plot(x,fxy(c1,:),'DisplayName','y = -2m');
plot(x,fxy(c2,:),'DisplayName','y = 0m');
plot(x,fxy(c3,:),'DisplayName','y = 1.6m');
hold off;

title('f_{xy} for specific values of y');
xlabel('x axis');
ylabel('f_{xy}');
legend;
grid on;
