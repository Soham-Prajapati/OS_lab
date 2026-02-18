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
