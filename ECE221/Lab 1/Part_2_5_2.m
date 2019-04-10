function [output] = Part_2_5_2(lower,upper)
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
    num_differentials = max((upper-lower)*10000+1,10000);
    x = linspace(lower,upper,num_differentials);
    dx = x(2) - x(1);
    
    y = cos(x).^2;
    
    S = sum(dx*y)
end
