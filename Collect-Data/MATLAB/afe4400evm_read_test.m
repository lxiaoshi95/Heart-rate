close all; 
tic
[R, IR, R_net, IR_net, R_ambient, IR_ambient] = afe4400evm_read(15000);
% figure
% Saturation = spo2(R, IR)
toc
figure; plot(R)
figure; plot(IR)
figure; plot(IR_net)
figure; plot(R_ambient)
figure; plot(IR_ambient)

fs = 500;
[bb, aa] = butter(3, [0.4 5]/(fs/2), 'bandpass');
R_1 = filter(bb, aa, R);
IR_1 = filter(bb, aa, IR);
t = (-4999: 10000) / fs;
figure
plot(t, R_1)
xlim([0 20])
figure
plot(t, IR_1)
xlim([0 20])