clear all; close all; clc; disp(datetime)

% Read impulse response
fname = '../data/impres';
fileID = fopen(fname);
impres = fread(fileID, 'float64');
fclose(fileID);

% Plot the whole data
figure('Units', 'normalized', 'Position', [.2 .2 .7 .4])
plot(impres);
title("Réponse impulsionnelle")
xlabel("Échantillons")
grid on
grid minor

% Plot zoom #1
range = 1:2048;
axes('Position', [.35 .62 .5 .23])
plot(impres(range))
xlim([range(1)-1, range(end)])
grid on

% Plot zoom #2
range = 1:512;
axes('Position', [.35 .25 .5 .23])
plot(impres(range))
xlim([range(1)-1, range(end)])
grid on