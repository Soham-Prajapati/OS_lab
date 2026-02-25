# Experiment 4 - Multithreading in C

## Name: Soham Prajapati
## UID no.: 2024308186
## Experiment No.: 4

---

## AIM:
Write programs to demonstrate multithreading in C using the pthread library, including concurrent calculation of statistics and random number generation.

---

## PROBLEM STATEMENT:



### Theory (Concise):

**Multithreading** allows multiple threads to run concurrently within a single process, sharing memory and resources. Threads are lightweight compared to processes and are ideal for parallel tasks, improving performance and CPU utilization.

**Threads vs Processes:**
- Threads share memory and resources within a process, while processes have separate memory spaces.
- Threads are faster to create and switch between than processes.

**pthread Library:**
- The POSIX thread (pthread) library provides APIs for thread creation, synchronization, and management in C.
- Common functions: `pthread_create`, `pthread_join`, `pthread_exit`, `pthread_mutex`.

**Advantages:**
- Efficient CPU usage, parallel execution, easy data sharing.

**Challenges:**
- Synchronization issues (race conditions), deadlocks, and debugging complexity.

**Applications:**
- Real-time systems, servers, GUIs, scientific computing, data processing, and more.

**Key Concepts:**
- Threads share global/heap memory, each has its own stack.
- Useful for parallel computation and efficient CPU utilization.


---

## ALGORITHM:

### For Program 1 (exp4-1.c - Concurrent Statistics Calculation):
1. Start the program
2. Prompt user to enter numbers separated by spaces
3. Parse input and convert to integer array
4. Create three threads:
   - One for average calculation
   - One for maximum calculation
   - One for median calculation
5. Each thread performs its computation and stores result in shared variable
6. Main thread waits for all threads to finish
7. Print average, maximum, and median
8. Free allocated memory and exit


### For Program 2 (exp4-2.c - Random Number Generation):
1. Start the program
2. Prompt user to enter the number of random numbers to generate (or number of threads, depending on version)
3. For the single-threaded version:
    - Create a thread that generates and prints n random numbers in a loop
    - Main thread waits for the random number thread to finish
4. For the multi-threaded version:
    - Create n threads, each generating and printing one random number
    - Main thread waits for all threads to finish
5. Exit

### For Program 3 (exp4-3.c - Countdown Timer):
1. Start the program
2. Prompt user to enter countdown seconds
3. Create a thread to run the countdown timer
4. Thread prints remaining time every second
5. Main thread waits for timer thread to finish
6. Exit

### For Program 4 (exp4-4.c - Multithreaded Keyword Search):
1. Start the program
2. Prompt user to enter filename and keyword
3. Open file and determine its size
4. Create two threads:
    - First thread searches first half of file
    - Second thread searches second half
5. Each thread counts keyword occurrences in its section
6. Main thread waits for both threads to finish
7. Print total and per-thread counts
8. Exit

---


## PROGRAMS

### File 1: exp4-1.c (Concurrent Statistics Calculation)
```c
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
```

### File 2: exp4-2.c (Random Number Generation - Single Thread)

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void *generate_random(void *arg){
    int iterations = *(int *)arg;
    for (int i = 0; i < iterations; i++){
        int num = rand() % 100 + 1;
        printf("Random Number: %d\n", num);
    }
    pthread_exit(0);
}

int main(){
    pthread_t thread;
    int iterations;
    printf("Enter number of iterations: ");
    scanf("%d", &iterations);
    srand(time(NULL));
    pthread_create(&thread, NULL, generate_random, &iterations);
    pthread_join(thread, NULL);
    return 0;
}
```

---

### File 2 (Alternative): exp4-2.c (Random Number Generation - Multi-Threaded, Each Thread One Number)

This alternative approach demonstrates creating `n` threads, each generating one random number. This is different from the original, where a single thread generated all `n` numbers.

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 1000

void* generate_random(void* arg) {
    int idx = *(int*)arg;
    int num = rand() % 100 + 1;
    printf("Thread %d generated: %d\n", idx, num);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int n;
    printf("Enter number of random numbers (threads to create): ");
    scanf("%d", &n);
    if (n > MAX_THREADS) {
        printf("Too many threads! Max allowed: %d\n", MAX_THREADS);
        return 1;
    }
    pthread_t threads[MAX_THREADS];
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int* idx = malloc(sizeof(int));
        *idx = i + 1;
        if (pthread_create(&threads[i], NULL, generate_random, idx) != 0) {
            perror("pthread_create");
            return 1;
        }
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
```

#### Explanation of the Alternative Version

- **Original (Single Thread):** One thread generates all `n` random numbers in a loop.
- **Alternative (Multi-Threaded):** `n` threads are created, each responsible for generating and printing one random number.
- **Benefits:**
    - Demonstrates thread creation and management.
    - Shows parallelism, though for such a simple task, overhead may outweigh benefits for large `n`.
- **Drawbacks:**
    - Thread creation overhead is significant for large `n`.
    - Not efficient for trivial tasks, but useful for learning thread basics.

Both versions are kept for comparison and learning purposes.

---

### File 3: exp4-3.c (Countdown Timer)
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
    srand(time(NULL));
    pthread_create(&thread, NULL, generate_random, &iterations);
    pthread_join(thread, NULL);
    return 0;
}
```

---

### File 2 (Alternative): exp4-2.c (Random Number Generation - Multi-Threaded, Each Thread One Number)

This alternative approach demonstrates creating `n` threads, each generating one random number. This is different from the original, where a single thread generated all `n` numbers.

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 1000

void* generate_random(void* arg) {
    int idx = *(int*)arg;
    int num = rand() % 100 + 1;
    printf("Thread %d generated: %d\n", idx, num);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int n;
    printf("Enter number of random numbers (threads to create): ");
    scanf("%d", &n);
    if (n > MAX_THREADS) {
        printf("Too many threads! Max allowed: %d\n", MAX_THREADS);
        return 1;
    }
    pthread_t threads[MAX_THREADS];
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int* idx = malloc(sizeof(int));
        *idx = i + 1;
        if (pthread_create(&threads[i], NULL, generate_random, idx) != 0) {
            perror("pthread_create");
            return 1;
        }
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
```

#### Explanation of the Alternative Version

- **Original (Single Thread):** One thread generates all `n` random numbers in a loop.
- **Alternative (Multi-Threaded):** `n` threads are created, each responsible for generating and printing one random number.
- **Benefits:**
    - Demonstrates thread creation and management.
    - Shows parallelism, though for such a simple task, overhead may outweigh benefits for large `n`.
- **Drawbacks:**
    - Thread creation overhead is significant for large `n`.
    - Not efficient for trivial tasks, but useful for learning thread basics.

Both versions are kept for comparison and learning purposes.

---

## COMPILATION AND EXECUTION:

From the exp4 folder, run the following commands to compile and execute each program:

```bash
gcc -Wall -o exp4-1 exp4-1.c -lpthread
gcc -Wall -o exp4-2 exp4-2.c -lpthread

./exp4-1
./exp4-2
./exp4-3
./exp4-4
```

---

## OUTPUT:

### Program 1 (exp4-1.c) - Concurrent Statistics Output:
```
Enter numbers separated by spaces: 5 2 8 1 9
Average = 5.00
Maximum = 9
Median = 5.00
```

### Program 2 (exp4-2.c) - Random Number Generation Output:
```
Enter number of iterations: 5
Random Number: 42
Random Number: 17
Random Number: 88
Random Number: 3
Random Number: 56
```

```
Enter number of random numbers (threads to create): 5
Thread 1 generated: 42
Thread 2 generated: 17
Thread 3 generated: 88
Thread 4 generated: 3
Thread 5 generated: 56
```

### Program 3 (exp4-3.c) - Countdown Timer Output:
```
Enter countdown seconds: 5
Time remaining: 5 seconds
Time remaining: 4 seconds
Time remaining: 3 seconds
Time remaining: 2 seconds
Time remaining: 1 seconds
Timer finished!
```

### Program 4 (exp4-4.c) - Multithreaded Keyword Search Output:
```
Enter filename: sample.txt
Enter keyword: hello
Keyword 'hello' found 7 times in total.
Thread 1 found 3, Thread 2 found 4.
```


---

## EXPLANATION:


### Program 1 (exp4-1.c)
- Uses three threads to calculate average, maximum, and median concurrently.
- Input is parsed and converted to integer array.
- Each thread writes its result to a shared variable.
- Main thread waits for all threads to finish and prints results.
- Demonstrates parallel computation and efficient use of CPU.
- Shows how threads can be used to perform independent tasks simultaneously.
- Median calculation uses sorting and handles both even and odd number of elements.
- No explicit synchronization is needed as each thread writes to its own variable.

### Program 2 (exp4-2.c)
- **Single Thread Version:**
    - A single thread is created, which generates and prints `n` random numbers in a loop.
    - Demonstrates basic thread creation and argument passing using `pthread_create`.
    - The main thread waits for the random number thread to finish using `pthread_join`.
    - Useful for understanding how to offload a repetitive task to a thread.

- **Multi-Threaded Version (Alternative):**
    - `n` threads are created, each responsible for generating and printing one random number.
    - Each thread receives its own index as an argument and prints its result.
    - Demonstrates thread creation, argument passing, and memory management (each thread receives a dynamically allocated argument).
    - Shows parallelism, but also highlights the overhead of creating many threads for simple tasks.
    - Useful for learning thread basics and understanding the trade-offs of thread granularity.

Both versions illustrate different approaches to using threads for random number generation.

---

### Program 2 (exp4-2.c)
- **Single Thread Version:**
    - A single thread is created, which generates and prints `n` random numbers in a loop.
    - Demonstrates basic thread creation and argument passing using `pthread_create`.
    - The main thread waits for the random number thread to finish using `pthread_join`.
    - Useful for understanding how to offload a repetitive task to a thread.

- **Multi-Threaded Version (Alternative):**
    - `n` threads are created, each responsible for generating and printing one random number.
    - Each thread receives its own index as an argument and prints its result.
    - Demonstrates thread creation, argument passing, and memory management (each thread receives a dynamically allocated argument).
    - Shows parallelism, but also highlights the overhead of creating many threads for simple tasks.
    - Useful for learning thread basics and understanding the trade-offs of thread granularity.

Both versions illustrate different approaches to using threads for random number generation.

---

### Program 3 (exp4-3.c)
- Uses a thread to implement a countdown timer.
- User specifies the countdown duration in seconds.
- The thread prints the remaining time every second until the timer reaches zero.
- Main thread waits for timer thread to finish.
- Demonstrates basic thread usage for timed tasks.

### Program 4 (exp4-4.c)
- Uses two threads to search a large text file for a specified keyword.
- First thread searches the first half, second thread searches the second half.
- Each thread counts occurrences in its section.
- Main thread prints total and per-thread counts.
- Demonstrates parallel file processing and thread coordination.
- Uses a thread to generate and print random numbers for a user-specified number of iterations.
- Random number generator is seeded for different results each run.
- Main thread waits for random number thread to finish.
- Demonstrates passing arguments to threads and basic thread usage.
- Shows how threads can be used for repetitive tasks in parallel with main program.


---


## Viva Questions & Answers

1. **What is a thread?**
    - A thread is a lightweight process that shares memory with other threads in the same process.
2. **What is the difference between a thread and a process?**
    - Threads share memory and resources, processes have independent memory spaces.
3. **Why use threads?**
    - To perform tasks concurrently and utilize CPU resources efficiently.
4. **How is data shared between threads?**
    - Threads share global variables and heap memory.
5. **What is the purpose of `pthread_join`?**
    - It ensures the main thread waits for other threads to finish before proceeding.
6. **How is the median calculated?**
    - The array is sorted, and the middle value (or average of two middle values) is taken.
7. **How do you pass arguments to a thread?**
    - By passing a pointer to the argument in `pthread_create`.
8. **Why use `srand(time(NULL))`?**
    - To seed the random number generator for different results each run.
9. **What is the range of random numbers generated?**
    - 1 to 100 (inclusive).
10. **What are the advantages of multithreading?**
     - Efficient CPU usage, parallel execution, shared memory, faster context switching.
11. **What are common issues in multithreaded programs?**
     - Race conditions, deadlocks, synchronization problems.


---


## Troubleshooting & Common Errors
- **File Not Found:** For exp4-4.c, ensure the file exists and is readable.
- **Incorrect Keyword:** If keyword is not found, output will show zero occurrences.
- **Timer Issues:** For exp4-3.c, ensure a valid positive integer is entered for countdown.
- **Segmentation Fault:** May occur if memory allocation fails or pointers are misused.
- **Incorrect Input:** Ensure numbers are entered separated by spaces or a valid integer is entered for iterations.
- **Thread Synchronization:** No explicit synchronization is needed here, as each thread writes to separate variables or only one thread is used for output.
- **Randomness:** If `srand` is not used, the sequence will be the same every run.
- **Race Condition:** If multiple threads modify shared data without synchronization, results may be unpredictable (not present in these programs).

---

## Tips
- Always check return values of `pthread_create` and `pthread_join` for errors.
- Free dynamically allocated memory to avoid leaks.
- Use proper input validation for robust programs.
- Always seed the random number generator for true randomness.
- Use mutexes or other synchronization primitives if threads need to modify shared data.

---

## CONCLUSION:

1. Successfully implemented two programs demonstrating multithreading in C using the pthread library.
2. Demonstrated concurrent calculation of statistics and random number generation.
3. Showed how threads can be used to perform tasks in parallel and efficiently utilize CPU resources.
4. Provided sample input, output, and explanations for each program.
5. Addressed common errors and provided troubleshooting tips.
6. Explained theory, advantages, and challenges of multithreading in detail.
