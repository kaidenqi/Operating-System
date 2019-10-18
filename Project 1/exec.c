#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if (ret == -1)
    {
        perror("pipe error");
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid < 0) //error occurred
    {
        perror("fork error");
        exit(1);
    }
    else if (pid == 0) //child process performs ./pre
    {
        close(1);
        dup(fd[1]);
        close(fd[1]);
        close(fd[0]);
        execlp("./pre", "pre", NULL);
        perror("execlp failed");
        exit(1);
    }
    else  //parent process performs ./sort
    {
        close(0);
        dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
        execlp("./sort", "sort", NULL);
        perror("execlp failed");
        exit(1);
    }
    
    //close(fd[0]);
    //close(fd[1]);
    
    return 0;
    
}
