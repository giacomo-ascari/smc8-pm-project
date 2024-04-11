% damped stiff string (supported boundary)
 
N = 50; % length of bar 
fs = 48000; % sampling rate for ss 
dur = 3; 
 
% parameter (lambda groups them all) 
c = 200;
L = 1;
s0 = 1;
s1 = 0.005;
kappa = 10;
k = 1;

h = sqrt((c*c*k*k + 4*s1*k + sqrt((c*c*k*k + 4*s1*k)^2 + 16*kappa*kappa*k*k)) / 2);
 
% time only scouts +1, 0, -1 
u_next = zeros(1, N); 
u_curr = zeros(1, N); 
u_prev = zeros(1, N); 
 
input = [1, zeros(1,dur*fs-1)]; 
input_pos = floor(N*0.13); 
output = zeros(1, dur*fs); 
output_pos = floor(N*0.78); 
 
% iteratore over time 
for n = 1:dur*fs 
 
    u_curr(input_pos) = u_curr(input_pos) + input(n); 
     
    % iterate over length 
    for l = (1+2):(N-2)
        
        u_next(l) = lambda * (u_curr(l+2) - 4*u_curr(l+1) + 6*u_curr(l) - 4*u_curr(l-1) + u_curr(l-2)) + 2*u_curr(l) - u_prev(l); 
    end 
 
    % boundaries 
    u_next(1) = 0; 
    u_next(2) = -u_next(3); 
    u_next(N-1) = -u_next(N-2); 
    u_next(N) = 0; 
 
    output(n) = u_next(output_pos); 
     
    u_prev = u_curr; 
    u_curr = u_next; 
 
end 
 
plot(output); 
 
soundsc(output, fs);