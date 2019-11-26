% Convolutional reverb - Offline
% T. Hueber - CNRS/GIPSA-lab
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% read the sample waveform
filename='./MG_list1_sent381.wav';
[x,Fs] = audioread(filename);


% read the impulse response waveform
filename='./IMreverbs1/Trig Room.wav';
[imp,Fsimp] = audioread(filename);

% Keep only one channel 
imp_left = imp(:,1);

M = length(imp_left);% N
N = length(x);% L
L = 1024; %length of segments P
  % convolution in tL//P, Phe temporal domain (slower)
  x_wind = zeros(fix(N/L), L);
  y_conv = zeros(fix(N/L),M+L-1);
  y_out = zeros(1,fix(N/L)*L+M-1);
  for n=1:fix(N/L) 
    x_wind(n,:) =x((n-1)*L+1:n*L);
    y_conv(n, :) = convTemp(x_wind(n, :),imp_left);
    K1=(n-1)*L+1;
    K2 = n*L+M-1; 
    y_out(K1:K2) =y_out(K1:K2) + y_conv(n, :);
  end
  soundsc(y_out,Fs);




%% END