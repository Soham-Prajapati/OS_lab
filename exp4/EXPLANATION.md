# exp4-a.c Explanation

## Program Overview
This program demonstrates multithreading in C using the pthread library. It calculates the average, maximum, and median of a set of integers entered by the user, with each calculation performed in a separate thread.



# exp4-2.c Explanation

## Program Overview
This program demonstrates the use of threads to generate random numbers in C. The user specifies the number of iterations, and a thread prints random numbers for each iteration.

---

## Key Concepts
- **Threaded Execution:** The random number generation is performed in a separate thread.
- **Random Number Generation:** Uses `rand()` and `srand()` for randomness.
- **Passing Arguments to Threads:** The number of iterations is passed to the thread function.

---

## Program Flow
1. **User Input:**
   - The user is prompted to enter the number of iterations.
2. **Thread Creation:**
   - A thread is created to generate random numbers.
3. **Thread Function:**
   - `generate_random`: Prints a random number for each iteration.
4. **Thread Joining:**
   - The main thread waits for the random number thread to finish.
5. **Output:**
   - Random numbers are printed to the console.

---

## Important Functions & System Calls
- `pthread_create`: Starts a new thread.
- `pthread_join`: Waits for a thread to finish.
- `rand`, `srand`: Used for random number generation.
- `scanf`: Used for input.

---

## Sample Input & Output
**Input:**
```
Enter number of iterations: 5
```
**Output:**
```
Random Number: 42
Random Number: 17
Random Number: 88
Random Number: 3
Random Number: 56
```

---

## Viva Questions
1. **How do you pass arguments to a thread?**
   - By passing a pointer to the argument in `pthread_create`.
2. **Why use `srand(time(NULL))`?**
   - To seed the random number generator for different results each run.
3. **What is the purpose of `pthread_join`?**
   - To ensure the main thread waits for the random number thread to finish.
4. **What is the range of random numbers generated?**
   - 1 to 100 (inclusive).

---

## Troubleshooting & Common Errors
- **Incorrect Input:** Ensure a valid integer is entered for iterations.
- **Thread Synchronization:** No explicit synchronization is needed as only one thread is used for output.
- **Randomness:** If `srand` is not used, the sequence will be the same every run.
- **Shared Data:** The input array and result variables are shared among threads.

---

## Program Flow
1. **User Input:**
   - The user is prompted to enter numbers separated by spaces.
   - The input is parsed and converted to an integer array.

2. **Thread Creation:**
   - Three threads are created:
     - One to calculate the average
     - One to calculate the maximum
     - One to calculate the median

3. **Thread Functions:**
   - `calculate_average`: Computes the average of the array.
   - `calculate_maximum`: Finds the maximum value in the array.
   - `calculate_median`: Sorts the array and computes the median.

4. **Thread Joining:**
   - The main thread waits for all calculation threads to finish using `pthread_join`.

5. **Output:**
   - The results (average, maximum, median) are printed.

---

## Important Functions & System Calls
- `pthread_create`: Starts a new thread.
- `pthread_join`: Waits for a thread to finish.
- `qsort`: Sorts the array for median calculation.
- `fgets`, `strtok`, `atoi`: Used for input parsing.

---

## Sample Input & Output
**Input:**
```
Enter numbers separated by spaces: 5 2 8 1 9
```
**Output:**
```
Average = 5.00
Maximum = 9
Median = 5.00
```

---

## Viva Questions
1. **What is a thread?**
   - A thread is a lightweight process that shares memory with other threads in the same process.
2. **Why use threads?**
   - To perform tasks concurrently and utilize CPU resources efficiently.
3. **How is data shared between threads?**
   - Threads share global variables and heap memory.
4. **What is the purpose of `pthread_join`?**
   - It ensures the main thread waits for other threads to finish before proceeding.
5. **How is the median calculated?**
   - The array is sorted, and the middle value (or average of two middle values) is taken.

---

## Troubleshooting & Common Errors
- **Segmentation Fault:** May occur if memory allocation fails or pointers are misused.
- **Incorrect Input:** Ensure numbers are entered separated by spaces.
- **Thread Synchronization:** No explicit synchronization is needed here, as each thread writes to separate variables.

---

## Tips
- Always check return values of `pthread_create` and `pthread_join` for errors.
- Free dynamically allocated memory to avoid leaks.
- Use proper input validation for robust programs.
