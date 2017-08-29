fileID = fopen('sruthi_DATA_.txt','r');
A = fscanf(fileID,'%f');


%-------------------------------------------------------Powerline------------------------------------------------------------------------------
len = 100;
fs = 250;
a = fir1(len,[0.37 0.43],'stop');
b = filter(a,1,A);

%--------------------------------------------------------Baseline-------------------------------------------------------------------------------

a1 = fft(b);
a1(1:100) = 0;
a1(end-100:end) = 0;
final = real(ifft(a1));

%--------------------------------------------------------PeakDetection---------------------------------------------------------------------------

max_value = max(final);
 
threshold = 0.37*max_value;
 
[pks,locs] = findpeaks(final,'minpeakheight',threshold); % locs gives the location of peaks in the input signal