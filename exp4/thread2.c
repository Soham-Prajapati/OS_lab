#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

            int run(void *arg){
    int i = *(int *)arg; // Copy the arg
    free(arg);           // Done with this
    printf("THREAD %d: running!\n", i);
    return i;
}

#define THREAD_COUNT 5

int main(void){
    thrd_t t[THREAD_COUNT];
    int i;
    printf("Launching threads...\n");

    for (i = 0; i < THREAD_COUNT; i++){
        // Get some space for a per-thread argument:
        int *arg = malloc(sizeof *arg);
        *arg = i;
        thrd_create(t + i, run, arg);
    }
}