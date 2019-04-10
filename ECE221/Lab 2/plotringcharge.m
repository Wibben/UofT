function plotringcharge(z1,z2,a,Q)
    N = 500;
    epsilon = 8.854e-12;
    rhol = Q/(2*pi*a);
    z = linspace(z1,z2,200);
    
    % Find values using the formula
    Ez_theory = (rhol*a*z)./(2*epsilon*((z.^2+a^2).^(3/2)));
    
    % Find values using ringofcharge
    for i = 1:200
        [Etot(i),Ex(i),Ey(i),Ez(i)] = ringofcharge(0,0,z(i),a,rhol,N);
    end
    
    figure(1);
    hold on;
    plot(z,Ez_theory,'--b','DisplayName','Ez Theoretical');
    plot(z,Ez,':k','DisplayName','Ez Matlab');
    hold off;
    title('E-Field of a ring of charge along the z-axis');
    xlabel('z');
    ylabel('Ez');
    legend('show');
end