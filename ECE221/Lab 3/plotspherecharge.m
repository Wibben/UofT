function plotspherecharge(y1,y2,a,rhos)
    N = 200;
    epsilon = 8.854e-12;
    y = linspace(y1,y2,100);
    
    % Pre-allocating arrays for speed
    E_theory = zeros(1,length(y));
    V_theory = zeros(1,length(y));
    Etot = zeros(1,length(y));
    Ex = zeros(1,length(y));
    Ey = zeros(1,length(y));
    Ez = zeros(1,length(y));
    V = zeros(1,length(y));
    
    % Calculate all of the values to graph
    for i = 1:length(y)
        % Theoretical values
        if(abs(y(i))<a) 
            E_theory(i) = 0;
            V_theory(i) = (rhos*a)/epsilon;
        else
            E_theory(i) = (rhos*a^2)/(epsilon*y(i)^2);
            V_theory(i) = (rhos*a^2)/(epsilon*abs(y(i)));
        end
        
        
        % Find values using sphereofcharge
        [Etot(i),Ex(i),Ey(i),Ez(i),V(i)] = sphereofcharge(0,y(i),0,a,rhos,N);
    end
    
    figure(1);
    hold on;
    plot(y,E_theory,'ro','DisplayName','Etot Theoretical');
    plot(y,Etot,'b-','DisplayName','Etot Matlab');
    hold off;
    title('E-Field of a sphere of charge along the y-axis');
    xlabel('y');
    ylabel('Etot');
    legend('show');
    
    figure(2);
    hold on;
    plot(y,V_theory,'ro','DisplayName','V Theoretical');
    plot(y,V,'b-','DisplayName','V Matlab');
    hold off;
    title('Potential from a sphere of charge along the y-axis');
    xlabel('y');
    ylabel('V');
    legend('show');
end