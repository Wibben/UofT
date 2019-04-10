[bass, fs] = audioread('bass.wav');
guitar = audioread('guitar.wav');
drums = audioread('drums.wav');

guitar_len = length(guitar);
guitar_time = guitar_len/fs;
fprintf("The duration of the guitar signal is %.2f seconds.\n", guitar_time);

duration = 5;
b = bass(1:fs*duration);
g = guitar(1:fs*duration);
d = drums(1:fs*duration);

fprintf("Normal Synthesized version.\n");
comp = b+g+d;
sound(comp,fs);

pause(duration+1);

fprintf("Ramped guitar version.\n");
ramp = transpose(linspace(1,5,fs*duration));
g_ramped = g.*ramp;
comp2 = b+g_ramped+d;
sound(comp2,fs);