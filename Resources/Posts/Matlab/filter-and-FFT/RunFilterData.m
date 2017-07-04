%
% Description
% ===========
% 
% Filter input data by Butterworth digital filter.
%
% Author
% ======
% 
% HU Yaoyu <huyaoyu@sjtu.edu.cn>
%
% Date
% ====
%
% Created on: 20160824
% Modified on:
%
% Usage
% =====
%
% The user should set the values listed in the section "User inputs.".
% 
% After running this script, the filtered data will be saved as .dat files.
% And some associated figures are also saved, including the .fig format and
% the .bmp format.
% 

% ================ Prepare workspace. ==================
clc
clear
close ALL

% ===================== Constants ======================


% =================== User inputs ======================

% Filter settings.
fs       = 800; % sample frequency, Hz
passFreq = 100; % pass band frequency, Hz
stopFreq = 120; % stop band frequency, Hz

% Input files.
ifs = {
    'test-input-data.xlsx';
};

N_FILES = size(ifs, 1);

% N_LINES = 60000; % Lines of data
% CHANNELS  = [2, 3, 4];     % Columns in the Excel file.
CHANNELS  = [2];     % Columns in the Excel file.

N_CHANNELS = size(CHANNELS, 2);

% =================== Filter =======================

dbutter = designfilt(...
'lowpassiir', 'PassbandFrequency', passFreq,...
'StopbandFrequency', stopFreq, ...
'PassbandRipple', 1,...
'StopbandAttenuation', 60,...
'SampleRate', fs,...
'DesignMethod', 'butter');

for I = 1:1:N_FILES
    
    close all;
    
    fn = ifs{I};
    [pathDir, name, ext] = fileparts(fn);
    
    % ==================== Read data. ====================
    
    fprintf('Reading data from %s...\n', fn);
    V = xlsread(fn);
    T = V(:, 1);
    V = V(:, CHANNELS);

    % ================ Filter data. ======================
    
    V_filtered = filter(dbutter, V);
    
    % =================== FFT. ===========================
    
    [ay,     freq,   ph] = fftAtFreq(V,          fs, 0, 0);
    [ay_F, freq_F, ph_F] = fftAtFreq(V_filtered, fs, 0, 0);
    
    % ================== Output. =========================
    
    for J = 1:1:N_CHANNELS
        % ================== Plot data. ==================
        
        saveName = sprintf('%s_LowPass_%dHz_C%0d', name, passFreq, J);
        
        h = figure('NumberTitle', 'off', 'Name', saveName);
        subplot(2, 1, 1);
        plot(T, V(:,CHANNELS(1, J) - 1));
        title('Raw data');
        xlabel('Time (s)');
        
        subplot(2, 1, 2);
        plot(T, V_filtered(:, CHANNELS(1, J) - 1));
        title('Filtered data');
        xlabel('Time (s)');
        
        saveas(h, saveName, 'fig');
        saveas(h, saveName, 'bmp');
        
        % ================ Plot FFT results. ============
        
        saveName = sprintf('%s_LowPass_%dHz_FFT_C%0d', name, passFreq, J);
        
        h = figure('NumberTitle', 'off', 'Name', saveName);
        subplot(2, 1, 1);
        plot(freq, ay(:,J));
        title('Raw data');
        xlabel('Frequency (Hz)');
        
        subplot(2, 1, 2);
        plot(freq, ay_F(:,J));
        title('Filtered data');
        xlabel('Frequency (Hz)');
        
        saveas(h, saveName, 'fig');
        saveas(h, saveName, 'bmp');
    end % J
    
    saveName = sprintf('%s_LowPass_%dHz.dat', name, passFreq);

    save(saveName, 'V', '-ascii');
end % I

    