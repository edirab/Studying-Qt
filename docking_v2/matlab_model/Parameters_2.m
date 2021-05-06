% *************************************************
% *** Параметры, необходимые для расчёта траектории 
% *************************************************

% Диаметры, ограничивающие перемещение центра масс, м
% (см. чертёж)
Path = struct;

Path.D_mass_center_max = 13;
Path.D_mass_center_min = 5.8;

% Угол сектора и полный угол, на который планируется повернуть 
Path.angular_delta = 15;
Path.angle = 90;

% Вычислим количесво требуемых секторов
Path.N_points = fix(360 / Path.angular_delta);

%disp('class N:'); display(class(N));

% Вектора с координатами
z_outer = zeros(1, int32(Path.N_points));
x_outer = zeros(1, int32(Path.N_points));
z_inner = zeros(1, int32(Path.N_points));
x_inner = zeros(1, int32(Path.N_points));

for i=1:Path.N_points
    %disp( class(i) ); disp( i );
    
    [p1_z, p1_x] = pol2cart(deg2rad(Path.angle), Path.D_mass_center_max);
    [p2_z, p2_x] = pol2cart(deg2rad(Path.angle + Path.angular_delta/2), Path.D_mass_center_min);
    
    %disp(p1_z); disp(p1_x);
    
    z_outer(int32(i)) = p1_z;
    x_outer(int32(i)) = p1_x;
    
    z_inner(int32(i)) = p2_z;
    x_inner(int32(i)) = p2_x;
    
    Path.angle = Path.angle + Path.angular_delta;
    
end

% Инициализируем нулями массивы для точек итоговой траектории
Path.z = zeros(1, int32(Path.N_points*2));
Path.x = zeros(1, int32(Path.N_points*2));
Path.direction = zeros(1, int32(Path.N_points*2));

j = 1;
for i=1:Path.N_points
   Path.z(j) = z_outer(i);
   Path.x(j) = x_outer(i);
   Path.direction(j) = 1;
   j = j + 1;
   
   Path.z(j) = z_inner(i);
   Path.x(j) = x_inner(i);
   Path.direction(j) = -1;
   j = j + 1;
end

clear p1_z p1_x p2_z p2_x i j z_inner x_inner z_outer x_outer;

%disp(z_final);
%disp(x_final);

% *************************************************
% *** Переменные подводного аппарата
% *************************************************
AUV = struct;
HydroDyn = struct;

AUV.m = 300; %24.7
HydroDyn.Cvx1 = 182.87;
HydroDyn.Cvx2 = 33;
AUV.lambda11 = 12.72;

AUV.Jyy = 366;
HydroDyn.Cwy1 = 1144.7;
HydroDyn.Cwy2 = 22.52;
AUV.lambda55 = 117.08;

AUV.P1 = 60 * 9.81; % = 588 H, упор одного движителя
AUV.U1 = 24; % В, напряжение

% Для ПФ одного винтомотора
AUV.K_thruster = AUV.P1 / AUV.U1; % = 24.5 H/B;
AUV.T_thruster = 0.3; % с

AUV.l = 0.225;

% Коэффициенты, которые необходимо синтезировать
AUV.k1_m = 4.5227;
AUV.k2_m = 5.32;

% Коэффициенты, которые необходимо синтезировать
AUV.k1_yaw = 0.5146;
AUV.k2_yaw = 0.6370;

% коэффициент перевода из радиан в градусы
kc = 57.2958;

%Зададим скорость движения по маршу
Vfwd = [0.3 0.8 1.3];
Vfwd = Vfwd(3);

%Радуис столкновения == описанной окружности по нижнему основанию
% + половина длины АНПА
Path.Rcrash = sqrt(2) + 1.5;

% Радиус точки траектории
Path.Rmin = 0.4;

%Течение направлено вдоль оси Z
Vz = 0;
%Vz = 0.1;
%Vz = 0.2;

% Задержка
Z_delay = 0.01;%0.05;
