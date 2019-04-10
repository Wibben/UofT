function plotringcharge_non_uniform(z1,z2,a)
    N = 500;
    z = linspace(z1,z2,200);
    
    % rhol most likely a function of theta
    theta = linspace(0,2*pi,N);
    % Put in the actual function here for rhol
    rhol = 5*exp(theta-pi) * 1e-3; 
    
    % Find values using ringofcharge
    for i = 1:200
        [Etot(i),Ex(i),Ey(i),Ez(i)] = ringofcharge(0,0,z(i),a,rhol,N);
    end
    
    figure(3);
    hold on;
    plot(z,Ex,'--b','DisplayName','Ex');
    plot(z,Ey,'-.r','DisplayName','Ey');
    plot(z,Ez,':k','DisplayName','Ez');
    hold off;
    title({'E-Field components of a non-uniform','ring of charge along the z-axis'});
    xlabel('z');
    ylabel('E-Field Component Value');
    legend('show');
end