#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main() {


   pid_t pid = fork();


   if(pid > 0) {
       printf("Parent exiting...\n");
       exit(0);   // Parent terminates immediately
   }
   else if(pid == 0) {
       sleep(15);  // Child waits for 15 seconds so parent can exit
       printf("Child Process adopted by init.\n New Parent PID: %d\n", getppid());
   }


   return 0;
}
