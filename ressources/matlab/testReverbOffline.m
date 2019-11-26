% Convolutional reverb - Offline
% T. Hueber - CNRS/GIPSA-lab
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
file_idx = 3;% Convolutional reverb - Offline
% T. Hueber - CNRS/GIPSA-lab
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% read the sample waveform
filename='./MG_list1_sent381.wav';
[x,Fs] = audioread(filename);


% read the impulse response waveform
impulse_resp_dict = [
    "Five Columns";                 % 1
    "Five Columns Long";            % 2
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
[imp,Fsimp] = audioread(filename);

% Keep only one channel 
imp_left = imp(:,1);

M = length(imp_left)
L = length(x)

step_fft = 1;

if (step_fft == 0)
  % convolution in the temporal domain (slower)
  x_conv = zeros(1,L+M-1);
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
    %fprintf('kmin=%i,kmax=%i,n=%i\n',kmin,kmax,n);
    for k=kmin:kmax
      tmp = tmp + x(k)*imp_left(n-k+1);
    end
    x_conv(n)=tmp;
  end
  
  soundsc(x_conv,Fs);
else
  % FFT-base convolution (faster)
  x_conv = fconv(x,imp_left);
  
end

soundsc(x_conv,Fs);

%% END