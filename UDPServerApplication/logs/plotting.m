file = load('1.txt');

%legend
%1 - time 
%2 - ax1
%3 - ay1
%4 - az1
%5 - gx1
%6 - gy1
%7 - gz1
%8 - mx1
%9 - my1
%10 - mz1
%11 - q0
%12 - q1
%13 - q2
%14 - q3
%15 - Yaw1
%16 - Pitch1
%17 - Roll1

%18 - ax2
%19 - ay2
%20 - az2
%21 - gx2
%22 - gy2
%23 - gz2
%24 - mx2
%25 - my2
%26 - mz2
%27 - q0
%28 - q1
%29 - q2
%30 - q3
%31 - Yaw2
%32 - Pitch2
%33 - Roll2

%34 - ax3
%35 - ay3
%36 - az3
%37 - gx3
%38 - gy3
%39 - gz3
%40 - mx3
%41 - my3
%42 - mz3
%43 - q0
%44 - q1
%45 - q2
%46 - q3
%47 - Yaw3
%48 - Pitch3
%49 - Roll3



figure('Name','IMU 1');
subplot(2,1,1);
plot(file(:,1),file(:,11),file(:,1),file(:,12),file(:,1),file(:,13),file(:,1),file(:,14),'LineWidth',3);
legend('q0','q1','q2','q3');
subplot(2,1,2);
plot(file(:,1),file(:,15),file(:,1),file(:,16),file(:,1),file(:,17),'LineWidth',3);
legend('Yaw','Pitch','Roll');
grid on;

figure('Name','IMU 2');
subplot(2,1,1);
plot(file(:,1),file(:,27),file(:,1),file(:,28),file(:,1),file(:,29),file(:,1),file(:,30),'LineWidth',3);
legend('q0','q1','q2','q3');
subplot(2,1,2);
plot(file(:,1),file(:,31),file(:,1),file(:,32),file(:,1),file(:,33),'LineWidth',3);
legend('Yaw','Pitch','Roll');
grid on;

figure('Name','IMU 3');
subplot(2,1,1);
plot(file(:,1),file(:,43),file(:,1),file(:,44),file(:,1),file(:,45),file(:,1),file(:,46),'LineWidth',3);
legend('q0','q1','q2','q3');
subplot(2,1,2);
plot(file(:,1),file(:,47),file(:,1),file(:,48),file(:,1),file(:,49),'LineWidth',3);
legend('Yaw','Pitch','Roll');
grid on;