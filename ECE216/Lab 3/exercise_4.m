%tempo = 114; % bpm
tempo = 114;
% Pre-Chorus + Chorus to never gonna give you up
% https://sheets-piano.ru/wp-content/uploads/2012/02/Rick-Astley-Never-Gonna-Give-You-Up.pdf
nggyu_song_data = {
    %'Bb', 4;    'C' , 4;    'Bb', 2;    'Gm', 2; 'C' , 3; % Pre-chorus
    'C' , 1;    'Bb', 1.5;  'C' , 2.5;  'Am', 1.5;
    'Dm', 2.5;  'Gm', 1.5;  'C' , 2.5;  'Am', 1;
    'Dm', 3;    'Bb', 1.5;  'C' , 2.5;  'Am', 1.5;
    'Dm', 2.5;  'Gm', 1.5;  'C' , 2.5;  'Am', 1;
    'Dm', 2;    
};

% Never gonna give you up
nggyu = get_song_wave(nggyu_song_data,60/tempo,11025);
%soundsc(nggyu);
nggyu_strummed = ADSR_env(nggyu,60/tempo,16,11025);
soundsc(nggyu_strummed);

spectrogram(nggyu_strummed, length(nggyu_strummed)/64, 0);