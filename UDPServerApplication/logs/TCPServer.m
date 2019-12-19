% Clear console and workspace
close all;
clear all;
clc;

% Configuration and connection
disp ('Receiver started');
t=tcpip('0.0.0.0', 4013,'NetworkRole','server');

% Wait for connection
disp('Waiting for connection');
fopen(t);
disp('Connection OK');

% Read data from the socket
for i=0:100
    tdata = fscanf(t);
    
    data = str2double(split(tdata, ' '))
end