# Lab 2 — Static and Dynamic Linking

## Problem Statement
Implement a simple summation library and demonstrate both static linking and dynamic linking methods.

## The Code Files:

**calc.c** - Library implementation
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

**hrl_lab.h** - Header file
```c
int summation(int n);
```

**driver.c** - Main program
```c
#include<stdio.h>
#include "hrl_lab.h"

void main(){
    printf("Summation Program\n");
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);
    
    int result = summation(n);
    if(result != -1){
        printf("Summation of 1 to %d = %d\n", n, result);
    }
}
```

## Compilation Process for Static Linking:

```bash
# Step 1: Compile calc.c to object file
gcc -c calc.c -o calc.o

# Step 2: Create static library (archive)
ar rcs hrl_lab.a calc.o

# Step 3: Compile driver.c to object file  
gcc -c driver.c -o driver.o

# Step 4: Link driver with static library to create executable
gcc -o driver driver.o -L. hrl_lab.a

# Step 5: Run the program
./driver

# Step 6: Clean up
rm -f calc.o driver.o hrl_lab.a driver
```

## Compilation Process for Dynamic Linking:

```bash
# Step 1: Compile calc.c with -fPIC flag
gcc -c -fPIC calc.c -o calc.o

# Step 2: Create shared library (macOS uses .dylib)
gcc -dynamiclib -o hrl_lab.dylib calc.o

# Step 3: Compile and link driver with shared library
gcc -L. -o driver driver.c hrl_lab.dylib

# Step 4: Run the program
./driver

# Step 5: Clean up
rm -f calc.o hrl_lab.dylib driver
```

## What is Static Linking?
- Library code is copied into the executable at compile time
- Creates standalone executable
- `ar rcs` creates archive with symbol table
- Example: summation(5) = 1+2+3+4+5 = 15

## What is Dynamic Linking?
- Library is loaded at runtime
- Smaller executable size
- Multiple programs can share the same library in memory
- `-fPIC` creates Position Independent Code
- On macOS, use `.dylib` instead of `.so`
