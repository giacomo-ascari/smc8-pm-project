% keyssss

[x,fs] = audioread("recordings\\keys recording 2.wav");

[f0,locs] = pitch(x,fs);

plot(f0(:,1));