
% коэффициент перевода из радиан в градусы
kc = 57.2958;

% *************************************************
% *** Переменные подводного аппарата
% *************************************************

m = 300; % кг
Cvx1 = 182.87;
Cvx2 = 33;
lambda11 = 12.72;

Jyy = 366;
Cwy1 = 1144.7;
Cwy2 = 22.52;
lambda55 = 117.08;

P1 = 60 * 9.81; % = 588 H, упор одного движителя
U1 = 24; % В, напряжение

% Для ПФ одного винтомотора
K_thruster = P1 / U1; % = 24.5 H/B;
T_thruster = 0.3; % с

l = 0.225;

% ПФ ДРК по маршу
K_propulsion_x = K_thruster * 4;
T_propulsion_x = T_thruster;

% ПФ ДРК по курсу
K_propulsion_w = K_thruster * 4 * l;
T_propulsion_w = T_thruster;

V_linearization = 0; % 0.2; 
w_linearization = 0;

% Получим передаточную функцию канала маршевой скорости
K_Vx = 1 / (2*Cvx1 * V_linearization + Cvx2); 
T_Vx = (m + lambda11)/(2*Cvx1 * V_linearization + Cvx2);


% Получим передаточную функцию канала поворота по курсу
K_omega_y = 1 / (2 * Cwy1 * w_linearization + Cwy2);
T_omega_y = (Jyy + lambda55)/(2 * Cwy1 * w_linearization + Cwy2);


% Для марша
T_v = (2*T_propulsion_x*(m+lambda11)*0.707)/...
    ( (2*Cvx1 * V_linearization + Cvx2)*T_propulsion_x + m + lambda11);

k2_m = ( T_propulsion_x*(m+lambda11) - (2*Cvx1 * V_linearization + Cvx2)* T_v^2) /...
    ( T_v^2 * K_propulsion_x );

K_v = K_propulsion_x / ( K_propulsion_x*k2_m + 2*Cvx1 * V_linearization + Cvx2 );

M = 1.03;
k1_m = (M^2 + M* sqrt(M^2 - 1)) / ( 4 * T_v * K_v);


% Для курса
T_omega = (2*T_propulsion_x*(Jyy+lambda55)*0.707)/...
    ( (2*Cwy1 * w_linearization + Cwy2)*T_propulsion_w + Jyy + lambda55);


k2_yaw = ( T_propulsion_x*(Jyy+lambda55) - (2*Cwy1 * w_linearization + Cwy2)* T_omega^2) /...
    ( T_omega^2 * kc * K_propulsion_w );


K_omega =  K_propulsion_w / ( K_propulsion_w* kc * k2_yaw + 2*Cwy1 * w_linearization + Cwy2 );

k1_yaw = (M^2 + M* sqrt(M^2 - 1)) / ( 4 * T_omega * K_omega * kc);
%k2_yaw = 10;

% Коэффициенты, которые необходимо синтезировать
%k1_m = 600; %4066;
%k2_m = 500; %527.58;
%k1_k = 0.3;
%k2_k = 0.16732;




