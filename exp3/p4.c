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
