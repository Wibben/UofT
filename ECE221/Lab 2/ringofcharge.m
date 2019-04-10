function [Etot,Ex,Ey,Ez] = ringofcharge(x,y,z,a,rhol,N)
    epsilon = 8.854e-12;
    theta = linspace(0,2*pi,N+1);
    dtheta = 2*pi/N;
    
    % R vector
    % When using linspace and cos, there tend to be an extra
    % peak generated at the end of the cos such that sum(cos(theta)) = 1
    % whereas sum(sin(theta)) ~ 0, to circumvent this an extra linspace
    % is computed and then concatenated
    theta = theta(1:N); % concatenate last bit so it won't generate an extra cos
    Rx = x-a*cos(theta);
    Ry = y-a*sin(theta);
    Rz = linspace(z,z,N);
    R = sqrt(Rx.^2 + Ry.^2 + Rz.^2);
    
    % Sometimes due to imprecision, R->0 but isn't actually 0
    % this makes R^3 immensely large, so it could be fixed by explicitly
    % setting it to 0
    idr = find(R<0.0000001);
    R(idr) = 0;
    
    % Get components of dE
    common_integrand = (rhol*a*dtheta) ./ (4*pi*epsilon*(R.^3));
    dEx = Rx .* common_integrand;
    dEy = Ry .* common_integrand;
    dEz = Rz .* common_integrand;
    
    % Compute final values by adding all the riemann terms
    Ex = sum(dEx);
    Ey = sum(dEy);
    Ez = sum(dEz);
    Etot = sqrt(Ex^2 + Ey^2 + Ez^2);
    %fprintf('E = %.4fx + %.4fy + %.4fz\nEtot = %.4f\n',Ex,Ey,Ez,Etot);
end