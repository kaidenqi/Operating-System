#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) //child executs the command from the command line
    {
        printf("I am the Child, my pid is %d\n", getpid());
        printf("I am the Child, my Parent's pid is %d\n", getppid());
        
     /*printf("degug:  %s\n",argv[0]);
        printf("degug:  %s\n",argv[1]);
        printf("degug:  %s\n",argv[2]);*/
        
        argv[argc] = 0;
        execvp(argv[1],&argv[1]);
        
        perror("exec failed");
        exit(1);
    }
    else  // parent uses wait to suspend execution until child finishes
    {
        wait((int *)0);
        printf("Child process is completed\n");
        printf("I am the Parent, my pid is %d\n", getpid());
        exit(0);
    }
    
    return 0;
}
