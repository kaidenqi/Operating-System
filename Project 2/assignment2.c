/* Assignment2   Kai Qi	 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef READ
#define READ 0
#endif

#ifndef WRITE
#define WRITE 1
#endif

int main()
{
    char *argv[20], buf[80], n, *p;
    int m, status, inword, continu;

    /* flags for redirection */
    int input_redirection_flag, output_redirection_flag;

    /* variables for pipes */
    int count, pipes;
    pid_t pid;
    
    /* left and right pipes */
    int left_pipe[2], right_pipe[2];
    
    while (1)
    {
        /* reset parsing variables */
        inword = 0;
        p = buf; // a pointer which points to buffer
        m = 0;
        continu=0;
        
        /* reset piping variables */
        count = pipes = pid = 0;
        
        /* print the shell prompt on the terminal */
        printf( "\nshhh> ");

        /* parse shell commands which typed from terminal */
        while ( ( n = getchar() ) != '\n'  || continu )
        {
            if ( n ==  ' ' )
            {
                if ( inword )
                {
                    inword = 0;
                    *p++ = 0;
                }
            }
            else if ( n == '\n' )
                continu = 0;
            else if ( n == '\\' && !inword )
                continu = 1;
            else
            {
                if ( !inword )
                {
                    inword = 1;
                    argv[m++] = p;
                    *p++ = n;
                }
                else
                    *p++ = n;
            }
        }
        
        /* append terminating character to the end of parser buffer and argv buffer */
        *p++ = 0;
        argv[m] = 0;

        /* if user wants to terminate the program */
        if (strcmp(argv[0],"exit") == 0)
            exit (0);
        
        /* reset input and output flags */
        input_redirection_flag = output_redirection_flag = 0;
        int b = 1;
        
        char *input_path[1], *out_path[1];
        
        /* containers for handling arguments */
        int start[20];
        start[0] = 0;
        
        /* preprocessing  */
        while (argv[count] != 0)
        {
            if(argv[0] == NULL)
                break;
            else if (strcmp(argv[count], "|") == 0)
            {
                argv[count] = 0;
                start[b] = count + 1;
                ++pipes;
                b++;
            }
            else if (strcmp(argv[count], "<") == 0)
            {
                input_path[0] = argv[count + 1]; /* copy input file name  */
                argv[count] = 0;
                input_redirection_flag = 1;
            }
            else if (strcmp(argv[count], ">") == 0)
            {
                out_path[0] = argv[count + 1]; /* copy output file name  */
                argv[count] = 0;
                output_redirection_flag = 1;
            }
            ++count;
        }
        
        /* execute commands */
        for(int index = 0; index <= pipes; ++index)
        {
            if (pipes > 0)
                pipe(right_pipe);
            
            pid = fork();
            if (pid < 0)
                perror("fork failed");
            else if (pid == 0)
            {
                //child process manages input redirection
                if((index == 0) && (input_redirection_flag == 1))
                {
                    int input = open(input_path[0], O_RDONLY);
                    if (input == -1)
                    {
                        perror("Input file failed to open\n");
                        return(EXIT_FAILURE);
                    }
                    close(READ);
                    dup(input);
                    close(input);
                }
                
                //child process manages output redirection
                if ((index == pipes) && (output_redirection_flag == 1))
                {
                    int output = creat(out_path[0], 00777);
                    if (output < 0)
                    {
                        perror("Output file failed to open\n");
                        return(EXIT_FAILURE);
                    }
                    close(WRITE);
                    dup(output);
                    close(output);
                }
                
                //child process manages pipes
                if (pipes > 0)
                {
                    if (index == 0) /* first child process */
                    {
                        close(WRITE);
                        dup(right_pipe[WRITE]);
                        close(right_pipe[WRITE]);
                        close(right_pipe[READ]);
                    }
                    else if (index < pipes) /* in-between child process */
                    {
                        close(READ);
                        dup(left_pipe[READ]);
                        close(WRITE);
                        dup(right_pipe[WRITE]);
                        close(right_pipe[READ]);
                        close(right_pipe[WRITE]);
                        close(left_pipe[READ]);
                        close(left_pipe[WRITE]);
                    }
                    else /* final child process */
                    {
                        close(READ);
                        dup(left_pipe[READ]);
                        close(left_pipe[READ]);
                        close(left_pipe[WRITE]);
                    }
                }
                
                /* execute commands */
                execvp(argv[start[index]], &argv[start[index]]);
                
            }
            else // parent process manages the pipes for the child processes
            {
                if (index > 0)
                {
                    close(left_pipe[READ]);
                    close(left_pipe[WRITE]);
                }
                left_pipe[READ] = right_pipe[READ];
                left_pipe[WRITE] = right_pipe[WRITE];
                
                /* parent process waits for child process to complete */
                wait(&status);
            }
        }
     wait(&status);
    }
    return 0;
}

