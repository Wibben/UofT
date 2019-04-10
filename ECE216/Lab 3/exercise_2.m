key_9_freq = get_freq(9);
fprintf("The frequency of key number 9 is %.2f\n",key_9_freq);

middle_C = get_wave('C','m',2,11025);
fprintf("Playing middle C sampled at 11025 Hz for 2 seconds\n");
soundsc(middle_C);
pause(3);

C_major_chord = get_chord_wave('C',2,11025);
fprintf("Playing C major chord sampled at 11025 Hz for 2 seconds\n");
soundsc(C_major_chord);
pause(3);

G_major_chord = get_chord_wave('G',2,11025);
fprintf("Playing G major chord sampled at 11025 Hz for 2 seconds\n");
soundsc(G_major_chord);
pause(3);

Am_minor_chord = get_chord_wave('C',2,11025);
fprintf("Playing A minor chord sampled at 11025 Hz for 2 seconds\n");
soundsc(Am_major_chord);
pause(3);
