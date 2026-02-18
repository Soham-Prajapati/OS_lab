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
