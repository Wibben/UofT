plot(ScopeData(:,1),ScopeData(:,2),'r');
hold on;
plot(ScopeData(:,1),ScopeData(:,3),'b');
title('Performance Plot (Speed v(t) vs reference r(t))');
xlabel('Time (s)');
ylabel('Velocity (ms^(-1))');
legend('r(t)','v(t)');
figure(2);
plot(ScopeData2(:,1),ScopeData2(:,2));
title('Saturation Voltage Plot');
xlabel('Time (s)');
ylabel('V_m (V)');