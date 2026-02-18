# Experiment 2 - Static and Dynamic Linking

## Name: Soham Prajapati
## UID no.: 2024308186
## Experiment No.: 2

---

## AIM:
Write a program for creating a static and dynamic link library for mathematical operations (summation) and test the library through both static and dynamic linking methods.

---

## PROBLEM STATEMENT:

### Theory:

**Linking** is the process of combining various pieces of code and data into a single executable file. There are two types of linking:

**1. Static Linking:**
Static linking is performed at compile time. The linker copies all library routines used in the program into the executable image. This results in a larger executable file, but the program becomes self-contained and doesn't require external library files at runtime.

**Advantages:**
- Standalone executable - no external dependencies
- Faster execution (no runtime linking overhead)
- Version compatibility guaranteed

**Disadvantages:**
- Larger executable size
- Memory inefficiency when multiple programs use the same library
- Must recompile to update library code

**2. Dynamic Linking:**
Dynamic linking is performed at runtime. The executable contains references to external library files (.so on Linux, .dylib on macOS, .dll on Windows) that are loaded when the program runs. Multiple programs can share a single copy of the library in memory.

**Advantages:**
- Smaller executable size
- Memory efficient (shared libraries)
- Library updates don't require recompilation
- Modular design

**Disadvantages:**
- Runtime dependency on library files
- Slightly slower (runtime linking overhead)
- Version conflicts possible

**Problem:** Create a library that calculates the summation of numbers from 1 to n (sum = 1+2+3+...+n) and demonstrate both static and dynamic linking approaches by building a driver program that uses this library.

---

## Algorithm:

### For Library (calc.c):
1. Define function `summation(int n)`
2. Check if n is negative, return -1 with error message
3. Initialize sum = 0
4. Loop from i = 1 to n
   - Add i to sum
5. Return sum

### For Driver Program (driver.c):
1. Include library header file
2. Display program title
3. Prompt user to enter a number
4. Read input number n
5. Call summation(n) function from library
6. If result is valid (not -1), display result
7. Exit

### For Static Linking:
1. Compile library source to object file: `gcc -c calc.c -o calc.o`
2. Create static archive: `ar rcs hrl_lab.a calc.o`
3. Compile driver to object file: `gcc -c driver.c -o driver.o`
4. Link driver with static library: `gcc -o driver driver.o -L. hrl_lab.a`
5. Run executable: `./driver`

### For Dynamic Linking:
1. Compile library with Position Independent Code: `gcc -c -fPIC calc.c -o calc.o`
2. Create shared library: `gcc -dynamiclib -o hrl_lab.dylib calc.o`
3. Compile and link driver with shared library: `gcc -L. -o driver driver.c hrl_lab.dylib`
4. Run executable: `./driver`

---

## PROGRAM:

### File 1: hrl_lab.h (Header File)
```c
int summation(int n);
```

### File 2: calc.c (Library Implementation)
```c
#include<stdio.h>

int summation(int n){
    if(n < 0){
        printf("Cannot calculate summation for negative numbers!");
        return -1;
    }
    int sum = 0;
    for(int i = 1; i <= n; i++){
        sum += i;
    }
    return sum;
}
```

### File 3: driver.c (Driver Program)
```c
#include<stdio.h>
#include "hrl_lab.h"

int main(){
    printf("Summation Program\n");
    int n;

    printf("Enter a number: ");
    scanf("%d", &n);
    
    int result = summation(n);
    if(result != -1){
        printf("Summation of 1 to %d = %d\n", n, result);
    }
    return 0;
}
```

---

## COMPILATION AND EXECUTION:

### Static Linking Process:
```bash
# Step 1: Compile calc.c to object file
gcc -c calc.c -o calc.o

# Step 2: Create static library (archive)
ar rcs hrl_lab.a calc.o

# Step 3: Compile driver.c to object file
gcc -c driver.c -o driver.o

# Step 4: Link driver with static library
gcc -o driver driver.o -L. hrl_lab.a

# Step 5: Run the program
./driver

# Clean up
rm -f calc.o driver.o hrl_lab.a driver
```

### Dynamic Linking Process:
```bash
# Step 1: Compile calc.c with Position Independent Code
gcc -c -fPIC calc.c -o calc.o

# Step 2: Create shared library (macOS uses .dylib)
gcc -dynamiclib -o hrl_lab.dylib calc.o

# Step 3: Compile and link driver with dynamic library
gcc -L. -o driver driver.c hrl_lab.dylib

# Step 4: Run the program
./driver

# Clean up
rm -f calc.o hrl_lab.dylib driver
```

---

## Output:

### Static Linking Output:
```
Summation Program
Enter a number: 5
Summation of 1 to 5 = 15
```

### Dynamic Linking Output:
```
Summation Program
Enter a number: 10
Summation of 1 to 10 = 55
```

**Explanation:**
- For input n=5: sum = 1+2+3+4+5 = 15
- For input n=10: sum = 1+2+3+4+5+6+7+8+9+10 = 55

---

## CONCLUSION:

1. Successfully implemented a mathematical library for summation calculations.

2. Demonstrated static linking where the library code is embedded into the executable at compile time using `ar` archiver tool, resulting in a standalone binary.

3. Demonstrated dynamic linking where the library is loaded at runtime, creating a smaller executable that depends on the shared library file (.dylib on macOS).

4. **Key Observations:**
   - Static linking produces larger executables but with no runtime dependencies
   - Dynamic linking produces smaller executables but requires the shared library at runtime
   - The `ar rcs` command creates static archive with symbol table
   - The `-fPIC` flag generates Position Independent Code necessary for shared libraries
   - Both methods produce identical functional output

5. **Practical Applications:**
   - Static linking is preferred for distributing standalone applications
   - Dynamic linking is preferred for system libraries and plugins
   - Understanding both methods is crucial for software development and deployment

6. Both linking methods were successfully tested and produced correct results for the summation function.
