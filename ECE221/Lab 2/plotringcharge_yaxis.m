function plotringcharge_yaxis(y1,y2,a,Q)
    N = 500;
    rhol = Q/(2*pi*a);
    y = linspace(y1,y2,381);
    
    % Find values using ringofcharge
    for i = 1:381
        [Etot(i),Ex(i),Ey(i),Ez(i)] = ringofcharge(0,y(i),0,a,rhol,N);
    end
    
    % Plot
    figure(2);
    hold on;
    plot(y,Ex,'--b','DisplayName','Ex');
    plot(y,Ey,'-.r','DisplayName','Ey');
    plot(y,Ez,':k','DisplayName','Ez');
    hold off;
    title({'E-Field Components of a ring','of charge along the y-axis'});
    xlabel('y');
    ylabel('E-Field Component Value');
    legend('show');
end