
clc
clear all

T_C=101;

simulation_time=200000; % in miliseconds
cummulative_tx_count=0;
cummulative_tx_oppertunity_count=0;

for N=2:100:1500% start of the number of vehicle loop
  % selection window sitch    
if (N<400)  
R_h=75;% maximum reselection counter value
R_l=25; % minimum reselection counter value
Gamma_value=20; % selection window size (ms)
elseif ((400<=N)&&(N<1000))    
R_h=30;% maximum reselection counter value
R_l=10; % minimum reselection counter value
Gamma_value=50; % selection window size (ms)
elseif ((1000<=N)&&(N<2000))    
R_h=15;% maximum reselection counter value
R_l=5; % minimum reselection counter value
Gamma_value=100; % selection window size (ms)
end



for f=1:5 % begining of averaging loop

Rate = .1;        % lambda   ( average events per unit time)
NumEvents = 300;     % number of events to generate

% HPD
InterEventTimes_H = exprnd(1/Rate,1,NumEvents);
EventTimes_H = cumsum(InterEventTimes_H);
Y_H=round(EventTimes_H*1000);
x_H=1;
for i=1:NumEvents
    
xStart_H = Y_H(i);
   T_H = 100; % HPD packet repetetion interval in ms
   k_H = 8;   % number of HPD repeat      
   x_H = [x_H,xStart_H + (0:k_H-1)*T_H];
end
x_H=x_H(2:end);



% DENM
InterEventTimes_D = exprnd(1/Rate,1,NumEvents);
EventTimes_D = cumsum(InterEventTimes_D);
Y_D=round(EventTimes_D*1000);
x_D=1;
for i=1:NumEvents
    
xStart_D = Y_D(i);
   T_D = 500; % DENM packet repetetion interval in ms
   k_D = 5; % number of DENM repeat
   x_D = [x_D,xStart_D + (0:k_D-1)*T_D];
end
x_D=x_D(2:end);

%CAM
EventTimes_C = 100:T_C:20000;
Y_C=EventTimes_C;

%MHD
InterEventTimes_M = exprnd(1/2,1,NumEvents);
EventTimes_M = cumsum(InterEventTimes_M);
Y_M=round(EventTimes_M*1000);
Y_M=Y_M(2:end);

Gamma=Gamma_value;
RC=round(R_l + (R_h-R_l).*rand(1,1)); % selecting the reselection counter
RC_old=RC;

% HPD
X_H = zeros(1,200000);
X_H(x_H)=1;
Z_H=X_H(1:200000);

% DENM
X_D = zeros(1,200000);
X_D(x_D)=1;
Z_D=X_D(1:200000);

%CAM
X_C = zeros(1,200000);
X_C(Y_C)=1;
Z_C=X_C(1:200000);

%MHD
X_M = zeros(1,200000);
X_M(Y_M)=1;
Z_M=X_M(1:200000);

t=1;
b_H=0;
b_D=0;
b_C=0;
b_M=0;
count_in_H=0;
count_in_H_1=0;
count_in_H_2=0;
count_in_H_3=0;
count_in_H_4=0;
count_in_H_5=0;

count_in_D=0;
count_in_D_1=0;
count_in_D_2=0;
count_in_D_3=0;
count_in_D_4=0;
count_in_D_5=0;


count_in_C=0;
count_in_C_1=0;
count_in_C_2=0;
count_in_C_3=0;
count_in_C_4=0;
count_in_C_5=0;

count_in_M=0;
count_in_M_1=0;
count_in_M_2=0;
count_in_M_3=0;
count_in_M_4=0;
count_in_M_5=0;


count_out_H=0;
count_out_D=0;
count_out_C=0;
count_out_M=0;

count_diff_H=0;
count_diff_D=0;
count_diff_C=0;
count_diff_M=0;

tx=0;
tx_oppertunity=0;


while t<simulation_time
    Gamma=Gamma-1;

   
    if Z_H(t)==1
             b_H=b_H+1;
            count_in_H=t;

    end
    
    if Z_D(t)==1
             b_D=b_D+1;
            count_in_D=t;

    end
    
    if Z_C(t)==1
             b_C=b_C+1;
            count_in_C=t;
    end
    
     if Z_M(t)==1
        switch b_M
        case 0
            b_M=b_M+1;
            count_in_M_1=t;
        case 1
            b_M=b_M+1;
            count_in_M_2=t;
        case 2
             b_M=b_M+1;
            count_in_M_3=t;
        case 3
             b_M=b_M+1;
            count_in_M_4=t;
        case 4
             b_M=b_M+1;
            count_in_M_5=t;
        end
    end
    
    if(Gamma==0 && RC>1 && b_H>0)
       
            tx=tx+1;
            b_H=b_H-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_H=t;
            count_diff_H(t)=count_out_H-count_in_H;
    end
    
    if(Gamma==0 && RC>1 && b_D>0 && b_H==0)
            tx=tx+1;
            b_D=b_D-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_D=t;

            count_diff_D(t)=count_out_D-count_in_D;

    end
    
     if(Gamma==0 && RC>1 && b_C>0 && b_H==0 && b_D==0)
            tx=tx+1;
            b_C=b_C-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_C=t;
            count_diff_C(t)=count_out_C-count_in_C;

     end
    
     if(Gamma==0 && RC>1 && b_M>0 && b_H==0 && b_D==0 && b_C==0)
        switch b_M
        case 5
            tx=tx+1;
            b_M=b_M-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_M=t;
            count_in_M=count_in_M_1;
            count_diff_M(t)=count_out_M-count_in_M;
            count_in_M_1=count_in_M_2;
            count_in_M_2=count_in_M_3;
            count_in_M_3=count_in_M_4;
            count_in_M_4=count_in_M_5;
        case 4
            tx=tx+1;
            b_M=b_M-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_M=t;
            count_in_M=count_in_M_1;
            count_diff_M(t)=count_out_M-count_in_M;
            count_in_M_1=count_in_M_2;
            count_in_M_2=count_in_M_3;
            count_in_M_3=count_in_M_4;
        case 3
            tx=tx+1;
            b_M=b_M-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_M=t;
            count_in_M=count_in_M_1;
            count_diff_M(t)=count_out_M-count_in_M;
            count_in_M_1=count_in_M_2;
            count_in_M_2=count_in_M_3;
        case 2
            tx=tx+1;
             b_M=b_M-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_M=t;
            count_in_M=count_in_M_1;
            count_diff_M(t)=count_out_M-count_in_M;
            count_in_M_1=count_in_M_2;
         case 1
             tx=tx+1;
             b_M=b_M-1;
            RC=RC-1;
            Gamma=Gamma_value;
            count_out_M=t;
            count_in_M=count_in_M_1;
            count_diff_M(t)=count_out_M-count_in_M;
        end
     end
    track_RC(t)=RC;
    
    if RC==1
        tx=tx+1;
        P_persis=0+ (1-0).*rand(1,1);
        resource_keep=0.4; % select a value between [0,0.8] according to standard
        RC=round(R_l+ (R_h-R_l).*rand(1,1)); % recalculate the RC value
    end
    if Gamma==0 % tx oppertunity
        Gamma=Gamma_value;
        tx_oppertunity=tx_oppertunity+1;
    end
    t=t+1;
    
    
end % end of time loop i.e., sumulation time of 200,000 ms 
tx_count(f)=tx;
tx_oppertunity_count(f)=tx_oppertunity;

mnz_H(f) = mean(nonzeros(count_diff_H));
mnz_D(f) = mean(nonzeros(count_diff_D));
mnz_C(f) = mean(nonzeros(count_diff_C));
mnz_M(f) = mean(nonzeros(count_diff_M));
end % end of  averaging loop

track_RC_with_N(N,:)=track_RC;


RC_count_sum=sum(track_RC_with_N==1);
bb=sum(RC_count_sum);
que=1;
Pi_1_0 =((bb/(N))/simulation_time);
x_t=1/Pi_1_0;
for w=0:Gamma_value-1
    que=que*(1-(1/(x_t-w)));
end
que;


average_tx_count(N)=mean(tx_count);
average_tx_oppertunity_count(N)=mean(tx_oppertunity_count);

average_H(N)=mean(nonzeros(mnz_H));
average_D(N)=mean(nonzeros(mnz_D));
average_C(N)=mean(nonzeros(mnz_C));

if N<400
    average_H(N)=mean(nonzeros(mnz_H));
    average_D(N)=mean(nonzeros(mnz_D));
    average_C(N)=mean(nonzeros(mnz_C));
    average_M(N)=mean(nonzeros(mnz_M));
elseif ((400<=N)&&(N<1000)) 
    average_H(N)=mean(nonzeros(mnz_H));
    average_D(N)=mean(nonzeros(mnz_D));
    average_C(N)=mean(nonzeros(mnz_C));
    average_M(N)=mean(nonzeros(mnz_M));
   
elseif((1000<=N)&&(N<2000))
    average_H(N)=mean(nonzeros(mnz_H));
    average_D(N)=mean(nonzeros(mnz_D));
    average_C(N)=mean(nonzeros(mnz_C));
    average_M(N)=mean(nonzeros(mnz_M));
end



cummulative_tx_count=cummulative_tx_count+average_tx_count(N);
cummulative_tx_oppertunity_count=cummulative_tx_oppertunity_count+average_tx_oppertunity_count(N);

    
      
        if N<400
        P_col(N)=(1-(1-((1-que)*(1-resource_keep)*(1/((Gamma_value*25)-N+1))))^(N-1))*10*83;
        elseif ((400<=N)&&(N<1000))    
        P_col(N)=(1-(1-((1-que)*(1-resource_keep)*(1/((Gamma_value*25)-N+1))))^(N-1))*10*83;
        elseif ((1000<=N)&&(N<2000))    
        P_col(N)=((1-(1-((1-que)*(1-resource_keep)*(1/((Gamma_value*25)-N+1))))^(N-1))*10)*83;
        end
             
            
        
        if N<400
        utilization(N) = (((average_tx_oppertunity_count(N)/simulation_time)*N*(1-P_col(N)))/25);
        elseif ((400<=N)&&(N<1000))    
        utilization(N) = (((average_tx_oppertunity_count(N)/simulation_time)*N*(1-P_col(N)))/25);
        elseif ((1000<=N)&&(N<2000))    
        utilization(N) = (((average_tx_oppertunity_count(N)/simulation_time)*N*(1-P_col(N)))/25);
        end
        
        throughput(N)=(utilization(N))*20e+6*1.1;

end % end of N loop

 isNZ1=(~average_H==0);
 isNZ2=(~average_D==0);
 isNZ3=(~average_C==0);
 isNZ4=(~average_M==0);
 isNZ5=(~P_col==0);
 isNZ6=(~utilization==0);
 isNZ7=(~throughput==0);
 
 N=1:1:1500;
 figure(1)
 title("Delay")
 hold on
 scatter(N(isNZ1),average_H(isNZ1),'+');
 hold on
 scatter(N(isNZ2),average_D(isNZ2),'o');
 hold on
 scatter(N(isNZ3),average_C(isNZ3),'*');
  hold on
 scatter(N(isNZ4),average_M(isNZ4),'x');

 
 figure(2)
 hold on
scatter(N(isNZ5),P_col(isNZ5),'square');


figure(3)
 hold on
scatter(N(isNZ6),utilization(isNZ6),'square');

figure(4)
 hold on
scatter(N(isNZ7),throughput(isNZ7),'square');

