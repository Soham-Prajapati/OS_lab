#include <stdio.h>

int main() {
    int n;
    printf("Enter number of trains: ");
    scanf("%d", &n);

    int at[n], bt[n], ct[n], tat[n], wt[n];
    int total_wt = 0, total_idle = 0;
    int current_time = 0;

    for(int i = 0; i < n; i++) {
        printf("\nTrain %d Arrival Time: ", i+1);
        scanf("%d", &at[i]);
        printf("Train %d Burst Time: ", i+1);
        scanf("%d", &bt[i]);
    }

    printf("\nGantt Chart:\n");

    for(int i = 0; i < n; i++) {

        if(at[i] > current_time) {
            total_idle += at[i] - current_time;
            current_time = at[i];
        }

        printf("| T%d ", i+1);

        ct[i] = current_time + bt[i];
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];

        total_wt += wt[i];
        current_time = ct[i];
    }

    printf("|\n");

    printf("\nTrain\tCT\tTAT\tWT\n");
    for(int i = 0; i < n; i++) {
        printf("T%d\t%d\t%d\t%d\n", i+1, ct[i], tat[i], wt[i]);
    }

    printf("\nAverage Waiting Time = %.2f", (float)total_wt/n);
    printf("\nTotal Idle Time = %d\n", total_idle);

    return 0;
}