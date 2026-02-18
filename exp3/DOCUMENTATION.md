# Experiment 3 - Process Management (Fork, Orphan, Zombie)

## Name: Soham Prajapati
## UID no.: 2024308186
## Experiment No.: 3

---

## AIM:
Write programs to demonstrate process creation using the `fork()` system call, and observe the behavior of orphan and zombie processes in Unix/Linux operating systems.

---

## PROBLEM STATEMENT:

### Theory:

**Process** is a program in execution. When a program is loaded into memory and becomes active, it becomes a process. A process has its own process ID (PID), memory space, and execution state.

**fork() System Call:**
The `fork()` system call is used to create a new process in Unix/Linux systems. It creates an exact copy of the calling process (parent process). The newly created process is called the child process.

**Key Characteristics of fork():**
- Returns 0 to the child process
- Returns child's PID to the parent process
- Returns -1 if fork fails
- Child process gets a copy of parent's data, heap, and stack (separate memory space)
- Both processes continue execution from the statement following fork()
- Both processes run concurrently

**Process States:**
1. **Running:** Process is currently being executed
2. **Ready:** Process is ready to execute but waiting for CPU
3. **Waiting/Blocked:** Process is waiting for some event (I/O, etc.)
4. **Terminated:** Process has completed execution

**Orphan Process:**
An orphan process is a process whose parent process has terminated or finished before the child process. When a parent process terminates, all its child processes become orphan processes. The operating system (init process with PID 1) adopts these orphan processes and becomes their new parent.

**Characteristics of Orphan Process:**
- Created when parent process terminates before child
- Adopted by init process (PID 1)
- Continue execution normally
- No resource wastage

**Zombie Process (Defunct Process):**
A zombie process is a process that has completed execution but still has an entry in the process table. This happens when a child process terminates but the parent process hasn't yet called `wait()` or `waitpid()` to read the child's exit status.

**Characteristics of Zombie Process:**
- Process has finished execution
- Exit status not yet collected by parent
- Occupies entry in process table
- Minimal system resources used (only process table entry)
- Cleaned up when parent calls `wait()` or when parent terminates

**Problem:** Create multiple programs to demonstrate:
1. Multiple process creation using fork() in a loop
2. Parent and child performing different computations
3. Independent memory spaces for parent and child
4. Sum calculation by parent and child processes
5. Orphan process creation and observation
6. Zombie process creation and observation

---

## Algorithm:

### For Program 1 (p1.c - Multiple Fork):
1. Start the program
2. Run a loop from i=0 to i<4
   - Call fork() in each iteration
3. After loop, each process (16 total) prints:
   - Its own PID using getpid()
   - Its parent's PID using getppid()
4. Exit

### For Program 2 (p2.c - Factorial and Sum of Digits):
1. Start the program
2. Initialize n = 5
3. Call fork() to create child process
4. Check return value of fork():
   - If pid == 0 (Child Process):
     * Initialize sum = 0, t = n
     * Extract each digit: sum += t%10, t = t/10
     * Print sum of digits
   - Else (Parent Process):
     * Initialize fact = 1
     * Loop from i=1 to n: fact = fact * i
     * Print factorial
5. Exit

### For Program 3 (p3.c - Variable Modification):
1. Start the program
2. Initialize x = 100
3. Call fork() to create child process
4. Check return value of fork():
   - If pid == 0 (Child Process):
     * x = x + 50
     * Print child's value of x
   - Else (Parent Process):
     * x = x - 20
     * Print parent's value of x
5. Demonstrates separate memory spaces
6. Exit

### For Program 4 (p4.c - Even and Odd Sum):
1. Start the program
2. Call fork() to create child process
3. Check return value of fork():
   - If pid == 0 (Child Process):
     * Initialize odd = 0
     * Loop from i=1 to 20 with step 2 (odd numbers)
     * Add each number to odd
     * Print odd sum
   - Else (Parent Process):
     * Initialize even = 0
     * Loop from i=2 to 20 with step 2 (even numbers)
     * Add each number to even
     * Print even sum
4. Exit

### For Program 5 (orphan.c - Orphan Process):
1. Start the program
2. Call fork() to create child process
3. Check return value of fork():
   - If fork() == 0 (Child Process):
     * Sleep for 5 seconds
     * Print child's PID and parent's PID
     * (Parent will be 1 after adoption)
   - Else (Parent Process):
     * Print "Parent exiting"
     * Exit immediately
4. Child becomes orphan and is adopted by init (PID 1)
5. Exit

### For Program 6 (zombie.c - Zombie Process):
1. Start the program
2. Call fork() to create child process
3. Check return value of fork():
   - If fork() == 0 (Child Process):
     * Print "Child done"
     * Exit immediately (becomes zombie)
   - Else (Parent Process):
     * Sleep for 10 seconds (without calling wait())
     * Print "Parent running without wait"
     * During sleep, child remains as zombie
4. Exit

---

## PROGRAMS

### File 1: `exp3/p1.c`
```c
#include <stdio.h>
#include <unistd.h>

int main() {
	for(int i=0;i<4;i++) {
		fork();
	}

	printf("PID: %d  Parent PID: %d\n", getpid(), getppid());
	return 0;
}
```

### File 2: `exp3/p2.c`
```c
#include <stdio.h>
#include <unistd.h>

int main() {
	int n = 5;
	int pid = fork();

	if(pid == 0) {
		int sum = 0, t = n;
		while(t>0){
			sum += t%10;
			t/=10;
		}
		printf("Child: Sum of digits = %d\n", sum);
	}
	else {
		int fact = 1;
		for(int i=1;i<=n;i++) fact*=i;
		printf("Parent: Factorial = %d\n", fact);
	}

	return 0;
}
```

### File 3: `exp3/p3.c`
```c
#include <stdio.h>
#include <unistd.h>

int main() {
	int x = 100;
	int pid = fork();

	if(pid==0) {
		x += 50;
		printf("Child x = %d\n", x);
	}
	else {
		x -= 20;
		printf("Parent x = %d\n", x);
	}

	return 0;
}
```

### File 4: `exp3/p4.c`
```c
#include <stdio.h>
#include <unistd.h>

int main() {
	int pid = fork();

	if(pid == 0) {
		int odd = 0;
		for(int i=1;i<=20;i+=2)
			odd += i;
		printf("Child Odd Sum = %d\n", odd);
	}
	else {
		int even = 0;
		for(int i=2;i<=20;i+=2)
			even += i;
		printf("Parent Even Sum = %d\n", even);
	}

	return 0;
}
```

### File 5: `exp3/orphan.c`
```c
#include <stdio.h>
#include <unistd.h>

int main() {
	if(fork()==0) {
		sleep(5);
		printf("Child PID=%d Parent=%d\n", getpid(), getppid());
	}
	else {
		printf("Parent exiting\n");
	}
}
```

### File 6: `exp3/zombie.c`
```c
#include <stdio.h>
#include <unistd.h>

int main() {
	if(fork()==0) {
		printf("Child done\n");
	}
	else {
		sleep(10);
		printf("Parent running without wait\n");
	}
}
```

---

## COMPILATION AND EXECUTION:

From the workspace root run the following commands to compile each program and execute them:

```bash
gcc -Wall -o exp3/p1 exp3/p1.c
gcc -Wall -o exp3/p2 exp3/p2.c
gcc -Wall -o exp3/p3 exp3/p3.c
gcc -Wall -o exp3/p4 exp3/p4.c
gcc -Wall -o exp3/orphan exp3/orphan.c
gcc -Wall -o exp3/zombie exp3/zombie.c

./exp3/p1
./exp3/p2
./exp3/p3
./exp3/p4
./exp3/orphan
./exp3/zombie
```

---

## OUTPUT:

### Program 1 (p1.c) - Multiple Fork Output:
```
PID: 32025  Parent PID: 32014
PID: 32014  Parent PID: 31460
PID: 32026  Parent PID: 32021
PID: 32021  Parent PID: 32014
PID: 32019  Parent PID: 32014
PID: 32024  Parent PID: 32020
PID: 32022  Parent PID: 32019
PID: 32023  Parent PID: 32018
PID: 32020  Parent PID: 32018
PID: 32018  Parent PID: 1
```

**Explanation:**
- The program forks 4 times in a loop, creating 2^4 = 16 processes total
- First fork creates 2 processes (original + 1 child)
- Second fork creates 4 processes (each of the 2 forks again)
- Third fork creates 8 processes
- Fourth fork creates 16 processes
- Each process prints its own PID and its parent's PID
- Some processes show parent PID as 1, indicating they were adopted by init (became orphans during execution)
- Output order is non-deterministic due to concurrent execution

### Program 2 (p2.c) - Factorial and Sum of Digits Output:
```
Parent: Factorial = 120
Child: Sum of digits = 5
```

**Explanation:**
- n = 5 is used for calculations
- Parent process calculates factorial: 5! = 5 × 4 × 3 × 2 × 1 = 120
- Child process calculates sum of digits: 5 has only one digit, so sum = 5
- Both processes execute concurrently and print their results
- The order of output may vary depending on process scheduling

### Program 3 (p3.c) - Variable Modification Output:
```
Parent x = 80
Child x = 150
```

**Explanation:**
- Initial value: x = 100
- After fork(), both parent and child have separate copies of x
- Parent modifies its copy: x = 100 - 20 = 80
- Child modifies its copy: x = 100 + 50 = 150
- Both print different values, proving they have independent memory spaces
- Changes in one process do not affect the other process
- This demonstrates the Copy-on-Write mechanism of fork()

### Program 4 (p4.c) - Even and Odd Sum Output:
```
Parent Even Sum = 110
Child Odd Sum = 100
```

**Explanation:**
- Parent calculates sum of even numbers from 2 to 20:
  * Even numbers: 2, 4, 6, 8, 10, 12, 14, 16, 18, 20
  * Sum = 2+4+6+8+10+12+14+16+18+20 = 110
- Child calculates sum of odd numbers from 1 to 19:
  * Odd numbers: 1, 3, 5, 7, 9, 11, 13, 15, 17, 19
  * Sum = 1+3+5+7+9+11+13+15+17+19 = 100
- Both processes work independently and print their results

### Program 5 (orphan.c) - Orphan Process Output:
```
Parent exiting
```

Note: After 5 seconds, child prints:
```
Child PID=<pid> Parent=1
```

**Explanation:**
- Parent process exits immediately after forking
- Child process sleeps for 5 seconds
- During sleep, child becomes an orphan (parent has terminated)
- Operating system (init process with PID 1) adopts the orphan child
- After waking up, child prints its PID and parent PID
- Parent PID is shown as 1, confirming adoption by init process
- This demonstrates automatic orphan process handling by the OS

### Program 6 (zombie.c) - Zombie Process Output:
```
Child done
Parent running without wait
```

**Explanation:**
- Child process prints "Child done" and exits immediately
- Parent process does not call wait() to collect child's exit status
- Child becomes a zombie process (terminated but entry remains in process table)
- During the 10-second sleep, child exists as a zombie
- You can verify zombie by running `ps aux | grep Z` in another terminal during execution
- Parent eventually prints "Parent running without wait"
- When parent terminates, zombie is cleaned up by init process
- This demonstrates the problem of not calling wait() after fork()

---

## CONCLUSION:

1. Successfully implemented six programs demonstrating various aspects of process management using the `fork()` system call in Unix/Linux operating systems.

2. Demonstrated that `fork()` creates an exact copy of the parent process, with the child getting a separate copy of all variables and memory space. Changes made by one process do not affect the other.

3. Observed that after `fork()`, both parent and child processes execute concurrently, and the order of execution is non-deterministic, depending on the OS scheduler.

4. **Key Observations about Process Creation:**
   - fork() creates 2^n processes when called n times in a loop
   - Parent receives child's PID, child receives 0 from fork()
   - Each process has independent memory space (Copy-on-Write)
   - Both processes continue from the statement after fork()

5. **Orphan Process Behavior:**
   - Successfully demonstrated orphan process creation by terminating parent before child
   - Confirmed that orphan processes are automatically adopted by init process (PID 1)
   - Orphan processes continue normal execution without any issues
   - No system resources are wasted by orphan processes

6. **Zombie Process Behavior:**
   - Successfully created zombie process by not calling wait() in parent
   - Zombie processes occupy an entry in process table but use minimal resources
   - Zombies are cleaned when parent calls wait() or when parent terminates
   - Too many zombie processes can fill up the process table

7. **Practical Applications:**
   - Understanding fork() is essential for creating multi-process applications
   - Process management is fundamental to server applications and daemon processes
   - Proper handling of child processes (using wait()) prevents zombie accumulation
   - Knowledge of orphan processes helps in understanding daemon process creation

8. All programs were successfully compiled and executed, demonstrating fundamental concepts of process creation, process states, and process relationships in Unix/Linux operating systems.

---

## REFERENCES

- Source files: [exp3/p1.c](exp3/p1.c), [exp3/p2.c](exp3/p2.c), [exp3/p3.c](exp3/p3.c), [exp3/p4.c](exp3/p4.c), [exp3/orphan.c](exp3/orphan.c), [exp3/zombie.c](exp3/zombie.c)

