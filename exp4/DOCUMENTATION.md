# Experiment 4 - Multithreading in C

## Name: Soham Prajapati
## UID no.: 2024308186
## Experiment No.: 4

---

## AIM:
Write programs to demonstrate multithreading in C using the pthread library, including concurrent calculation of statistics and random number generation.

---

## PROBLEM STATEMENT:


### Theory:

**Multithreading** is a technique where multiple threads (smaller units of a process) run concurrently within a single process, sharing memory and resources. Threads are lighter than processes and are ideal for parallel tasks, improving performance and CPU utilization.

**Thread vs Process:**
- A process is an independent program in execution, with its own memory space.
- A thread is a lightweight unit of execution within a process, sharing memory and resources with other threads.
- Threads are faster to create and switch between than processes.

**pthread Library:**
- The POSIX thread (pthread) library provides APIs for thread creation, synchronization, and management in C.
- Threads share global and heap memory, but each has its own stack.
- Common pthread functions: `pthread_create`, `pthread_join`, `pthread_exit`, `pthread_mutex`, etc.

**Advantages of Multithreading:**
- Efficient CPU usage
- Parallel execution of tasks
- Shared memory space (easy data sharing)
- Faster context switching than processes

**Challenges:**
- Synchronization issues (race conditions)
- Deadlocks if not managed properly
- Debugging can be complex

**Applications:**
- Real-time systems, servers, GUI applications, scientific computing

**Key Concepts:**
- Threads share global and heap memory
- Each thread has its own stack
- Threads can be created, joined, and synchronized
- Useful for parallel computation and efficient CPU utilization

### Theory of Thread:

Pthreads, or POSIX threads, serves as a standardized approach to thread creation and synchronization, gaining support across a spectrum of operating systems like Linux, macOS, and iOS. In the realm of C programming, Pthreads offer a pathway for crafting multiple threads within a singular process. This concurrent execution enhances the program's overall efficiency.

In single-threaded programs, a solitary flow of control prevails, executing tasks sequentially. This sequential execution becomes a bottleneck when a task consumes a considerable amount of time, forcing the program to patiently wait before moving on to the subsequent task. Conversely, the adoption of multi-threaded programs introduces a paradigm shift by enabling multiple tasks to unfold simultaneously in distinct threads. This approach fosters faster program completion times, as the CPU adeptly switches between diverse threads to maintain optimal utilization.

Within the C programming landscape, the Pthreads library becomes instrumental by providing an arsenal of functions dedicated to the creation and management of threads. For instance, the `pthread_create` function facilitates the birth of a new thread, while the `pthread_join` function allows for the orderly waiting of a thread's completion. Moreover, Pthreads offers synchronization mechanisms like mutexes and condition variables, acting as guardians to ensure secure access to shared data structures amidst the tumult of multiple threads.

However, treading into the domain of Pthreads in C necessitates vigilance regarding potential race conditions. These conditions arise when multiple threads concurrently access and modify shared data structures, often leading to unexpected outcomes. To avert such pitfalls, judicious use of proper synchronization mechanisms becomes imperative, ensuring a harmonious and error-free execution of multithreaded programs.

The pthreads library in C provides several important functions for creating, managing, and synchronizing threads. Some of the most commonly used functions are:

- `pthread_create`: Used to create a new thread. It takes as arguments the address of a pthread_t variable to store the thread identifier, a pointer to the thread's attributes (which can be set using `pthread_attr_init`), the address of the function that will run as the new thread, and an argument to pass to the thread function.
- `pthread_join`: Used to wait for a thread to complete. It takes as arguments the thread identifier and a pointer to a location where the exit status of the thread can be stored.
- `pthread_exit`: Used to terminate the calling thread. It takes as an argument a pointer to the exit status of the thread.
- `pthread_mutex_init`: Used to initialize a mutex, which is a synchronization primitive that can be used to protect access to shared data structures.
- `pthread_mutex_lock`: Used to lock a mutex, preventing other threads from accessing the protected data structure until the mutex is unlocked.
- `pthread_mutex_unlock`: Used to unlock a mutex, allowing other threads to access the protected data structure.
- `pthread_cond_init`: Used to initialize a condition variable, which is a synchronization primitive that can be used to signal between threads.
- `pthread_cond_wait`: Used to wait on a condition variable. It takes as arguments a pointer to a mutex that is associated with the condition variable, and a pointer to the condition variable.
- `pthread_cond_signal`: Used to signal a condition variable, waking up a thread that is waiting on it.

**Uses of Multithreading:**
- User Interface Responsiveness
- Web Servers
- Data Processing and Analysis
- Game Development
- Database Operations
- Network Applications
- Image and Video Processing
- Parallelizing Batch Processing

**Key Concepts:**
- Threads share global and heap memory
- Each thread has its own stack
- Threads can be created, joined, and synchronized
- Useful for parallel computation and efficient CPU utilization


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

### File 2: exp4-2.c (Random Number Generation)
### File 3: exp4-3.c (Countdown Timer)
```c
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
```

### File 4: exp4-4.c (Multithreaded Keyword Search)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct SearchArgs {
    const char *filename;
    const char *keyword;
    long start;
    long end;
    int count;
};

void *search_file(void *arg) {
    struct SearchArgs *args = (struct SearchArgs *)arg;
    FILE *file = fopen(args->filename, "r");
    if (!file) {
        perror("File open error");
        pthread_exit(0);
    }
    fseek(file, args->start, SEEK_SET);
    char line[1024];
    int local_count = 0;
    long pos = args->start;
    while (pos < args->end && fgets(line, sizeof(line), file)) {
        if (strstr(line, args->keyword)) local_count++;
        pos = ftell(file);
    }
    args->count = local_count;
    fclose(file);
    pthread_exit(0);
}

int main() {
    char filename[256], keyword[64];
    printf("Enter filename: ");
    scanf("%s", filename);
    printf("Enter keyword: ");
    scanf("%s", keyword);
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File open error");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fclose(file);
    long mid = filesize / 2;
    struct SearchArgs args1 = {filename, keyword, 0, mid, 0};
    struct SearchArgs args2 = {filename, keyword, mid, filesize, 0};
    pthread_t t1, t2;
    pthread_create(&t1, NULL, search_file, &args1);
    pthread_create(&t2, NULL, search_file, &args2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    int total = args1.count + args2.count;
    printf("Keyword '%s' found %d times in total.\n", keyword, total);
    printf("Thread 1 found %d, Thread 2 found %d.\n", args1.count, args2.count);
    return 0;
}
```
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
```
Enter number of iterations: 5
Random Number: 42
Random Number: 17
Random Number: 88
Random Number: 3
Random Number: 56
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
