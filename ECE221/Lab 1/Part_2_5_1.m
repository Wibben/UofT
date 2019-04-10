function [output] = Part_2_5_1(coefficients, range)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    num_points = (range(2)-range(1))*100 + 1;
    num_coeff = numel(coefficients);
    term = zeros(num_coeff,num_points);
    
    x = linspace(range(1),range(2),num_points);
    y = zeros(1,num_points);
    
    for i=1:num_coeff
        % Calculate for the term
        term(i,:) = coefficients(i)*x.^(i-1);
        % Add to final function
        y = y + term(i,:);
    end
    
    % Plotting
    figure(3);
    title(['Plot of polynomial of Order ',mat2str(num_coeff)]);
    hold on;
    % Plot each term
    for i = 1:num_coeff
        plot(x,term(i,:),'DisplayName',['Term ',mat2str(i)]);
    end
    % Plot final function
    plot(x,y,'DisplayName','y(x)');
    hold off;
    
    legend;
end