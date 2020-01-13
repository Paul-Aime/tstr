% Convolutional reverb - Offline
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all; close all; clc; disp(datetime);

% read the sample waveform
filename='./MG_list1_sent381.wav';
[x,Fs] = audioread(filename);

% read the impulse response waveform
impulse_resp_dict = [
    "Five Colums";                  % 1
    "Five Colums Long";             % 2
    "French 18th Century Salon";    % 3
    "Going Home";                   % 4
    "In The Silo Revised";          % 5
    "Narrow Bumpy Space";           % 6
    "Nice Drum Room";               % 7
    "Parking Garage";               % 8
    "Rays";                         % 9
    "Trig Room";                    % 10
    ];
file_idx = 3;
filename = "./IMreverbs1/" + impulse_resp_dict(file_idx) + ".wav"
[imp,Fsimp] = audioread(filename);

% Keep only one channel 
imp = imp(:,1);

% Get lengths
M = length(imp)
L = length(x)

% Convolution in the temporal domain (slower)
x_conv_t = tconv(x, imp)
soundsc(x_conv_t, Fs);

% FFT-base convolution (faster)
x_conv_f = fconv(x, imp);
soundsc(x_conv_f, Fs);
