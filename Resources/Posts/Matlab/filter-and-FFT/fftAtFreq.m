function [ay,freq,ph] = fftAtFreq(V,fs,NFFT,ommit)
% Perform FFT on matrix V. Every column in V is a sequence of data from one
% channel.
% fs - the sample frequency
% NFFT - the number of nodes for FFT
%
% Author
% ======
%
% HU Yaoyu <huyaoyu@sjtu.edu.cn>
%

% local variables
n = 16; % default order

[row,col] = size(V);

% FFT

if (NFFT <= 0 || NFFT > row)
    N = row;
else
    N = NFFT;
end

% NFFT = 10000;
y = fft(V,N)/N;

freq = fs/2*linspace(0,1,N/2+1);

ay  = 2*abs(y(1:N/2+1,:));

% phase
ph = angle(y(1:N/2+1,:));

end