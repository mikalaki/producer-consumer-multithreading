%% Graphs and statistics for the multiple prod-cons waiting time measurements ,QUEUESIZE=1000

%% STATS calculations of all DATA
LOOP=10000;


i=0;
j=0;
standartDeviations=zeros(7,9);
minWaitingTime=zeros(7,9);
maxWaitingTime=zeros(7,9);
meanWaitingTime=zeros(7,9);
meanWaitingTime1stQuarter=zeros(7,9);
meanWaitingTime2ndQuarter=zeros(7,9);
meanWaitingTime3rdQuarter=zeros(7,9);
for p =[1 2 4  8  16 32 64 ] %7
   i=i+1;  
   j=0;
  for q =[ 1 2 4  8  16 32 64 128 256] %9
      j=j+1;
      %filename="allTimesP100_Q100.csv";
      filename=sprintf( "allTimesP%03d_Q%03d.csv", p,q);
      csvData=importdata(filename);
      standartDeviations(i,j)=std(csvData);
      minWaitingTime(i,j)=min(csvData);
      maxWaitingTime(i,j)=max(csvData);      
      meanWaitingTime1stQuarter(i,j)=mean(csvData(1: ((1*p*LOOP)/4)));
      meanWaitingTime2ndQuarter(i,j)=mean(csvData(1: ((2*p*LOOP)/4)));
      meanWaitingTime3rdQuarter(i,j)=mean(csvData(1: ((3*p*LOOP)/4)));
      meanWaitingTime(i,j)=mean(csvData);
      
  end 
end
meanError1=abs(meanWaitingTime-meanWaitingTime1stQuarter);
meanError2=abs(meanWaitingTime-meanWaitingTime2ndQuarter);
meanError3=abs(meanWaitingTime-meanWaitingTime3rdQuarter);


%% MEAN WAITING TIME Graph
hold off;
data = meanWaitingTime;

figure(4);

x=[1 2 4 8 16 32 64 128 256];
% % %plotting P=1
% semilogy(x,data(1,:),'-*');
% hold on;

%plotting P=2
semilogy(x,data(2,:),'-*');
hold on;

%plotting P=4
semilogy(x,data(3,:),'-*');
hold on;

% %plotting P=8
% semilogy(data(4,:),x,'-*');
% hold on;

%plotting P=16
semilogy(x,data(5,:),'-*');
hold on;


% %plotting P=32
semilogy(x,data(6,:),'-*');
hold on;

% % % plotting P=64
% semilogy(x,sdata(7,:),'-*');
% hold on;

% legend("number of producers p=1", "number of producers p=2","number of producers p=4", ...
% "number of producers p=8","number of producers p=16","number of producers p=32",...
% "number of producers p=64");
legend("number of producers p=2","number of producers p=4", ...
"number of producers p=16","number of producers p=32");

title("Mean waiting time of a function.(QUEUESIZE=1000)");
xlabel("Number of consumer threads q");
ylabel("Mean waiting time in microseconds (usec)");


%%MAX
%% MAX WAITING TIME Graph
hold off;
data = maxWaitingTime;

figure(5);

x=[1 2 4 8 16 32 64 128 256];
% % %plotting P=1
% plot(x,data(1,:),'-*');
% hold on;

%plotting P=2
plot(x,data(2,:),'-*');
hold on;

%plotting P=4
plot(x,data(3,:),'-*');
hold on;

% %plotting P=8
%plot(data(4,:),x,'-*');
% hold on;

%plotting P=16
plot(x,data(5,:),'-*');
hold on;


% %plotting P=32
plot(x,data(6,:),'-*');
hold on;

% % % plotting P=64
% plot(x,sdata(7,:),'-*');
% hold on;

% legend("number of producers p=1", "number of producers p=2","number of producers p=4", ...
% "number of producers p=8","number of producers p=16","number of producers p=32",...
% "number of producers p=64");
legend("number of producers p=2","number of producers p=4", ...
"number of producers p=16","number of producers p=32");

title("Max waiting time of a function.(QUEUESIZE=1000)");
xlabel("Number of consumer threads q");
ylabel("Max waiting time in microseconds (usec)");


%% MIN
%%MIN WAITING TIME Graph
hold off;
data = minWaitingTime;

figure(6);

x=[1 2 4 8 16 32 64 128 256];
% % %plotting P=1
% semilogy(x,data(1,:),'-*');
% hold on;

%plotting P=2
plot(x,data(2,:),'-*');
hold on;

%plotting P=4
plot(x,data(3,:),'-*');
hold on;

% %plotting P=8
% plot(data(4,:),x,'-*');
% hold on;

%plotting P=16
plot(x,data(5,:),'-*');
hold on;


% %plotting P=32
plot(x,data(6,:),'-*');
hold on;

% % % plotting P=64
% plot(x,sdata(7,:),'-*');
% hold on;

% legend("number of producers p=1", "number of producers p=2","number of producers p=4", ...
% "number of producers p=8","number of producers p=16","number of producers p=32",...
% "number of producers p=64");
legend("number of producers p=2","number of producers p=4", ...
"number of producers p=16","number of producers p=32");

title("Min waiting time of a function.(QUEUESIZE=1000)");
xlabel("Number of consumer threads q");
ylabel("Min waiting time in microseconds (usec)");

%% MEAN WAITING TIME Graph (3/4)
hold off;
data = meanWaitingTime3rdQuarter;

figure(7);

x=[1 2 4 8 16 32 64 128 256];
% % %plotting P=1
% plot(x,data(1,:),'-*');
% hold on;

%plotting P=2
semilogy(x,data(2,:),'-*');
hold on;

%plotting P=4
semilogy(x,data(3,:),'-*');
hold on;

% %plotting P=8
% plot(data(4,:),x,'-*');
% hold on;

%plotting P=16
semilogy(x,data(5,:),'-*');
hold on;


% %plotting P=32
semilogy(x,data(6,:),'-*');
hold on;

% % % plotting P=64
% plot(x,sdata(7,:),'-*');
% hold on;

% legend("number of producers p=1", "number of producers p=2","number of producers p=4", ...
% "number of producers p=8","number of producers p=16","number of producers p=32",...
% "number of producers p=64");
legend("number of producers p=2","number of producers p=4", ...
"number of producers p=16","number of producers p=32");

title("Mean waiting time for first 3/4 of executions (QUEUESIZE=1000)");
xlabel("Number of consumer threads q");
ylabel("Mean waiting time in microseconds (usec)");

