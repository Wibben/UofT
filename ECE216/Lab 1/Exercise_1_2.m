T=1;
Fs = 48000;
N = T*Fs;
t = 0 : 1/Fs : T;
Fn = 1000;
y = 2*sin(Fn*2*pi*t);
sound(y,Fs);

db_gain = 20*log(2)/log(10);
fprintf("The DB gain due to doubling the voltage is %.2f\n",db_gain);