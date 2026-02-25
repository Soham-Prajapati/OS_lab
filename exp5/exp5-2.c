#include <stdio.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of patients: ");
    scanf("%d", &n);

    int at[n], bt[n], rt[n], ct[n], tat[n], wt[n];
    int completed = 0, time = 0;
    float total_wt = 0, total_tat = 0;

    for(int i=0;i<n;i++){
        printf("\nPatient %d Arrival Time: ",i+1);
        scanf("%d",&at[i]);
        printf("Treatment Time: ");
        scanf("%d",&bt[i]);
        rt[i] = bt[i];
    }

    printf("\nGantt Chart:\n");

    while(completed != n){
        int min = INT_MAX, shortest = -1;

        for(int i=0;i<n;i++){
            if(at[i] <= time && rt[i] > 0 && rt[i] < min){
                min = rt[i];
                shortest = i;
            }
        }

        if(shortest == -1){
            time++;
            continue;
        }

        printf("| P%d ",shortest+1);
        rt[shortest]--;
        time++;

        if(rt[shortest] == 0){
            completed++;
            ct[shortest] = time;
        }
    }

    printf("|\n");

    printf("\nPatient\tTAT\tWT\n");

    for(int i=0;i<n;i++){
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];

        total_wt += wt[i];
        total_tat += tat[i];

        printf("P%d\t%d\t%d\n",i+1,tat[i],wt[i]);
    }

    printf("\nAverage WT = %.2f", total_wt/n);
    printf("\nAverage TAT = %.2f\n", total_tat/n);

    return 0;
}