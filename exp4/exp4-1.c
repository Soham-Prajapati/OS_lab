#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int *arr;
int n;
double average;
int maximum;
double median;

void *calculate_average(void *arg){
    
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += arr[i];
    }

    average = (double)sum / n;
    pthread_exit(0);
}

void *calculate_maximum(void *arg){
    
    maximum = arr[0];
    
    for (int i = 1; i < n; i++){
        if (arr[i] > maximum){
            maximum = arr[i];
        }
    }

    pthread_exit(0);
}

int compare(const void *a, const void *b){
    return (*(int *)a - *(int *)b);
}

void *calculate_median(void *arg){
    
    qsort(arr, n, sizeof(int), compare);
    
    if (n % 2 == 0){
        median = (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
    } else {
        median = arr[n / 2];
    }

    pthread_exit(0);
}

int main(){
    
    int argc;
    char **argv;
    char input[1024];
    printf("Enter numbers separated by spaces: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    // Count numbers
    argc = 1;
    for (char *p = input; *p; p++) {
        if (*p == ' ' || *p == '\n') argc++;
    }

    argv = malloc(argc * sizeof(char *));
    argv[0] = "program";
    int idx = 1;
    char *token = strtok(input, " \n");
    while (token != NULL) {
        argv[idx++] = token;
        token = strtok(NULL, " \n");
    }
    

    if (argc < 2){
        printf("Usage: %s <numbers>\n", argv[0]);
        return 1;
    }
    
    n = argc - 1;
    arr = malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++){
        arr[i] = atoi(argv[i + 1]);
    }

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, calculate_average, NULL);
    pthread_create(&t2, NULL, calculate_maximum, NULL);
    pthread_create(&t3, NULL, calculate_median, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Average = %.2f\n", average);
    printf("Maximum = %d\n", maximum);
    printf("Median = %.2f\n", median);

    free(arr);

    return 0;
}