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
