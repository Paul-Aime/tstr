%% Convolutional reverb - Online simulation
% P. Aimé - Phelma, 3A SICOM
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all; close all; clc; disp(datetime);

%% Read files

% read the sample waveform
filename='./MG_list1_sent381.wav';
[x, Fs] = audioread(filename);

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
file_idx = 10;
filename = "./IMreverbs1/" + impulse_resp_dict(file_idx) + ".wav"
[imp, Fsimp] = audioread(filename);
imp_left = imp(:,1); % keep only one channel 

K = length(x);        % total length of the signal
M = length(imp_left); % impusle response length
L = 8192;             % lenght of the window (== BufferSize)
K = K - mod(K,L);     % no simulation of non-full (last) buffer (underrun)


%% Convolution in the temporal domain

x_conv_tot = zeros(K+M-1, 1);
x_conv = zeros(L+M-1, 1);

fprintf("Computing bloc-convolution...      ")
step = 0;
nb_step = K/L;
for k = 1:L:K
    % compute convolution for buffer k and add it to previous computation
    x_conv = x_conv + tconv(x(k:k+L-1), imp_left); % size (L+M-1, 1)
    
    % output totally computed part (output buffer)
    x_conv_tot(k:k+L-1) = x_conv(1:L);
    
    % save overlapping part in the next buffer
    x_conv(1:M-1) = x_conv(L+1:end); % L+1 = end-(M-1)
    
    % reset the remaining buffer samples
    x_conv(M:end) = 0;
    
    % disp progression
    step = step + 1;
    fprintf("\b\b\b\b\b%3d %%", int64(100*step/nb_step))
end
fprintf('\n') % end of progression display

% play result
soundsc(x_conv_tot, Fs);


%% Convolution in the frequential domain
% TODO

%% END