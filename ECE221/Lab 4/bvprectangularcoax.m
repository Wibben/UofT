function [V,Ex,Ey,C,C2,C3,We,We2,gridpointsx,gridpointsy,innerx,innery,outerx,outery] = bvprectangularcoax(a,b,c,d,xo,yo,er,Vo)
    % This function used the finite difference method to solve the two-dimensional
    % electrostatic boundary value problem related to a square coaxial cable.
    %   a = width of outer conductor
    %   b = height of outer conductor
    %   c = width of inner conductor
    %   d = height of inner conductor
    %   xo = the x-coordinate of the location of the bottom left corner of the inner conductor
    %   yo = the y-coordinate of the location of the bottom left corner of the inner conductor
    %   er = the relative permittivity of the dielectric which fills the space
    %   between the inner and outer conductor
    %   Vo = electric potential of the inner conductor (outer is grounded)

    % Define the fundamental constant eo
    eo=8.854e-12;

    % Set number of nodes and node spacings
    Nx=101;
    hx=a/(Nx-1);
    hy=hx;
    Ny=round(b/hy+1);

    % Set the initial values of V to zero
    V = zeros(Nx,Ny);

    % Set the known potential values (or boundary values)
    V(1,1:Ny)=0; % Grounded left side
    V(1:Nx,1)=0; % Grounded bottom side
    V(Nx,1:Ny)=0; % Grounded right side
    V(1:Nx,Ny)=0; % Grounded top side

    innerstartx=round(xo/hx+1);
    innerendx=round(innerstartx+c/hx);
    innerstarty=round(yo/hy+1);
    innerendy=round(innerstarty+d/hy);
    V(innerstartx:innerendx,innerstarty:innerendy)=Vo; % Set potentials of inner conductor

    % Determine the final voltage distributions iteratively
    while 1
        Vold = V(2:Nx-1,2:Ny-1);
        Vnew = Vold;
        for i = 1:Nx-2
            for j = 1:Ny-2
                % Ensure that inner condictor nodes are not changed
                if(i<innerstartx-1 || i>innerendx-1 || j<innerstarty-1 || j>innerendy-1)
                    Vnew(i,j) = 0.25 * (V(i+2,j+1)+V(i,j+1)+V(i+1,j+2)+V(i+1,j));
                end
            end
        end
        
        % Calculate max percent deviation
        maxdev = max(max(abs(100*(Vnew-Vold)./Vnew)));

        % Effectively making this a do-while loop
        if(maxdev > 1e-4) V(2:Nx-1,2:Ny-1) = Vnew;
        else break; % Exit loop
        end
    end
    
    % Evaluate E-Field components
    [Ey,Ex] = gradient(-V,hx,hy);
    
    % Calculate Capacitance per unit length (C)
    C = 1/Vo * er * eo * ( ...
        sum(V(innerstartx,innerstarty:innerendy)-V(innerstartx-1,innerstarty:innerendy))/hx * hy + ... % Left side
        sum(V(innerendx,innerstarty:innerendy)-V(innerendx+1,innerstarty:innerendy))/hx * hy + ... % Right side
        sum(V(innerstartx:innerendx,innerstarty)-V(innerstartx:innerendx,innerstarty-1))/hy * hx + ... % Bottom side
        sum(V(innerstartx:innerendx,innerendy)-V(innerstartx:innerendx,innerendy+1))/hy * hx); % Top side
    
    % Calculate capacitance per unit length using some other contours
    % A little farther out
    C2 = 1/Vo * er * eo * ( ...
        sum(V(innerstartx-5,innerstarty-5:innerendy+5)-V(innerstartx-6,innerstarty-5:innerendy+5))/hx * hy + ... % Left side
        sum(V(innerendx+5,innerstarty-5:innerendy+5)-V(innerendx+6,innerstarty-5:innerendy+5))/hx * hy + ... % Right side
        sum(V(innerstartx-5:innerendx+5,innerstarty-5)-V(innerstartx-5:innerendx+5,innerstarty-6))/hy * hx + ... % Bottom side
        sum(V(innerstartx-5:innerendx+5,innerendy+5)-V(innerstartx-5:innerendx+5,innerendy+6))/hy * hx) % Top side
    
    % Just inside the outer conductor
    C3 = 1/Vo * er * eo * ( ...
        sum(V(2,2:Ny-1))/hx * hy + ... % Left side
        sum(V(Nx-1,2:Ny-1))/hx * hy + ... % Right side
        sum(V(2:Nx-1,2))/hy * hx + ... % Bottom side
        sum(V(2:Nx-1,Ny-1))/hy * hx) % Top side
    
    % Calculate energy stored per unit length using capacitance per unit length (We)
    We = 0.5 * C * Vo^2;
    
    % Calculate energy stored per unit length using calculated E-Fields (We2)
    We2 = 0.5 * er * eo * sum(sum(Ex.^2 +Ey.^2)) * hx * hy;
    
    % Plot th node grid
    [gridpointsx,gridpointsy,innerx,innery,outerx,outery] = fdrectcoaxplotnodes(a,b,c,d,xo,yo,Nx,Ny);
    
    % Plot the potential distribution
    figure;
    meshc(gridpointsx,gridpointsy,V');
    title('Potential Distribution for Rectangular Coaxial Cable');
    xlabel('x (m)');
    ylabel('y (m)');
    zlabel('V (Volts)');
    
    figure;
    contourf(gridpointsx,gridpointsy,V');
    title('Potential Distribution Contour for Rectangular Coaxial Cable');
    xlabel('x (m)');
    ylabel('y (m)');
    
    % Plot the E-Field
    figure;
    quiver(gridpointsx,gridpointsy,Ex',Ey');
    title('E-FIeld Distribution for Rectangular Coaxial Cable');
    xlabel('x (m)');
    ylabel('y (m)');
end