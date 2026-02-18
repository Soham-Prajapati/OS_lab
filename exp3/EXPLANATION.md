# Experiment 3 - Detailed Explanation & Viva Questions

## Name: Soham Prajapati
## UID no.: 2024308186

---

## TABLE OF CONTENTS
1. [Core Concepts Explained](#core-concepts-explained)
2. [Program-wise Detailed Explanation](#program-wise-detailed-explanation)
3. [Common Viva Questions & Answers](#common-viva-questions--answers)
4. [Important System Calls](#important-system-calls)
5. [Process States and Transitions](#process-states-and-transitions)
6. [Troubleshooting & Common Errors](#troubleshooting--common-errors)

---

## CORE CONCEPTS EXPLAINED

### What is fork()?

`fork()` is a system call that creates a new process by duplicating the calling process. The new process is called the **child process**, and the calling process is called the **parent process**.

**How fork() works internally:**
1. Operating system creates a new process control block (PCB) for the child
2. Child gets a copy of parent's:
   - Code segment (shared, read-only)
   - Data segment (copy-on-write)
   - Stack segment (separate copy)
   - Heap segment (copy-on-write)
3. Child gets a unique Process ID (PID)
4. Child's Parent PID (PPID) is set to parent's PID
5. Both processes resume execution from the line after fork()

**Copy-on-Write (COW):**
- Initially, parent and child share the same memory pages
- When either process modifies a page, OS creates a separate copy
- This makes fork() efficient (no immediate copying)

**Return Values:**
- **In Parent:** fork() returns the child's PID (positive number)
- **In Child:** fork() returns 0
- **On Error:** fork() returns -1

### What are Orphan Processes?

**Definition:** An orphan process is a child process whose parent has terminated.

**What happens to orphans?**
- The init process (PID 1) adopts all orphan processes
- init becomes the new parent (PPID becomes 1)
- Orphan continues normal execution
- When orphan terminates, init collects its exit status

**Why orphans are created:**
- Parent exits before child completes
- Parent crashes or is killed
- Intentional (for creating daemon processes)

**Are orphans harmful?** 
No! Orphans are handled automatically by the OS and continue running normally.

### What are Zombie Processes?

**Definition:** A zombie (defunct) process is a process that has completed execution but still has an entry in the process table.

**How zombies are created:**
1. Child process calls exit()
2. Child's state changes to ZOMBIE
3. Parent hasn't called wait() yet to read exit status
4. Child's entry remains in process table
5. Child occupies a slot but uses no other resources

**Zombie Process State:**
- Process has terminated
- Code, data, stack all deallocated
- Only PCB (Process Control Block) remains
- Waiting for parent to collect exit status

**How to remove zombies:**
- Parent calls wait() or waitpid()
- Parent terminates (init adopts and cleans zombies)
- System reboot (not recommended!)

**Why zombies are a problem:**
- Each zombie occupies a process table entry
- Too many zombies can fill process table
- No new processes can be created if table is full
- Usually indicates poor programming (forgot wait())

---

## PROGRAM-WISE DETAILED EXPLANATION

### Program 1: p1.c - Multiple Fork Demonstration

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

**Line-by-line Explanation:**

- **Line 1-2:** Include necessary header files
  - `stdio.h` for printf()
  - `unistd.h` for fork(), getpid(), getppid()

- **Line 5-7:** Loop that calls fork() 4 times
  - **Iteration 1:** 1 process forks → 2 processes total
  - **Iteration 2:** 2 processes fork → 4 processes total
  - **Iteration 3:** 4 processes fork → 8 processes total
  - **Iteration 4:** 8 processes fork → 16 processes total

- **Line 8:** All 16 processes print their PID and PPID
  - getpid() returns current process's PID
  - getppid() returns parent's PID

**Process Tree:**
```
                    Original Process
                    /              \
                   P1              P2
                  /  \            /  \
                P3    P4        P5    P6
               / \   / \       / \   / \
              P7 P8 P9 P10   P11 P12 P13 P14
```

**Key Points:**
- Total processes = 2^n where n = number of forks
- Here n=4, so 2^4 = 16 processes
- All processes execute concurrently
- Output order is unpredictable

---

### Program 2: p2.c - Factorial and Sum of Digits

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int n = 5;
    int pid = fork();
    
    if(pid == 0) {
        // Child Process
        int sum = 0, t = n;
        while(t>0){
            sum += t%10;
            t/=10;
        }
        printf("Child: Sum of digits = %d\n", sum);
    }
    else {
        // Parent Process
        int fact = 1;
        for(int i=1;i<=n;i++) fact*=i;
        printf("Parent: Factorial = %d\n", fact);
    }
    return 0;
}
```

**Line-by-line Explanation:**

- **Line 5:** n = 5 is the number for calculations

- **Line 6:** fork() is called
  - Returns 0 to child
  - Returns child's PID to parent

- **Lines 8-15:** Child process code (pid == 0)
  - Sum of digits algorithm:
    * Extract last digit: t%10
    * Add to sum
    * Remove last digit: t/=10
    * Repeat until t becomes 0
  - For n=5: only one digit, sum = 5

- **Lines 16-21:** Parent process code (pid > 0)
  - Factorial algorithm:
    * fact = 1 × 2 × 3 × 4 × 5 = 120
  - Simple loop multiplication

**Why pid variable?**
- Storing fork() return helps distinguish parent/child
- Child: pid == 0
- Parent: pid == child's PID (positive)
- Error: pid == -1

**Important:** Both processes run simultaneously!

---

### Program 3: p3.c - Variable Independence

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

**Line-by-line Explanation:**

- **Line 5:** x = 100 initialized BEFORE fork()
  - Both parent and child initially have x = 100

- **Line 6:** fork() creates child
  - Parent gets copy of x
  - Child gets separate copy of x

- **Lines 8-10:** Child modifies its copy
  - Child's x = 100 + 50 = 150
  - Parent's x is NOT affected

- **Lines 11-14:** Parent modifies its copy
  - Parent's x = 100 - 20 = 80
  - Child's x is NOT affected

**Key Concept: Copy-on-Write**
- Initially both share same memory page
- When child modifies x, OS creates new page for child
- When parent modifies x, each has separate memory
- This proves processes have independent memory spaces

**Why different values?**
- After fork(), each process has its own address space
- Variables are copied, not shared
- Changes in one don't affect the other

---

### Program 4: p4.c - Even and Odd Sum

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

**Line-by-line Explanation:**

- **Line 5:** fork() called immediately

- **Lines 7-11:** Child calculates odd sum
  - Loop: i = 1, 3, 5, 7, 9, 11, 13, 15, 17, 19
  - i+=2 increments by 2 (gets next odd number)
  - Sum = 1+3+5+7+9+11+13+15+17+19 = 100

- **Lines 12-17:** Parent calculates even sum
  - Loop: i = 2, 4, 6, 8, 10, 12, 14, 16, 18, 20
  - i+=2 increments by 2 (gets next even number)
  - Sum = 2+4+6+8+10+12+14+16+18+20 = 110

**Formula for sum:**
- Sum of first n odd numbers = n²
  * Here: 10 odd numbers, sum = 10² = 100 ✓
- Sum of first n even numbers = n(n+1)
  * Here: 10 even numbers, sum = 10×11 = 110 ✓

**Parallel Execution:**
- Both loops run simultaneously
- CPU scheduler switches between them
- Results printed independently

---

### Program 5: orphan.c - Orphan Process Creation

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

**Line-by-line Explanation:**

- **Line 5:** fork() called in if condition
  - If result is 0, we're in child
  - If result > 0, we're in parent

- **Lines 5-7:** Child process
  - sleep(5) pauses execution for 5 seconds
  - During this time, parent will exit
  - After waking, prints PID and PPID
  - PPID will be 1 (init process)

- **Lines 8-10:** Parent process
  - Prints "Parent exiting"
  - Exits immediately (no wait() call)
  - Child becomes orphan

**Timeline:**
```
Time 0s: fork() creates child
Time 0s: Parent prints "Parent exiting" and exits
Time 0s-5s: Child is sleeping, now an orphan
Time 0s: init (PID 1) adopts child
Time 5s: Child wakes up
Time 5s: Child prints, showing Parent=1
Time 5s: Child exits, init collects its status
```

**Why getppid() returns 1?**
- Parent exited
- OS automatically assigns init as new parent
- init's PID is always 1
- This is automatic orphan handling

**Note:** No return statement in parent code block, but main() exits anyway.

---

### Program 6: zombie.c - Zombie Process Creation

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

**Line-by-line Explanation:**

- **Lines 5-6:** Child process
  - Prints "Child done"
  - Exits immediately (reaches end of if block)
  - Becomes ZOMBIE (waiting for parent to collect status)

- **Lines 7-10:** Parent process
  - sleep(10) for 10 seconds
  - Does NOT call wait() or waitpid()
  - Child remains zombie during this sleep
  - After 10 seconds, prints message

**Timeline:**
```
Time 0s: fork() creates child
Time 0s: Child prints "Child done" and exits
Time 0s: Child becomes ZOMBIE (no wait() from parent)
Time 0s-10s: Child is ZOMBIE in process table
Time 0s-10s: Parent is sleeping
Time 10s: Parent wakes and prints
Time 10s: Parent exits
Time 10s: init adopts zombie and cleans it up
```

**How to verify zombie?**
Run this in another terminal while program is sleeping:
```bash
ps aux | grep Z
ps aux | grep defunct
```

**Process Table Entry:**
```
PID    STATE   COMMAND
12345  Z       [zombie] <defunct>
```

**What's wrong with this program?**
- Parent should call wait() before sleeping
- Proper code:
```c
if(fork()==0) {
    printf("Child done\n");
} else {
    wait(NULL);  // This prevents zombie!
    printf("Parent collected child status\n");
}
```

---

## COMMON VIVA QUESTIONS & ANSWERS

### Q1: What is fork() and what does it return?

**Answer:** 
fork() is a system call used to create a new process. It creates an exact duplicate of the calling process.

**Return values:**
- Returns 0 to the child process
- Returns child's PID (positive integer) to parent
- Returns -1 on error (e.g., insufficient memory, too many processes)

### Q2: What is the difference between fork() and exec()?

**Answer:**
- **fork():** Creates a NEW process that is a copy of the parent. Both parent and child execute the same code.
- **exec():** Replaces the current process's memory with a NEW program. Process ID remains same but code changes.

**Example:**
```c
// fork example
if(fork() == 0) {
    // child runs same program
}

// exec example
execl("/bin/ls", "ls", NULL);  // replaces current process with 'ls'
```

### Q3: Why do we check if(pid == 0) after fork()?

**Answer:**
We check the return value of fork() to determine which process (parent or child) is currently executing.

- **pid == 0:** We are in the child process
- **pid > 0:** We are in the parent process (pid contains child's PID)
- **pid < 0:** Error occurred, fork failed

This allows us to write different code for parent and child.

### Q4: What is getpid() and getppid()?

**Answer:**
- **getpid():** Returns the process ID of the CURRENT process
- **getppid():** Returns the Parent Process ID of the current process

**Example:**
```c
printf("My PID: %d\n", getpid());      // Current process
printf("My Parent: %d\n", getppid());  // Parent's PID
```

### Q5: What happens if parent terminates before child?

**Answer:**
The child becomes an **orphan process**. The operating system automatically assigns the init process (PID 1) as the new parent. The orphan continues execution normally and when it terminates, init collects its exit status.

### Q6: What happens if child terminates before parent calls wait()?

**Answer:**
The child becomes a **zombie process**. It has finished execution but its entry remains in the process table because the parent hasn't collected its exit status. The zombie occupies a slot in the process table until:
- Parent calls wait() or waitpid()
- Parent terminates (then init cleans it)

### Q7: How many processes are created by n fork() calls in a loop?

**Answer:**
Total processes = **2^n** (including the original process)

**Explanation:**
- 1st fork: 1 → 2 processes
- 2nd fork: 2 → 4 processes
- 3rd fork: 4 → 8 processes
- nth fork: 2^(n-1) → 2^n processes

### Q8: Do parent and child share variables after fork()?

**Answer:**
No! After fork(), each process has its own separate copy of all variables. They have independent memory spaces. Changes made by one process do not affect the other.

**Reason:** Copy-on-Write mechanism. Initially they share memory, but when either modifies a variable, OS creates a separate copy.

### Q9: What is wait() system call?

**Answer:**
wait() is used by parent to wait for child process termination and collect its exit status.

**Syntax:**
```c
#include <sys/wait.h>
int status;
pid_t child_pid = wait(&status);
```

**Purpose:**
- Prevents zombie processes
- Synchronizes parent with child
- Allows parent to know child's exit status

### Q10: What is the difference between zombie and orphan?

**Answer:**

| Aspect | Zombie | Orphan |
|--------|--------|--------|
| Definition | Child terminated, parent alive, no wait() | Parent terminated, child alive |
| Parent | Original parent alive | Adopted by init (PID 1) |
| State | Terminated (defunct) | Running normally |
| Problem | Wastes process table entry | No problem |
| Solution | Parent calls wait() | Automatically handled by OS |
| Harmful | Yes (if too many) | No |

### Q11: Can a process become both zombie and orphan?

**Answer:**
No, not at the same time. 
- **Zombie:** Process is dead, parent is alive
- **Orphan:** Process is alive, parent is dead

However, sequence is possible:
1. Child becomes orphan (parent dies)
2. init adopts orphan
3. Orphan terminates
4. Becomes zombie of init (very brief)
5. init immediately calls wait() and cleans it

### Q12: What is init process?

**Answer:**
- First process started by kernel during boot
- Always has PID = 1
- Parent of all orphan processes
- Automatically reaps (cleans) zombie children
- Runs until system shutdown
- On modern Linux: systemd (still PID 1)

### Q13: How to prevent zombie processes?

**Answer:**

**Method 1: Use wait()**
```c
if(fork() == 0) {
    // child work
    exit(0);
} else {
    wait(NULL);  // Wait for child
}
```

**Method 2: Use signal handler**
```c
signal(SIGCHLD, SIG_IGN);  // Ignore child termination signal
```

**Method 3: Use double fork**
```c
if(fork() == 0) {
    if(fork() == 0) {
        // grandchild does work
    }
    exit(0);  // intermediate child exits
}
wait(NULL);  // parent waits for intermediate child
// Grandchild becomes orphan, adopted by init
```

### Q14: What is the output of this code?
```c
int main() {
    fork() || fork();
    printf("Hello\n");
}
```

**Answer:** "Hello" is printed **3 times**.

**Explanation:**
- First fork(): creates 2 processes
  - Parent: fork() returns PID (true), || short-circuits, doesn't execute second fork
  - Child: fork() returns 0 (false), || evaluates second fork
- Child's second fork(): creates 2 more processes
  - Total: 1 parent + 2 from child's fork = 3 processes
- Each prints "Hello"

### Q15: What are process states in Unix/Linux?

**Answer:**

**Five process states:**

1. **NEW/CREATED:** Process is being created
2. **READY:** Process is ready to run, waiting for CPU
3. **RUNNING:** Process is currently executing on CPU
4. **WAITING/BLOCKED:** Process is waiting for I/O or event
5. **TERMINATED:** Process has finished execution

**Additional states:**
- **ZOMBIE:** Terminated but entry in process table
- **STOPPED:** Process suspended (Ctrl+Z)

**State Transitions:**
```
NEW → READY → RUNNING → TERMINATED
       ↑         ↓
       ←─ WAITING
```

---

## IMPORTANT SYSTEM CALLS

### 1. fork()
```c
#include <unistd.h>
pid_t fork(void);
```
- Creates new process
- Returns: 0 to child, PID to parent, -1 on error

### 2. getpid()
```c
#include <unistd.h>
pid_t getpid(void);
```
- Returns current process's PID

### 3. getppid()
```c
#include <unistd.h>
pid_t getppid(void);
```
- Returns parent's PID

### 4. wait()
```c
#include <sys/wait.h>
pid_t wait(int *status);
```
- Waits for any child to terminate
- Prevents zombie processes
- Returns child's PID

### 5. waitpid()
```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int options);
```
- Waits for specific child
- More control than wait()

### 6. exit()
```c
#include <stdlib.h>
void exit(int status);
```
- Terminates calling process
- Status returned to parent via wait()

### 7. sleep()
```c
#include <unistd.h>
unsigned int sleep(unsigned int seconds);
```
- Suspends execution for specified seconds

---

## PROCESS STATES AND TRANSITIONS

### Process Control Block (PCB)

Each process has a PCB containing:
- **Process ID (PID):** Unique identifier
- **Parent PID (PPID):** Parent's identifier
- **Process State:** Running, Ready, Waiting, etc.
- **Program Counter:** Next instruction address
- **CPU Registers:** Register values
- **Memory Information:** Page tables, memory limits
- **I/O Information:** Open files, devices
- **Accounting:** CPU time used, time limits

### State Diagram

```
    ┌──────────┐
    │   NEW    │
    └────┬─────┘
         │
         ↓
    ┌──────────┐      ┌───────────┐
    │  READY   │ ←──→ │  RUNNING  │
    └────┬─────┘      └─────┬─────┘
         ↑                  │
         │                  ↓
         │            ┌──────────┐
         └────────────│ WAITING  │
                      └──────────┘
                           │
                           ↓
                      ┌──────────┐
                      │TERMINATED│
                      └──────────┘
```

**Transitions:**
1. **NEW → READY:** Process created, ready to run
2. **READY → RUNNING:** Scheduler assigns CPU
3. **RUNNING → READY:** Time slice expired (preemption)
4. **RUNNING → WAITING:** I/O request, waiting for event
5. **WAITING → READY:** I/O completed, event occurred
6. **RUNNING → TERMINATED:** Process finished or killed

---

## TROUBLESHOOTING & COMMON ERRORS

### Error 1: Too many processes

**Symptom:**
```
fork: Resource temporarily unavailable
```

**Cause:** System limit on number of processes reached

**Solution:**
- Check limit: `ulimit -u`
- Increase limit: `ulimit -u 4096`
- Fix code creating too many processes

### Error 2: Zombie accumulation

**Symptom:**
```bash
$ ps aux | grep defunct
many  [zombie] <defunct> entries
```

**Cause:** Parent not calling wait()

**Solution:**
```c
// Add wait() in parent
if(fork() == 0) {
    // child code
} else {
    wait(NULL);  // ADD THIS
}
```

### Error 3: Segmentation fault in child

**Cause:**
- Accessing parent's pointers after fork()
- Pointers point to parent's memory space

**Solution:**
- Allocate new memory in child
- Don't share pointers across fork()

### Error 4: Race condition in output

**Symptom:** Output appears in random order

**Cause:** Parent and child run concurrently

**Solution:**
- Use wait() to synchronize
- Use proper I/O flushing
```c
fflush(stdout);  // Force output
```

### Error 5: Missing header files

**Symptom:**
```
error: implicit declaration of function 'fork'
```

**Solution:** Include proper headers
```c
#include <stdio.h>    // printf
#include <unistd.h>   // fork, getpid, getppid, sleep
#include <sys/wait.h> // wait, waitpid
#include <stdlib.h>   // exit
```

---

## TIPS FOR VIVA SUCCESS

1. **Always mention return values of fork()**
   - 0 to child
   - Child's PID to parent
   - -1 on error

2. **Explain the difference clearly:**
   - fork() vs exec()
   - Zombie vs Orphan
   - wait() vs waitpid()

3. **Know the formulas:**
   - n forks in loop = 2^n processes
   - Sum of n odd numbers = n²
   - Sum of n even numbers = n(n+1)

4. **Be ready to trace code execution:**
   - Draw process tree
   - Show timeline of events
   - Explain output order

5. **Understand practical applications:**
   - Web servers use fork() for handling clients
   - Shell creates processes using fork()+exec()
   - Daemons are created using fork()

6. **Common follow-up questions:**
   - How would you modify this program to...?
   - What happens if we add another fork()?
   - How to prevent zombie/orphan?

---

## SUMMARY

**Key Takeaways:**
- fork() creates identical copy of process
- Parent and child have separate memory
- Orphans are adopted by init (PID 1)
- Zombies waste process table entries
- Always use wait() to prevent zombies
- Process states: NEW, READY, RUNNING, WAITING, TERMINATED

**Remember for Viva:**
- Speak confidently
- Draw diagrams when explaining
- Give practical examples
- Understand concepts, don't memorize
- Be ready to modify code on the spot

Good luck! 🚀
