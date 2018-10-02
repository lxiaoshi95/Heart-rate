function S = spo2(R, IR)
  T = 1:numel(R);
  fs = 500; % default. max is 2Ksps
  offset = 130; % Needs calibration
  ratio = -10; % Needs calibration

  subplot(4, 1, 1)
  plot(T, R, '-r', T, IR, '-b')

  % Equalization and normalization: get lower side evelope, low pass filter it.
  Rb  = sgolayfilt(R, 2, 1001);
  IRb = sgolayfilt(IR, 2, 1001);
  R  = R ./ Rb;
  IR = IR ./ IRb;

  subplot(4, 1, 2)
  plot(T, R, '-r')

%   [bb, aa]= butter(5, (10)/(500/2), 'low');
%   i_r = filter(bb, aa, IR);
  i_r = smooth(IR, 10);
  
  subplot(4, 1, 3)
  plot(T, IR, '-b')
  hold on
  plot(T, i_r, '-r')
  xlim([1001 10000])

  % Segment? or FFT?
  R  = R - mean(R);
  IR = IR - mean(IR);

  [PR, FR] = periodogram(R - mean(R), blackman(numel(R)), numel(R), fs);
  [PIR, FIR] = periodogram(IR - mean(IR), blackman(numel(IR)), numel(IR), fs);
  PR = 10 * log10(PR);
  PIR = 10 * log10(PIR);
  base = min([min(PR) min(PIR)]);
  PR = PR - base;
  PIR = PIR - base;

  pro = max([PR; PIR]) * 0.5;

  [PKPR, PKLR] = findpeaks(PR, 'MinPeakProminence', pro);
  [PKPIR, PKLIR] = findpeaks(PIR, 'MinPeakProminence', pro);

  subplot(4, 1, 4)
  plot(FR, PR, '-r', FR(PKLR), PKPR, 'or', FIR, PIR, '-b', FIR(PKLIR), PKPIR, 'ob')

  r = PKPR(1);
  ir = PKPIR(1);
  %S = offset + r / ir * ratio;
  %S = r / ir;
  S = r - ir;
end
