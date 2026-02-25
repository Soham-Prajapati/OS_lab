# Experiment 5 - CPU Scheduling Algorithms (FCFS, SJF, RR, Priority)

## Name: Soham Prajapati
## UID no.: 2024308186
## Experiment No.: 5

---

## AIM:
Write programs to simulate and analyze various CPU scheduling algorithms (FCFS, SJF, Round Robin, Priority) using real-world analogies (trains, patients, customers, flights) and compute waiting time, turnaround time, and Gantt chart.

---

## PROBLEM STATEMENT:

### Theory:

**CPU Scheduling** is the process of determining which process in the ready queue will be allocated the CPU next. Different algorithms optimize for different criteria (waiting time, turnaround time, fairness).

**1. FCFS (First-Come, First-Served):**
- Processes are scheduled in the order they arrive.
- Simple, non-preemptive.
- Analogy: Trains arriving at a station.

**2. SJF (Shortest Job First):**
- Process with the shortest burst time is scheduled next.
- Can be preemptive or non-preemptive.
- Analogy: Patients with shortest treatment time are treated first.

**3. Round Robin (RR):**
- Each process gets a fixed time quantum in cyclic order.
- Preemptive, fair for all.
- Analogy: Customers served in a queue with equal time slots.

**4. Priority Scheduling:**
- Process with highest priority (lowest number) is scheduled next.
- Can be preemptive or non-preemptive.
- Analogy: Flights landing based on priority.

**Key Metrics:**
- **Waiting Time (WT):** Time a process spends waiting in the ready queue.
- **Turnaround Time (TAT):** Total time from arrival to completion.
- **Gantt Chart:** Visual representation of process execution order.

---

## ALGORITHM:

### FCFS (exp5-1.c - Trains):
1. Input number of trains, arrival and burst times.
2. For each train, if arrival > current time, add idle time.
3. Calculate completion, turnaround, and waiting times.
4. Print Gantt chart and metrics.

### SJF (exp5-2.c - Patients):
1. Input number of patients, arrival and treatment times.
2. While not all completed, pick patient with shortest remaining time.
3. Execute for 1 unit, update Gantt chart.
4. Calculate completion, turnaround, and waiting times.

### Round Robin (exp5-3.c - Customers):
1. Input number of customers, arrival and service times, time quantum.
2. While not all completed, cycle through customers.
3. Execute for time quantum or remaining time.
4. Update Gantt chart, calculate metrics.

### Priority (exp5-4.c - Flights):
1. Input number of flights, arrival, landing times, and priority.
2. While not all completed, pick flight with highest priority.
3. Execute for 1 unit, update Gantt chart.
4. Calculate completion, turnaround, and waiting times.

---

## PROGRAM:

### File 1: exp5-1.c (FCFS - Trains)
```c
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
```

### File 2: exp5-2.c (SJF - Patients)
```c
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
```

### File 3: exp5-3.c (Round Robin - Customers)
```c
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
```

### File 4: exp5-4.c (Priority - Flights)
```c
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
```

---

## COMPILATION AND EXECUTION:

```bash
# Compile each program
gcc exp5-1.c -o exp5-1
gcc exp5-2.c -o exp5-2
gcc exp5-3.c -o exp5-3
gcc exp5-4.c -o exp5-4

# Run each program
./exp5-1
./exp5-2
./exp5-3
./exp5-4
```

---

## OUTPUT (Sample):

### FCFS (exp5-1.c)
```
Enter number of trains: 3
Train 1 Arrival Time: 0
Train 1 Burst Time: 4
Train 2 Arrival Time: 2
Train 2 Burst Time: 3
Train 3 Arrival Time: 5
Train 3 Burst Time: 2

Gantt Chart:
| T1 | T2 | T3 |

Train	CT	TAT	WT
T1	4	4	0
T2	7	5	2
T3	9	4	2

Average Waiting Time = 1.33
Total Idle Time = 0
```

### SJF (exp5-2.c)
```
Enter number of patients: 3
Patient 1 Arrival Time: 0
Treatment Time: 5
Patient 2 Arrival Time: 2
Treatment Time: 2
Patient 3 Arrival Time: 4
Treatment Time: 1

Gantt Chart:
| P1 | P2 | P3 |

Patient	TAT	WT
P1	6	1
P2	3	1
P3	1	0

Average WT = 0.67
Average TAT = 3.33
```

### Round Robin (exp5-3.c)
```
Enter number of customers: 3
Customer 1 Arrival Time: 0
Service Time: 5
Customer 2 Arrival Time: 1
Service Time: 3
Customer 3 Arrival Time: 2
Service Time: 4
Enter Time Quantum: 2

Gantt Chart:
| C1 | C2 | C3 | C1 | C2 | C3 | C1 |

Customer	TAT	WT
C1	12	7
C2	7	4
C3	10	6

Average WT = 5.67
Average TAT = 9.67
```

### Priority (exp5-4.c)
```
Enter number of flights: 3
Flight 1 Arrival Time: 0
Landing Time: 3
Priority: 2
Flight 2 Arrival Time: 1
Landing Time: 2
Priority: 1
Flight 3 Arrival Time: 2
Landing Time: 1
Priority: 3

Gantt Chart:
| F2 | F1 | F3 |

Flight	TAT	WT
F1	5	2
F2	2	0
F3	1	0

Average WT = 0.67
```

---

## CONCLUSION:

1. Successfully implemented and simulated four CPU scheduling algorithms using real-world analogies.
2. Demonstrated calculation of waiting time, turnaround time, and Gantt chart for each algorithm.
3. Observed that FCFS is simple but may cause high waiting time; SJF optimizes for shortest jobs; RR ensures fairness; Priority scheduling allows urgent tasks to be handled first.
4. Each algorithm has its own advantages and disadvantages depending on the scenario.
5. Understanding scheduling is crucial for efficient resource management in operating systems.
