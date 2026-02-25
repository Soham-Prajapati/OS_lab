#include <stdio.h>

int main(){
    int n, tq;
    printf("Enter number of customers: ");
    scanf("%d",&n);

    int at[n], bt[n], rt[n], ct[n], tat[n], wt[n];
    int time = 0, completed = 0;
    float total_wt = 0, total_tat = 0;

    for(int i=0;i<n;i++){
        printf("\nCustomer %d Arrival Time: ",i+1);
        scanf("%d",&at[i]);
        printf("Service Time: ");
        scanf("%d",&bt[i]);
        rt[i] = bt[i];
    }

    printf("Enter Time Quantum: ");
    scanf("%d",&tq);

    printf("\nGantt Chart:\n");

    while(completed < n){
        for(int i=0;i<n;i++){
            if(rt[i] > 0 && at[i] <= time){
                printf("| C%d ",i+1);

                if(rt[i] > tq){
                    time += tq;
                    rt[i] -= tq;
                } else {
                    time += rt[i];
                    rt[i] = 0;
                    ct[i] = time;
                    completed++;
                }
            }
        }
    }

    printf("|\n");

    printf("\nCustomer\tTAT\tWT\n");

    for(int i=0;i<n;i++){
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];

        total_wt += wt[i];
        total_tat += tat[i];

        printf("C%d\t\t%d\t%d\n",i+1,tat[i],wt[i]);
    }

    printf("\nAverage WT = %.2f",total_wt/n);
    printf("\nAverage TAT = %.2f\n",total_tat/n);

    return 0;
}