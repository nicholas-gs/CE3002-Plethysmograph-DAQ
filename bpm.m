clear all
clc
close all
% analog input to A0
comPort='COM4';
s=serial(comPort);
set(s,'DataBits',8);
set(s,'StopBits',1);
set(s,'BaudRate',9600);
set(s,'Parity','none');
fopen(s)
a=1;
n=150;

for l=1:20
    a=1;
    clear sig
c1=clock;
c1=fix(c1)
tic;
while(a<=n)
    temp=str2num(fscanf(s));
 if temp>=0
    sig(a)=temp/1024*5;
    a=a+1;
 end
end
c2=clock;
c2=fix(c2)
tp=toc;
sig=smooth(sig, 0.05);
t=(1:length(sig))*tp/n;
plot(t,sig);
xlabel(['Time period = ' num2str(tp) 'seconds'])
[pks,locs]=findpeaks(sig,'MinPeakDistance',5);
findpeaks(sig,t)
text(locs+.02,pks,num2str((1:numel(pks))'))
pause(0.01);
avgptp=mean(diff(locs));
bpm=60/((tp(end)/n)*avgptp);

bpm
tp
diff(locs)
avgptp

value='http://blynk-cloud.com/iv1Zbdgke4ZCgovZK6L8VosXSHQCYxMF/update/V1?value=';
led='http://blynk-cloud.com/iv1Zbdgke4ZCgovZK6L8VosXSHQCYxMF/update/V0?value=';

heartRate=sprintf("%s%d", value, bpm);
ledOff=sprintf("%s%d", led, 0);
ledOn=sprintf("%s%d", led, 1);
respond=webread(heartRate);
if (bpm>=60) && (bpm<=100)
    respond=webread(ledOff);
else
    respond=webread(ledOn);
end

end

fclose(s);
delete(s)
disp('Done')