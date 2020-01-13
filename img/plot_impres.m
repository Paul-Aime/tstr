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
range1 = 1:2048;
axes('Position', [.35 .62 .5 .23])
plot(impres(range1))
xlim([range1(1)-1, range1(end)])
grid on

% Plot zoom #2
range2 = 1:512;
axes('Position', [.35 .25 .5 .23])
plot(impres(range2))
xlim([range2(1)-1, range2(end)])
grid on


%% Spectrum

% Time specifications
fs = 44100;
dt = 1/fs;
nsamples = length(impres);
t = (0:dt:(nsamples-1)*dt)';
assert(size(t, 1) == nsamples);

% Frequency specifications
nfft = nsamples;
df = fs/nfft;
f = -fs/2:df:fs/2-df;

% Fourier Transform
S = fftshift(fft(impres(1:nsamples), nfft));
S = abs(S) / nsamples; % normalize

% Keep only >= 0 part 
% TODO do it by going to hilbert representation of the signal
S = S(f>=0);
f = f(f>=0);

% Sliding mean
Sm = zeros(size(S)); % initialize
ws_hz = 500; % window size in Hz
ws = floor(ws_hz/df); % in number of samples
if ~mod(ws, 2) % enforce odd value for ws
    ws = ws - 1;
end
pl = S(1:floor((ws-1)/2)); % left padding
pr = S(end-floor((ws-1)/2):end); % left padding
Sp = [pl; S; pr]; % padding
for idx = 1:length(S)
    Sm(idx) = mean(Sp(idx:idx+ws-1));
end

% Plot the spectrum
figure('Units', 'normalized', 'Position', [.2 .2 .7 .4])
plot(f, S, 'b-');
hold on
hm = plot(f, Sm, 'g-', 'LineWidth', 2);
xlim([0 f(end)])
ax = gca;
ax.XAxis.TickLabels = split(num2str(ax.XAxis.TickValues./1000)); % Hz -> kHz
xlabel('Frequency [kHz]');
title('Module of Fourier Transform')
legend(hm, ['mean over ' num2str(ws_hz) ' Hz sliding window'])