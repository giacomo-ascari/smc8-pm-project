% symbolic math solver

% LIST SYMBOLS
% syms x y q;

% p -> plus one OR positive
% m -> minus one OR negative
% c -> center

disp("defining symbols")

% they have to cover all the stencil
syms d_tp d_tc d_tm d_tt;
syms d_xp d_xc d_xm d_xx d_xxxx;
syms u(n,l);
syms n l;
syms h k;
syms kappa;

% DECLARE EQUATION (or system)
% eqn = y == 2*x;
% OR SYSTEM
% eqns = [y == 2*x + q, 4 == 1 + q];

disp("defining equations")

eqns = [
    d_tp * u(n,l) == 1/k * (u(n+1,l) - u(n,l)), ...                        % forward time difference
    d_tm * u(n,l) == 1/k * (u(n,l) - u(n-1,l)), ...                        % backward time difference
    d_tc * u(n,l) == 1/(2*k) * (u(n+1,l) - u(n-1,l)), ...                  % centred time difference
    d_xp * u(n,l) == 1/h * (u(n,l+1) - u(n,l)), ...                        % forward space difference
    d_xm * u(n,l) == 1/h * (u(n,l) - u(n,l-1)), ...                        % backward space difference
    d_xc * u(n,l) == 1/(2*h) * (u(n,l+1) - u(n,l-1)), ...                  % centred space difference
    d_tt * u(n,l) == d_tp * d_tm * u(n,l), ...                             % some more cheat sheet
    d_xx * u(n,l) == d_xp * d_xm * u(n,l), ...                             % some more cheat sheet
    d_xxxx * u(n,l) == d_xp * d_xp * d_xm * d_xm * u(n,l), ...             % some more cheat sheet
    d_tt * u(n,l) == -kappa*kappa*d_xxxx*u(n,l)
];

% SOLVE FOR A VARIABLE
% solution_x = solve(eqn, x);
% OR VARIABLES
% solution_x = solve(eqns, [x, q]);

disp("solving...")

s = solve(eqns, u);
disp(s)

disp("solved")

rewrite(eqns, "kappa")

%%

syms x y; % Define symbolic variables
syms f(x, y); % Define symbolic function f(x, y)

% Define your equations
eq1 = x + y == 5;
eq2 = f(x, y) == x^2 + y^2;

% Substitute the desired value into the function f(x, y)
desired_value = f(x-1, y);
eq2_sub = subs(eq2, f(x, y), desired_value);

% Solve the system of equations with the substituted function
solution = solve([eq1, eq2_sub], [x, y]);

% Display the solutions
disp('Solutions:');
disp(solution);
