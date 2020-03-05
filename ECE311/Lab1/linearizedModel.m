y_star = 1;
g = 9.8;
M = 1;
R = 3;
L = 1;

A = [0,1,0;
     2*g/y_star,0,-2*sqrt(g)/(sqrt(M)*y_star);
     0,0,-R/L];
B = [0;0;1];
C = [1,0,0];
D = 0;

fprintf("Eigenvalues of A");
eig(A)

[num,denom] = ss2tf(A,B,C,D);
Gs = tf(num,denom);
fprintf("Poles and zeros of G(s)");
zpk(Gs)

[sizes, x0, states] = magball;
[A_check,B_check,C_check,D_check] = linmod('magball',[1;0;sqrt(g)],3*sqrt(g))

sys = ss(A,B,C,D);
sys_2 = ss(A_check,B_check,C_check,D_check);
% figure
% impulse(sys,2);
figure
step(sys,2,'r-')
hold on
step(sys_2,2,'k.')
legend({'linearlized','linmod'},'Location','southwest')