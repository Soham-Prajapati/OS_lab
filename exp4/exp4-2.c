#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void *generate_random(void *arg){

    int iterations = *(int *)arg;
    int num = rand() % 100 + 1;
    printf("Thread %lu: Random Number: %d\n", (unsigned long)pthread_self(), num);

    pthread_exit(0);
}

int main(){

    pthread_t thread;
    int iterations;
    
    printf("Enter number of iterations: ");
    scanf("%d", &iterations);

    srand(time(NULL));
    for (int i = 0; i < iterations; i++){
        pthread_create(&thread, NULL, generate_random, &iterations);
    }
    pthread_join(thread, NULL);

    return 0;
}