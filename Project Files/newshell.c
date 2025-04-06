#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>

#include <ctype.h>
#include <alias.h>
#include <built_inCommands.h>

bool quit = false;  // bool value for exit command
#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
HashItem *table[MAX_HASH] = {0};
int SIZE = 512;

int error(const char *msg)
{
    printf("Error: %s\n", msg);
    return -1;
}

void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s> ", cwd);
}

void clear(char *splitStrings[SIZE]) // function to clear the array arguments
{
    for (size_t i = 0; i < SIZE; i++)
    {
        splitStrings[i] = NULL;
    }
}

// function to execute the commands from user input
void execute(char *splitStrings[SIZE], int ctr)
{
    bool in = false;
    bool out = false;
    int pipes = 0; // monitors number of pipes

    if (!(splitStrings[0])) // if the pointer is null aka no arguments, exit
    {
        return;
    }

    char *argum[SIZE]; // arguments array

    for (int i = 0; i < SIZE; i++)
    {
        argum[i] = NULL; // set all positions to null
    }

    int j = 0;            // argum counter
    char term[] = "exit"; // used to check for the exit command
    int file_c;
    int pindx[2] = {-1, -1}; // indexes of pipes
    char *cmd2 = NULL;       // Stores second command
    char *cmd3 = NULL;       // stores third command
    for (int i = 0; i < ctr; i++)
    {
        if (!(strcmp(splitStrings[i], term))) // if the exit command is found, set our bool to true and proceed
        {
            quit = true;
            return;
        }
        else if (!(strcmp(splitStrings[i], "cd"))) // if the exit command is found, set our bool to true and proceed
        {

            if (splitStrings[i + 1])
                if (i + 1 < ctr && splitStrings[i + 1])
                {
                    changeDir(splitStrings[i + 1]);
                    i++;
                }
                else
                {
                    changeDir(NULL);
                }
            return;
        }
        else if (!(strcmp(splitStrings[i], "alias"))) // if the exit command is found, set our bool to true and proceed
        {
            if (splitStrings[i + 1] && splitStrings[i + 2])
            {

                runAlais(table, splitStrings[i + 1], splitStrings[i + 2]);
                i += 2;
            }
            else if (splitStrings[i + 1])
            {
                runAlais(table, splitStrings[i + 1], NULL);
                i++;
            }
            else
            {

                runAlais(table, NULL, NULL);
            }
            return;
        }
        else if (!strcmp(splitStrings[i], "path"))
        {                    // if path command is found
            int option = -1; // checks how many arguments were supplied
            if (splitStrings[i + 1] && splitStrings[i + 2])
            {
                if (!strcmp(splitStrings[i + 1], "+"))
                {
                    option = 1; // if adding to path option is 1
                }
                else if (!strcmp(splitStrings[i + 1], "-"))
                {
                    option = 2; // if removing from path option is 2
                }
                pathFunc(option, splitStrings[i + 2]); // calling with option and pathname
            }
            else if (!splitStrings[i + 1])
            { // if there is no argument run normal path call
                pathFunc(0, NULL);
            }
            return;
        }
        else if (!(strcmp(splitStrings[i], "<")))
        {
            in = true;
            file_c = i + 1;
            break;
        }
        else if (!(strcmp(splitStrings[i], ">")))
        {
            out = true;
            file_c = i + 1;
            break;
        }
        else if (!(strcmp(splitStrings[i], "|")))
        {
            pindx[pipes++] = ++i; // increase number of known pipes and save index of next command
            continue;
        }
        if (!splitStrings[i]) // if we find a null pointer continue, not needed, may remove later
        {
            continue;
        }
        argum[j] = splitStrings[i]; // copy
        if (pipes == 1 && cmd2 == NULL)
            cmd2 = argum[j]; // store second command
        else if (pipes == 2 && cmd3 == NULL)
            cmd3 = argum[j]; // store third command
        j++;
    }
    printf("\n");

    // fork
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("failed forking child..\n");
        exit(1);
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

        // child process
        if (pipes == 0)
        {
            if (in)
            {
                int fd0 = open(splitStrings[file_c], O_RDONLY);
                if (fd0 == -1 || !(splitStrings[file_c]))
                {
                    perror("redirection input file not entered or does not exist...\n");
                    return;
                }
                dup2(fd0, STDIN_FILENO);
                close(fd0);
            }
            if (out)
            {
                int fd1 = creat(splitStrings[file_c], 0644);
                if (fd1 == -1 || !(splitStrings[file_c]))
                {
                    perror("redirection output file not entered or error opening file...\n");
                    return;
                }
                dup2(fd1, STDOUT_FILENO);
                close(fd1);
            }
            execvp(splitStrings[0], argum); // execvp takes in arguments as an array of type char *
            printf("command not found...\n");
            exit(1);
        }
        else
        {               // if there are pipes
            int fd[2];  // first pipe
            int fd2[2]; // second pipe
            if (pipe(fd) == -1)
            {
                perror("Pipe 1");
                exit(1); // makes sure pipe is created properly
            }
            switch (fork())
            { // fork for the first time
            case -1:
                perror("Fork 1"); // print error if fork fails
                exit(1);
            case 0:
                // First Child
                if (pipes == 1)
                { // if one pipe run second command with input from first command without forking
                    dup2(fd[0], fileno(stdin));
                    close(fd[0]);
                    close(fd[1]);
                    char *args2[] = {cmd2, NULL};
                    execvp(args2[0], args2);
                    printf("Command %s not found", splitStrings[pindx[0]]);
                    exit(1);
                }
                else
                {
                    if (pipes > 2)
                        printf("Only up to 2 pipelines are supported the rest will be disregarded\n"); // print if more than 2 pipes are requested
                    if (pipe(fd2) == -1)
                    { // second pipe
                        perror("Pipe 2");
                        exit(1);
                    }
                    switch (fork())
                    { // second fork
                    case -1:
                        perror("Fork 2"); // error for fork failure
                        exit(1);
                    case 0:                          // Second Child
                        dup2(fd2[0], fileno(stdin)); // takes piped output from cmd1 and cmd2 and runs cmd3
                        close(fd2[0]);
                        close(fd2[1]);
                        close(fd[0]);
                        close(fd[1]);
                        char *args3[] = {cmd3, NULL};
                        execvp(args3[0], args3);

                        printf("Command %s not found", splitStrings[pindx[1]]);
                        exit(1);
                    default:                        // First Child
                        dup2(fd[0], fileno(stdin)); // takes output from cmd1 and runs cmd2
                        dup2(fd2[1], fileno(stdout));
                        close(fd[0]);
                        close(fd[1]);
                        close(fd2[0]);
                        close(fd2[1]);
                        char *args2[] = {cmd2, NULL};
                        execvp(args2[0], args2);

                        printf("Command %s not found", splitStrings[pindx[0]]);
                        exit(1);
                    }
                }
            default:
                // Parent
                dup2(fd[1], fileno(stdout)); // runs cmd1
                close(fd[0]);
                close(fd[1]);
                execvp(splitStrings[0], argum);
                printf("Command %s not found", splitStrings[0]);

                /*      If this code segment is included, the program stops running
                    tcsetpgrp(STDIN_FILENO, getpgrp()); // when process is complete, restore the shell as the foreground process group
                */
                exit(1);
            }
        }
    }
    else
    {
        // parent process waits for child
        wait((int *)0);
    }
}

int takeInput(char *str)
{
    // char* buf;
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    read = getline(&line, &len, stdin);
    if (read > 512)
    {
        printf("Character limit of 512 reached...\n");
        printf("Your character count: %zu\n", read);
        printf("Continuing with the first 512 characters...\n");
    }
    if (read != 1)
    {                               // 1 and not 0 due to new line
        if (line[read - 1] == '\n') // Removing the new line from getline
            line[read - 1] = 0;
        // add_history(buf); // add history code here
        if (line)
        {
            strcpy(str, line);
        }

        return 0;
    }
    else
    {
        free(line);
        return 1;
    }
}

int parseSpace(char *str, char **parsed)
{
    // might be better way to clear
    // worried about possible memory leak
    for (int i = 0; i < MAXLIST; i++)
    {
        if (parsed[i] != NULL)
        {
            parsed[i] = NULL;
        }
        else
        {
            break;
        }
    }

    const char *p = str;
    int count = 0;

    // equals will also count as space
    // e.g. alias test='cd ..' is same as alias test 'cd ..'
    for (int i = 0; i < MAXLIST; i++)
    {
        while (isspace(*p) || *p == '=')
            p++;
        if (*p == '\0')
            break;

        if (*p == '"' || *p == '\'')
        {
            int quote = *p++;
            const char *begin = p;

            while (*p && *p != quote)
                p++;
            if (*p == '\0')
                return error("Unmatched quote");
            char buffer[SIZE];
            snprintf(buffer, MAXCOM, "%.*s", (int)(p - begin), begin);
            parsed[count] = strdup(buffer);
            if (!parsed[count])
            {
                perror("Memory allocation failed for strdup");
                exit(1); // Or handle error more gracefully
            }
            count++;
            p++;
            continue;
        }

        if (strchr("<>()|", *p))
        {
            char buffer[SIZE];
            snprintf(buffer, MAXCOM, "%c", *p);
            parsed[count] = strdup(buffer);
            if (!parsed[count])
            {
                perror("Memory allocation failed for strdup");
                exit(1); // Or handle error more gracefully
            }
            count++;
            p++;
            continue;
        }

        if (isalnum(*p) || *p == '.' || *p == '/' || *p == '-' || *p == '+')
        {
            const char *begin = p;

            while (isalnum(*p) || *p == '.' || *p == '/' || *p == '-' || *p == '+')
                p++;
            char buffer[SIZE];
            snprintf(buffer, MAXCOM, "%.*s", (int)(p - begin), begin);
            parsed[count] = strdup(buffer);
            if (!parsed[count])
            {
                perror("Memory allocation failed for strdup");
                exit(1); // Or handle error more gracefully
            }
            count++;
            continue;
        }

        return error("Illegal character");
    }
    return count;
}

void signal_handler(int signal)
{
    // do nothing, which ignores signal in shell
}

int main(int argc, char *argv[])
{
    // ignore SIGNINT and SIGSTOP in shell
    signal(SIGINT, signal_handler);
    signal(SIGSTOP, signal_handler);

    int charCount = 0;
    // if more than 2 arguments, exit
    if (argc > 2)
    {
        printf("too many arguments passed.\n");
        printf("please enter: newshell [batchFile]\n");
        printf("exiting...\n");
        exit(1);
    }
    // if theres a second argument enter batch mode
    else if (argc == 2)
    {
        FILE *batch_f;
        char buffer[SIZE];
        for (int i = 0; i < SIZE; i++) // initialized buffer to null so charCount can step through loop
            buffer[i] = '\0';
        batch_f = fopen(argv[1], "r");
        if (!batch_f)
        {
            perror("cannot open file...\n");
            exit(1);
        }
        while (fgets(buffer, SIZE, batch_f))
        {
            printf("%s", buffer);

            HashItem *aliasCheck = getItem(table, buffer); // check alias table

            if (aliasCheck)
            {
                strcpy(buffer, aliasCheck->value);
            }

            char **splitStrings; // 2-D array to store strings

            for (int i = 0; i < SIZE; i++)
                if (buffer[i] != '\0')
                    charCount++;

            int length;
            splitStrings = str_split(buffer, &length, ';');

            for (int i = 0; i < length; i++)
            {
                char *parsedCommand[MAXLIST] = {NULL}; // Correct initialization
                int lengthArgs = parseSpace(splitStrings[i], parsedCommand);
                execute(parsedCommand, lengthArgs);
            }
        }
        if (feof(batch_f))
        {
            printf("end of file reached..\n");
            if (charCount > 512) // if charCount over 512 then notify and continue
            {
                printf("Character limit of 512 reached...\n");
                printf("Your character count: %i\n", charCount);
                printf("Continuing with the first 512 characters...\n");
            }
        }
        else
        {
            printf("error while reading..\n");
        }
    }
    else
    {
        char input[SIZE]; // character array to store users input

        while (1)
        {
            printDir(); // prompt

            if (takeInput(input))
                continue;

            HashItem *aliasCheck = getItem(table, input); // check alias table

            if (aliasCheck)
            {
                strcpy(input, aliasCheck->value);
            }

            char **splitStrings; // 2-D array to store strings

            int length;
            splitStrings = str_split(input, &length, ';');

            for (int i = 0; i < length; i++)
            {
                char *parsedCommand[MAXLIST] = {NULL}; // Initialize all elements to NULL.
                int lengthArgs = parseSpace(splitStrings[i], parsedCommand);
                execute(parsedCommand, lengthArgs);
            }

            if (quit)
            {
                printf("Exiting Shell...\n");
                exit(0);
            }
        }
    }

    return 0;
}
