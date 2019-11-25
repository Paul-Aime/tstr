% Convolutional reverb - Offline
% T. Hueber - CNRS/GIPSA-lab
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

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
imp_left = imp(:,1);

N = length(x);        % Total length of the signal
L = length(imp_left); % Impusle response length
M = 512;              % Lenght of the window (== BufferSize)

% convolution in the temporal domain
x_conv_tot = zeros(1, N+M-1);

x_conv_prev = zeros(1, L+M-1);
x_conv_next = zeros(1, L+M-1);

for k = 1:L:N
    x_conv_prev = zeros(1, L+M-1);
    x_conv_next = zeros(1, L+M-1);
    for n=1:L+M-1
        tmp = 0;
        if (n>=M)
          kmin = n-M+1;
        else
          kmin = 1;
        end

        if (n<L)
          kmax = n;
        else
          kmax = L;
        end

        fprintf('kmin=%i,kmax=%i,n=%i\n',kmin,kmax,n);

        for k=kmin:kmax
          tmp = tmp + x(k)*imp_left(n-k+1);
        end
        x_conv(n)=tmp;
    end
end

soundsc(x_conv,Fs);

% convolution in the frequential domain
% TODO

%% END