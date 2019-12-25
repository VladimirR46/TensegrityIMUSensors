file = load('1.txt');

%% legend
% 1 - time 

%% IMU 1
% 2 - ax1
% 3 - ay1
% 4 - az1
% 5 - gx1
% 6 - gy1
% 7 - gz1
% 8 - mx1
% 9 - my1
% 10 - mz1
% 11 - q0
% 12 - q1
% 13 - q2
% 14 - q3
% 15 - Yaw1
% 16 - Pitch1
% 17 - Roll1
%% IMU 2
% 18 - ax2
% 19 - ay2
% 20 - az2
% 21 - gx2
% 22 - gy2
% 23 - gz2
% 24 - mx2
% 25 - my2
% 26 - mz2
% 27 - q0
% 28 - q1
% 29 - q2
% 30 - q3
% 31 - Yaw2
% 32 - Pitch2
% 33 - Roll2
%% IMU 3
% 34 - ax3
% 35 - ay3
% 36 - az3
% 37 - gx3
% 38 - gy3
% 39 - gz3
% 40 - mx3
% 41 - my3
% 42 - mz3
% 43 - q0
% 44 - q1
% 45 - q2
% 46 - q3
% 47 - Yaw3
% 48 - Pitch3
% 49 - Roll3
%%


%% Acceleration
figure('Name','IMU (Acceleration)');
subplot(3,1,1);
plot(file(:,1),file(:,2),file(:,1),file(:,3),file(:,1),file(:,4),'LineWidth',3);
title('Acceleration (imu sensor_{1})');
xlabel('time (microsec)');
ylabel('[m/s^2]');
legend('a_x','a_y','a_z');
subplot(3,1,2);
plot(file(:,1),file(:,18),file(:,1),file(:,19),file(:,1),file(:,20),'LineWidth',3);
title('Acceleration (imu sensor_{2})');
xlabel('time (microsec)');
ylabel('[m/s^2]');
legend('a_x','a_y','a_z');
subplot(3,1,3);
plot(file(:,1),file(:,34),file(:,1),file(:,35),file(:,1),file(:,36),'LineWidth',3);
title('Acceleration (imu sensor_{3})');
xlabel('time (microsec)');
ylabel('[m/s^2]');
legend('a_x','a_y','a_z');

%% Speed
figure('Name','IMU (Speed)');
subplot(3,1,1);
plot(file(:,1),file(:,5),file(:,1),file(:,6),file(:,1),file(:,7),'LineWidth',3);
title('Speed (imu sensor_{1})');
xlabel('time (microsec)');
ylabel('[deg/s]');
legend('g_x','g_y','g_z');
subplot(3,1,2);
plot(file(:,1),file(:,21),file(:,1),file(:,22),file(:,1),file(:,23),'LineWidth',3);
title('Speed (imu sensor_{2})');
xlabel('time (microsec)');
ylabel('[deg/s]');
legend('g_x','g_y','g_z');
subplot(3,1,3);
plot(file(:,1),file(:,21),file(:,1),file(:,22),file(:,1),file(:,23),'LineWidth',3);
title('Speed (imu sensor_{3})');
xlabel('time (microsec)');
ylabel('[deg/s]');
legend('g_x','g_y','g_z');
grid on;

%% Magnetic
figure('Name','IMU (Magnetic)');
subplot(3,1,1);
plot(file(:,1),file(:,8),file(:,1),file(:,9),file(:,1),file(:,10),'LineWidth',3);
title('Magnetic (imu sensor_{1})');
xlabel('time (microsec)');
ylabel('[gauss]');
legend('m_x','m_y','m_z');
subplot(3,1,2);
plot(file(:,1),file(:,24),file(:,1),file(:,25),file(:,1),file(:,26),'LineWidth',3);
title('Magnetic (imu sensor_{2})');
xlabel('time (microsec)');
ylabel('[gauss]');
legend('m_x','m_y','m_z');
subplot(3,1,3);
plot(file(:,1),file(:,40),file(:,1),file(:,41),file(:,1),file(:,42),'LineWidth',3);
title('Magnetic (imu sensor_{3})');
xlabel('time (microsec)');
ylabel('[gauss]');
legend('m_x','m_y','m_z');
grid on;

%% Quaternions
figure('Name','IMU (Quaternions)');
subplot(3,1,1);
plot(file(:,1),file(:,11),file(:,1),file(:,12),file(:,1),file(:,13),file(:,1),file(:,14),'LineWidth',3);
title('Quaternions (imu sensor_{1})');
xlabel('time (microsec)');
ylabel('radians');
legend('q_0','q_1','q_2','q_3');
subplot(3,1,2);
plot(file(:,1),file(:,27),file(:,1),file(:,28),file(:,1),file(:,29),file(:,1),file(:,30),'LineWidth',3);
title('Quaternions (imu sensor_{2})');
xlabel('time (microsec)');
ylabel('radians');
legend('q_0','q_1','q_2','q_3');
subplot(3,1,3);
plot(file(:,1),file(:,43),file(:,1),file(:,44),file(:,1),file(:,45),file(:,1),file(:,46),'LineWidth',3);
title('Quaternions (imu sensor_{3})');
xlabel('time (microsec)');
ylabel('radians');
legend('q_0','q_1','q_2','q_3');


%% Euler Angles
figure('Name','IMU (Euler Angles)');
subplot(3,1,1);
plot(file(:,1),file(:,15),file(:,1),file(:,16),file(:,1),file(:,17),'LineWidth',3);
title('Imu sensor_{2}');
xlabel('time (microsec)');
ylabel('degrees');
legend('Yaw','Pitch','Roll');
subplot(3,1,2);
plot(file(:,1),file(:,31),file(:,1),file(:,32),file(:,1),file(:,33),'LineWidth',3);
title('Imu sensor_{2}');
xlabel('time (microsec)');
ylabel('degrees');
legend('Yaw','Pitch','Roll');
subplot(3,1,3);
plot(file(:,1),file(:,47),file(:,1),file(:,48),file(:,1),file(:,49),'LineWidth',3);
title('Imu sensor_{3}');
xlabel('time (microsec)');
ylabel('degrees');
legend('Yaw','Pitch','Roll');

