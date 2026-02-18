#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main() {


   pid_t pid = fork();


   if(pid > 0) {
       sleep(15);  // Parent sleeping
       printf("Parent awake\n");
   }
   else if(pid == 0) {
       printf("Child exiting...\n");
       exit(0);  // Child terminates immediately
   }


   return 0;
}
