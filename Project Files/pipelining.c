//Gustavo Platas

/*
To compile and run:
- gcc pipelining.c -o pipelining
- make pipelining
- ./pipelining | <command1> | <command2> | <command3>
*/
// The commands must be separated by the '|' pipeline operator, including after calling './pipelining'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int main(int argc, char* argv[]) 
{
    int pipefd1[2];
    int pipefd2[2];
    pid_t pid;
	
	char *cmd1 = argv[1];
	char *cmd2 = argv[2];
	char *cmd3 = argv[3];
	

    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) //if pipes fail, exit
	{
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) 
	{
        perror("fork");
        exit(EXIT_FAILURE);
    } 
	else if (pid == 0) 
	{
    /*      If this code segment is included, the program stops running 
        // SIGNAL HANDLING
        pid_t child_pid = getpid(); 
        setpgid(child_pid, child_pid);  // set the pgid to the child's pid
        tcsetpgrp(STDIN_FILENO, child_pid); //set this process group as the foreground process group
        signal(SIGINT, SIG_DFL);    // reset SIGINT to default 
        signal(SIGTSTP, SIG_DFL);   // reset SIGSTP to default
    */

        // Child process 1
        close(pipefd1[0]); // close unused read end
        dup2(pipefd1[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pipefd1[1]); // close write end
		execlp(cmd1, cmd1, NULL);
        
    } 
	else 
	{
        pid = fork();
        if (pid < 0) 
		{
            perror("fork");
            exit(EXIT_FAILURE);
        } 
		else if (pid == 0) 
		{
            // Child process 2
            close(pipefd1[1]); // close unused write end
            dup2(pipefd1[0], STDIN_FILENO); // redirect stdin from pipe
            close(pipefd1[0]); // close read end
            close(pipefd2[0]); // close unused read end
            dup2(pipefd2[1], STDOUT_FILENO); // redirect stdout to pipe
            close(pipefd2[1]); // close write end
            execlp(cmd2, cmd2, NULL); 
        } 
		else 
		{
            pid = fork();
            if (pid < 0) 
			{
                perror("fork");
                exit(EXIT_FAILURE);
            } 
			else if (pid == 0) 
			{
                // Child process 3
                close(pipefd2[1]); // close unused write end
                dup2(pipefd2[0], STDIN_FILENO); // redirect stdin from pipe
                close(pipefd2[0]); // close read end
                execlp(cmd3, cmd3, NULL); 
            } 
			else 
			{
                close(pipefd1[0]); // close unused read end
				close(pipefd1[1]); // close write end
                close(pipefd2[0]); // close unused read end
                close(pipefd2[1]); // close write end

                // wait for all child processes to finish
                int status, i;
                for (i = 0; i < 3; i++)
				{
					wait(&status);
				}

            /*      If this code segment is included, the program stops running 
                tcsetpgrp(STDIN_FILENO, getpgrp()); // when process is complete, restore the shell as the foreground process group
            */


                return 0;
            }
        }
    }
}
