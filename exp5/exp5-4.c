#include <stdio.h>
#include <limits.h>

int main(){
    int n;
    printf("Enter number of flights: ");
    scanf("%d",&n);

    int at[n], bt[n], pr[n], rt[n], ct[n], tat[n], wt[n];
    int completed = 0, time = 0;
    float total_wt = 0;

    for(int i=0;i<n;i++){
        printf("\nFlight %d Arrival Time: ",i+1);
        scanf("%d",&at[i]);
        printf("Landing Time: ");
        scanf("%d",&bt[i]);
        printf("Priority: ");
        scanf("%d",&pr[i]);
        rt[i] = bt[i];
    }

    printf("\nGantt Chart:\n");

    while(completed != n){
        int highest = -1, min_priority = INT_MAX;

        for(int i=0;i<n;i++){
            if(at[i] <= time && rt[i] > 0 && pr[i] < min_priority){
                min_priority = pr[i];
                highest = i;
            }
        }

        if(highest == -1){
            time++;
            continue;
        }

        printf("| F%d ",highest+1);
        rt[highest]--;
        time++;

        if(rt[highest] == 0){
            completed++;
            ct[highest] = time;
        }
    }

    printf("|\n");

    printf("\nFlight\tTAT\tWT\n");

    for(int i=0;i<n;i++){
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        total_wt += wt[i];

        printf("F%d\t%d\t%d\n",i+1,tat[i],wt[i]);
    }

    printf("\nAverage WT = %.2f\n",total_wt/n);

    return 0;
}