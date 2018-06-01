%% clear
close all; clc; clear;

%%
fs = 10; %sampling frequency
ret = read_log_file('out_0508_1159');
[n_packets,~] = size(ret);
data1 = zeros(1,n_packets);
data2 = zeros(1,n_packets);
nr1 = 1;
nc1 = 1;
nr2 = 1;
nc2 = 2;
pac = 1:n_packets;
channel_info = zeros(6,n_packets);
cur_channel = 1;
for subcarrier = 56:4:56
    for packet = 1:n_packets
        csi_data = ret{packet}.csi;
        if(csi_data ~= 0)
            data1(1,packet) = csi_data(nr1,nc1,subcarrier);
            data2(1,packet) = csi_data(nr2,nc2,subcarrier);
        end
    end
    ang1 = angle(data1);
    ang2 = angle(data2);
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    x = pac;
    y = mod(ang1 - ang2, 2*pi);
    opol = 5;
    [p,s,mu] = polyfit(x, y, opol);
    f_y = polyval(p, x, [], mu);
    figure(1)
    plot(x/40, y, 'r', x/40, f_y, 'b', 'linewidth', 2)
    legend('Signal', 'Baseline')
    set(gca, 'FontSize', 40)
    ylim([0 7])
    y = y - f_y;
    figure(2)
    plot(x/40, y, 'r', 'linewidth', 2)
    set(gca, 'FontSize', 40)
    wavename='cmor3-3';
    totalscal = 2048; %尺度序列的长度，即scal的长度 %length of scale
    wcf = centfrq(wavename); %小波的中心频率 %center frequency of wavelet
    cparam = 2 * wcf * totalscal; %为得到合适的尺度所求出的参数 %coefficient
    a = totalscal:-1:1;
    scal = cparam ./ a; %得到各个尺度，以使转换得到频率序列为等差序列
    coefs = cwt(y, scal, wavename); %得到小波系数 %wavelet coefficient
    f = scal2frq(scal, wavename, 1/fs); %将尺度转换为频率 %change scale to frequency
    figure(3)
    imagesc(x/40, f, abs(coefs)); %绘制色谱图 %plot the imaging
    xlabel('time t/s');
    ylabel('frequency f/Hz');
    title('time-frequency presentation');
    ylim([0 3])
    set(gca, 'FontSize', 20)
    set(gca, 'position', [0.12, 0.12, 0.8, 0.8])
    set(gcf, 'position', [100 100 900 800])
    yy = abs(coefs);
    yy(find(f<1), :) = 0;
    figure(4)
    imagesc(x/40, f, yy); %绘制色谱图
    xlabel('time t/s');
    ylabel('frequency f/Hz');
    title('time-frequency presentation');
    ylim([1 2])
    set(gca, 'FontSize', 20)
    set(gca, 'position', [0.12, 0.12, 0.8, 0.8])
    set(gcf, 'position', [100 100 900 800])
end
