#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *countdown(void *arg) {
    int seconds = *(int *)arg;
    for (int i = seconds; i > 0; i--) {
        printf("Time remaining: %d seconds\n", i);
        sleep(1);
    }
    printf("Timer finished!\n");
    pthread_exit(0);
}

int main() {
    pthread_t timer_thread;
    int seconds;
    printf("Enter countdown seconds: ");
    scanf("%d", &seconds);
    pthread_create(&timer_thread, NULL, countdown, &seconds);
    pthread_join(timer_thread, NULL);
    return 0;
}
