% damped stiff string (simply supported boundary)
 
fs = 44100; % sampling rate for ss 
dur = 5; 
f0 = 300; % Hz | f0 = c / (2*L);
 
% parameters 
E = 210*(10^9); % Pa, young's modulus
T = 750; % N, tension
ro = 7850; % kg/m3, density
r = 0.001; % m, radius
A = pi * (r^2); % m2, cross area
I = pi * (r^4) / 4; % m4, momentum

c = sqrt(T / (ro * A)); % m/s, wave speed (174)
kappa = sqrt(E * I / (ro * A)); % m2/s, stiffness coeff. (2.585)
disp(c);
disp(kappa);
L = 0.5 * c / f0; % 1 in this example
s0 = 1;
s1 = 0.005;

k = 1/fs;
lower_h = sqrt((c*c*k*k + 4*s1*k + sqrt((c*c*k*k + 4*s1*k)^2 + 16*kappa*kappa*k*k)) / 2);
N = floor(L/lower_h);
h = L / N;

lambda = c*k/h;
mu = kappa*k/(h*h);

% for simply supported, we need two virtual points
N_virt = (N + 1) + 2;

% time only scouts +1, 0, -1 
u_next = zeros(1, N_virt); 
u_curr = zeros(1, N_virt); 
u_prev = zeros(1, N_virt); 
 
input = [1, zeros(1,dur*fs-1)]; 
input_pos = floor(N_virt*0.13); 
output = zeros(1, dur*fs); 
output_pos = floor(N_virt*0.78);
 
% iteratore over time 
for n = 1:dur*fs 
 
    u_curr(input_pos) = u_curr(input_pos) + input(n); 
     
    % iterate over length 
    % for simply supported, l belongs to {1,...,N-1}
    % but there are virtual points, extending u in space
    % and boundaries are manual
    for l = (mymap(1)):(mymap(N-1))
        u_next(l) = (1 / (1+s0*k)) * (...
            + (2 - 2*lambda*lambda - 6*mu*mu - 4*s1*k/(h*h)) * u_curr(l) + ...
            + (lambda*lambda + 4*mu*mu + 2*s1*k/(h*h)) * (u_curr(l+1) + u_curr(l-1)) + ...
            - mu*mu*(u_curr(l+2) + u_curr(l-2)) + ...
            + (-1 + s0*k + 4*s1*k/(h*h)) * u_prev(l) + ...
            - 2*s1*k/(h*h) * (u_prev(l+1) + u_prev(l-1))); 
    end 
 
    % boundaries 
    u_next(mymap(0)) = 0;
    u_next(mymap(-1)) = -u_next(mymap(1)); 
    u_next(mymap(N)) = 0; 
    u_next(mymap(N+1)) = -u_next(mymap(N-1)); 
 
    output(n) = u_next(output_pos); 
     
    u_prev = u_curr; 
    u_curr = u_next; 
 
end 
 
plot(output); 
 
soundsc(output, fs);




function y = mymap(x)
	y = x + 2;
end