clc
close all
%% 4.2.1
TI = 0.0725;%0.0863;
a = 1.576;%0.788;
b = 11;%5.5;
G = tf([TI 1],[TI 0])*tf([a],[1 b]);
rlocus(G)
rlocfind(G)
%% 4.2.2
a = 1.576;%0.788;
b = 11;%5.5;
theta = 0;
TI = 0.0725;%0.0863;
K = 18.4;%43.7;
VMAX_UPM = 11.75;
%% 4.2.3
TI = 0.0544;%0.0605;
a = 1.576;%0.788;
b = 11;%5.5;
G = tf([TI 1],[TI 0])*tf([a],[1 b]);
rlocus(G)
rlocfind(G)
theta = 0;
K = 31.1;%69.2; % Limited to 50
