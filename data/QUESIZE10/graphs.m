%% Graphs for the multiple prod-cons mean waiting time measurements ,QUEUESIZE=10

data = importdata('data_original.csv');

figure(1);

plotYmax=1800000;
plotYmin=0;
xMax=inf;
% % %plotting Q=1
% dataToShow=data(:,1)==1 & data(:,3)<=plotYmax & data(:,3)>=plotYmin & data(:,2)<=xMax;
% plot(data(dataToShow,2), data(dataToShow,3),'-*');
% hold on;
% 
%plotting Q=2
dataToShow=data(:,1)==2 & data(:,3)<=plotYmax & data(:,3)>=plotYmin& data(:,2)<=xMax;
plot(data(dataToShow,2), data(dataToShow,3),'-*');
hold on;

%plotting Q=4
dataToShow=data(:,1)==4 & data(:,3)<=plotYmax & data(:,3)>=plotYmin& data(:,2)<=xMax;
plot(data(dataToShow,2), data(dataToShow,3),'-*');
hold on;
% %plotting Q=8
% dataToShow=data(:,1)==8 & data(:,3)<=plotYmax & data(:,3)>=plotYmin& data(:,2)<=xMax;
% plot(data(dataToShow,2), data(dataToShow,3),'-*');
% hold on;

%plotting Q=16
dataToShow=data(:,1)==16 & data(:,3)<=plotYmax & data(:,3)>=plotYmin& data(:,2)<=xMax;
plot(data(dataToShow ,2), data(dataToShow,3),'-*');
axis([-5 inf 0 inf])
hold on;
% 
% %plotting Q=32
dataToShow=data(:,1)==32 & data(:,3)<=plotYmax & data(:,3)>=plotYmin& data(:,2)<=xMax;
plot(data(dataToShow ,2), data(dataToShow ,3),'-*');

hold on;

% % plotting Q=64
% dataToShow=data(:,1)==64 & data(:,3)<=plotYmax & data(:,3)>=plotYmin& data(:,2)<=xMax;
% plot(data(dataToShow,2), data(dataToShow,3),'-*');
% hold on;

% legend("number of producers p=1", "number of producers p=2","number of producers p=4", ...
% "number of producers p=8","number of producers p=16","number of producers p=32",...
% "number of producers p=64");
legend("number of producers p=2","number of producers p=4", ...
"number of producers p=16","number of producers p=32");

title("Average mean waiting time of a function.(QUEUESIZE=10)");
xlabel("Number of consumer threads q");
ylabel("Average mean waiting time in microseconds (usec)");