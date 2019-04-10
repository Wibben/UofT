% Put the initial values here
a = 1;
b = 0.5;
c = 0.2;
d = 0.1;
xo = 0.2;
yo = 0.2;
er = 1;
Vo = 5;

% Set up to compute and display the required values
[V,Ex,Ey,C,C2,C3,We,We2,gridpointsx,gridpointsy,innerx,innery,outerx,outery] = bvprectangularcoax(a,b,c,d,xo,yo,er,Vo);

fprintf("The capacitance per unit length is: %e\n",C);
fprintf("The capacitance per unit length calculated a little farther out is: %e\n",C2);
fprintf("The capacitance per unit length calculated just inside the outer conductor is: %e\n",C3);
fprintf("The energy per unit length using capacitance is: %e\n",We);
fprintf("The energy per unit length using E-Fields is: %e\n",We2);
