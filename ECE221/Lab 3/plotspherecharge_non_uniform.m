function plotspherecharge_non_uniform(z1,z2,a)
    N = 200;
    z = linspace(z1,z2,100);
    
    % Pre-allocating arrays for speed
    Etot = zeros(1,length(z));
    Ex = zeros(1,length(z));
    Ey = zeros(1,length(z));
    Ez = zeros(1,length(z));
    V = zeros(1,length(z));
    
    % rhos is a function of phi
    phi = linspace(0,pi,N+1);
    % Implement the function here
    rhos = 3*cos(phi);
    
    % Find values using sphereofcharge_non_uniform
    for i = 1:length(z)
        [Etot(i),Ex(i),Ey(i),Ez(i),V(i)] = sphereofcharge_non_uniform(0,0,z(i),a,rhos,N);
    end
    
    figure(1);
    hold on;
    plot(z,Ex,'r-.','DisplayName','Ex');
    plot(z,Ey,'g-.','DisplayName','Ey');
    plot(z,Ez,'b-.','DisplayName','Ez');
    plot(z,Etot,'m-','DisplayName','Etot');
    hold off;
    title('E-Field of a sphere of charge along the z-axis');
    xlabel('z');
    ylabel('E-Field');
    grid on;
    legend('show');
    
    figure(2);
    hold on;
    plot(z,V,'b-','DisplayName','V');
    hold off;
    title('Potential from a sphere of charge along the z-axis');
    xlabel('z');
    ylabel('Potential');
    legend('show');
    grid on;
end