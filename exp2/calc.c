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
