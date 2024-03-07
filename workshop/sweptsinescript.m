%% GENERAL DATA

% Simultaneous measurement of impulse response and
% distortion with a swept-sine technique.
% Hell yeah

f1 = 50;
f2 = 15000;
fs = 48000;

w1 = 2 * pi * f1;
w2 = 2 * pi * f2;

T = 15;
t = linspace(0, T, fs*T);
iterations = 5;

disp("1");

%% GENERATE SINE SWEEP x(t)

% linear sine sweep
%linss = sin(w1 * t + (w2-w1)/T * (t.^2)/2);

% logarithmic sine sweep
logss = sin ( ...
    (w1*t)/log(w2/w1) ...
    .* (exp(t/T*log(w2/w1))-1) ...
);

audiowrite("logss.wav", logss, fs);
plot(t, logss);
%soundsc(logss, fs);

disp("2");

%% GENERATE INVERSE FILTER f(t)

% conv(x(t), f(t)) = d(t) (dirac)
% h(t) = conv(y(t), f(t))
% f(t) is called inverse filter
% farina what do you mean

%d = [1 zeros(1, T*fs-1)];
%[f, remainder] = deconv(logss, d);

f = fliplr(logss);
decay = -6*log2(w2/w1)/T*t; % in dB, from 0 to -6*log2(w2/w1)
env = 10.^(decay/10);
invf = f.*env;

audiowrite("invf.wav", invf, fs);
subplot(2, 1, 1)
plot(t, decay);
subplot(2, 1, 2)
plot(t, invf);

disp("3");

%% READ REC AND CONVOLVE

y = zeros(1, T*fs);
[invf,tempfs] = audioread("invf.wav");
invf = invf';

for i=1:iterations
    s = strcat("recordings\\rec", num2str(i), ".wav");
    [tempy, tempfs] = audioread(s);
    y = y + tempy';
end

y = y / iterations;
%plot(t, y);
soundsc(y,fs);

h = conv(y,invf);
normh = h / max(abs(h));

audiowrite("ir.wav", normh, fs);
plot(normh);

disp("4");

%% READ IR AND CONVOLVE

[h,fs] = audioread("irproc.wav");
[x,fs] = audioread("recordings\\untitled.wav");

y = conv(x, h);
ynorm = y / max(abs(y));
soundsc(y, fs);
%audiowrite("untitled.wav", ynorm, fs);