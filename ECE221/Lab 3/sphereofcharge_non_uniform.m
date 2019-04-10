function [Etot,Ex,Ey,Ez,V] = sphereofcharge_non_uniform(x,y,z,a,rhos,N)
    epsilon = 8.854e-12;
    
    % Setting up integrals
    dtheta = 2*pi/N;
    dphi = pi/N;
    theta = linspace(0,2*pi-dtheta,N);
    phi = linspace(0,pi,N+1);
    Ex = 0; Ey = 0; Ez = 0; V = 0;
    
    % Calculate components of E and V
    % Outer integral (over theta)
    for i = 1:length(theta)
        % rhos passed in will be a vector of size N x N
        common_integrand = (rhos.*a^2.*sin(phi)*dphi*dtheta) / (4*pi*epsilon);
        
        % R vector
        Rx = x-a*sin(phi)*cos(theta(i));
        Ry = y-a*sin(phi)*sin(theta(i));
        Rz = z-a*cos(phi);
        R = sqrt(Rx.^2 + Ry.^2 + Rz.^2);
        
        % Make values of R that are already extremely close to 0 0
        R(R<0.000001) = 0;
        
        % Calculate differential vector for inner integral (over phi)
        dEx = (Rx .* common_integrand)./(R.^3);
        dEy = (Ry .* common_integrand)./(R.^3);
        dEz = (Rz .* common_integrand)./(R.^3);
        dV = common_integrand ./ R;
        
        % Add to total components of E and V
        Ex = Ex + sum(dEx);
        Ey = Ey + sum(dEy);
        Ez = Ez + sum(dEz);
        V = V + sum(dV);
    end
    
    % Compute final E
    Etot = sqrt(Ex^2 + Ey^2 + Ez^2);
    %fprintf('E = %.4fx + %.4fy + %.4fz\nEtot = %.4f\n',Ex,Ey,Ez,Etot);
end