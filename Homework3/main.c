//Author: Cory McDonald

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

#define BUFSIZE 1024
#define CSTRSIZE 100
#define CMDSIZE 30

#define DEBUG 1


int parse_command(char *line, char **cmd1, char **cmd2, char *infile, char *outfile);
void exec_cmd(char** cmd1);
void exec_cmd_in(char** cmd1, char* infile);
void exec_cmd_opt_in_append(char** cmd1, char* infile, char* outfile);
void exec_cmd_opt_in_write(char** cmd1, char* infile, char* outfile);
void exec_pipe(char** cmd1, char** cmd2);
void exec_pipe_in(char** cmd1, char** cmd2, char* infile);
void exec_pipe_opt_in_append(char** cmd1,char** cmd2,char* infile,char* outfile);
void exec_pipe_opt_in_write(char** cmd1,char** cmd2,char* infile,char* outfile);

bool interface(char *line);
void logInfo(char *line);


int main ( int argc, char *argv[] )
{
    if (argc == 2)
    {
        interface(argv[1]);
    }
    else
    {
        bool active = true;
        char userCommand[BUFSIZE];
        while (active)
        {
            printf("myshell-%% ");
            //This makes sure we don't go past the limit of 256 characters. That would be very, very bad
            fgets (userCommand, 256, stdin);
            //Removing newline from commandls
            if ((strlen(userCommand) > 0) && (userCommand[strlen (userCommand) - 1] == '\n'))
            userCommand[strlen (userCommand) - 1] = '\0';
            if (userCommand[0] != '\0' && userCommand != NULL)
            {
                if ( interface(userCommand) == true)
                {
                    active = false;
                    break;
                }
            }
        }
    }
    return 0;
}
bool interface(char *line)
{
    
    // Bad practices ftw
    char infile[CSTRSIZE];
    char outfile[CSTRSIZE];
    bool continueExecute = false;
    char *cmd1[CMDSIZE];
    char *cmd2[CMDSIZE];
    int i;
    int k;
    cmd1[0] = NULL;
    cmd2[0] = NULL;
    infile[0] = '\0';
    outfile[0] = '\0';
    
    i = parse_command(line, cmd1, cmd2, infile, outfile);
    if(i == 0)
    {
        continueExecute = true;
    }
    switch(i)
    {
        case 1:
            exec_cmd(cmd1);
            break;
        case 2:
            exec_cmd_in(cmd1, infile);
            break;
        case 3:
            exec_cmd_opt_in_append(cmd1, infile, outfile);
            break;
        case 4:
            exec_cmd_opt_in_write(cmd1, infile, outfile);
            break;
        case 5:
            exec_pipe(cmd1, cmd2);
            break;
        case 6:
            exec_pipe_in(cmd1, cmd2,infile);
            break;
        case 7:
            exec_pipe_opt_in_append(cmd1, cmd2,infile,outfile);
            break;
        case 8:
            exec_pipe_opt_in_write(cmd1, cmd2,infile,outfile);
            break;
        default:
            break;
    }
    
    if (i > 9)
    {
        k = 0;
        while (cmd1[k] != NULL)
        {
            printf("cmd1[%d] = %s\n", k, cmd1[k]);
            k++;
        };
        k = 0;
        while (cmd2[k] != NULL)
        {
            printf("cmd2[%d] = %s\n", k, cmd2[k]);
            k++;
        };
        if (strlen(infile))
        {
            printf("input redirection file name: %s\n", infile);
        }
        if (strlen(outfile))
        {
            printf("output redirection file name: %s\n", outfile);
        }
    }
    printf("return code is %d\n", i);
    
    return continueExecute;
}
int parse_command(char *line, char **cmd1, char **cmd2, char *infile, char *outfile)
{
    int returnCode = 9;
    int cmd1Index = 0;
    int cmd2Index = 0;
    
    char delimin[2] = " ";
    char outputRedirectedTo[3]; //Could be >>,>,<
    char *token;
    char *copyOfLine = (char *) malloc (strlen(line) + 1);
    
    bool isOutputRedirected = false;
    bool pipe = false;
    bool reset = true;
    copyOfLine = strcpy(copyOfLine, line);
    token = strtok(copyOfLine, delimin); //Tokenizing
    
    while (token != NULL)
    {
        if (strstr(token, "quit") || strstr(token, "exit")) //Quiting
        {
            printf("Program terminates successfully by the user\n");
            returnCode = 0;
            break;
        }
        else if (reset == true) //Taking in command, otherwise we will assume it is an argument
        {            
            
            reset = false;
            //Return code stuff
            //This way we can make sure that we have all executables
            if(true)
            {
                if (pipe == true)
                {
                    
                    cmd2[cmd2Index] = token + '\0';
                    cmd2Index++;
                    if(isOutputRedirected == true)
                    {
                       returnCode += 4;   
                       isOutputRedirected = false;
                    }else
                    {
                        returnCode = 5;
                    }
                }
                else
                {
                    //Simple command
                    cmd1[cmd1Index] = token + '\0';
                    cmd1Index++;
                    returnCode = 1;
                }
            }else
            {
                returnCode = 9;
                break;
            }
        }
        else if (strstr(token, "|")) //Piping include a space
        {
            reset = true;
            pipe = true;
            if (strlen(token) > 1) //more than 1 character, an argument has been attached to it
            {
                char substringToken[3];
                memcpy( substringToken, &token[0], 2 ); //Figure out what the hell i'm doing here.
                substringToken[2] = '\0';
            }
        }
        else if (strstr(token, ">>") || strstr(token, ">") || strstr(token, "<")) //Output redirected
        {
            isOutputRedirected = true;
            strncpy(outputRedirectedTo, token, sizeof(outputRedirectedTo));
            outputRedirectedTo[sizeof(outputRedirectedTo) - 1] = '\0';
        }
        else if (isOutputRedirected == true)
        {
            if (strstr(outputRedirectedTo, ">>"))
            {
                strcpy(outfile, token);
                if (pipe == true)
                {
                    returnCode = 7;
                }
                else
                {
                    returnCode = 3;
                }
            }
            else if (strstr(outputRedirectedTo, ">"))
            {                
                strcpy(outfile, token);
                if (pipe == true)
                {
                    returnCode = 8;
                }
                else
                {
                    returnCode = 4;
                }
            }
            else if (strstr(outputRedirectedTo, "<"))
            {

                if (pipe == true)
                {
                    returnCode = 6;
                }else
                {
                    strcpy(infile, token);
                    returnCode = 2;
                }
            }

        }
        else
        {
            if (pipe == true)
            {
                cmd2[cmd2Index] = token + '\0';
                cmd2Index++;
            }
            else
            {
                cmd1[cmd1Index] =  token + '\0';
                cmd1Index++;
            }       
        }       
        
        token = strtok(NULL, delimin);
    }
    cmd1[cmd1Index] = NULL;
    cmd2[cmd2Index] = NULL;
    return returnCode;
}


void exec_cmd(char** cmd1)
{
    pid_t pid;
    if ((pid = fork()) == -1)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {
        execvp(cmd1[0], cmd1);
        printf("%s: command not found\n", cmd1[0]);
    }else
    {
        waitpid(pid, NULL, 0);
    }
}
void exec_cmd_in(char** cmd1, char* infile)
{
    pid_t pid;
    if ((pid = fork()) == -1)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {
        int fd = open(infile, O_RDONLY);
        dup2(fd, 0);
        close(fd);
        execvp(cmd1[0], cmd1);
    }else
    {
        waitpid(pid, NULL, 0);
    }
}
void exec_cmd_opt_in_append(char** cmd1, char* infile, char* outfile)
{
    pid_t pid;
    
    if ((pid = fork()) == -1)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {
        if(outfile[0] != '\0')
        {
            int outFD = open(outfile, O_APPEND | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            if(infile[0] != '\0')
            {
                int inFD = open(infile, O_RDONLY);
                dup2(inFD, 0);
                close(inFD);
            }
            dup2(outFD, 1);
            close(outFD);
            
            execvp(cmd1[0], cmd1);
        }
    }
    else
    {
        waitpid(pid, NULL, 0);
        
    }
}
void exec_cmd_opt_in_write(char** cmd1, char* infile, char* outfile)
{
    pid_t pid;
    
    if ((pid = fork()) == -1)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {
        if(outfile[0] != '\0')
        {
            int outFD = open(outfile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if(infile[0] != '\0')
            {
                int inFD = open(infile, O_RDONLY);
                dup2(inFD, 0);
                close(inFD);
            }
            dup2(outFD, 1);
            close(outFD);
            
            execvp(cmd1[0], cmd1);
        }
    }
    else
    {
        waitpid(pid, NULL, 0);
        
    }
}
void exec_pipe(char** cmd1, char** cmd2)
{
    int pipefd[2];
    int pid2;
    int pid1;
    
    pipe(pipefd);
        
    if ((pid1 = fork()) == -1)
    {
        perror("fork error");
    }
    else if (pid1 == 0)
    {
        pid2 = fork();
        
        if (pid2 == 0) //CHILD
        {    
            wait(0);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]);            
            
            execvp(cmd2[0], cmd2);
            printf("%s: command not found\n", cmd2[0]);
            exit(1);
        }
        else 
        {   
            
            //PARENT
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);            
            execvp(cmd1[0], cmd1);
            printf("%s: command not found\n", cmd1[0]);
            exit(1);
        }
        
    }
    wait(0);     
    close(pipefd[0]);
    close(pipefd[1]);
}
void exec_pipe_in(char** cmd1, char** cmd2, char* infile)
{
    int pipefd[2];
    int pid2;
    int pid1;

    pipe(pipefd);

    if ((pid1 = fork()) == -1)
    {
        perror("fork error");
    }
    else if (pid1 == 0)
    {
        int fd = open(infile, O_RDONLY);
        close(pipefd[0]);
        dup2(pipefd[1], 1);

        dup2(fd, 0);
        close(fd);
        close(pipefd[1]);

        execvp(cmd1[0], cmd1); //Ends child
        printf("%s: command not found\n", cmd1[0]);
        exit(1);
    }
    //In this situation I did not fork in the child because I could not wait for pid2 to finish inside the parent 
    else
    {
        pid2 = fork();

        if (pid2 == 0) //CHILD
        {
            waitpid(pid1, NULL, 0); //Waiting for child to finish

            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);

            execvp(cmd2[0], cmd2);
            printf("%s: command not found\n", cmd2[0]);
            exit(1);
        }
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid2, NULL, 0);
}
void exec_pipe_opt_in_append(char** cmd1,char** cmd2,char* infile,char* outfile)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;

    pipe(pipefd);
    pid1 = fork();

    if (pid1 < 0)
    {
        perror("error with fork");
    }
    else if (pid1 == 0)
    {
        int fd = open(infile, O_RDONLY);

        close(pipefd[0]);
        dup2(pipefd[1], 1);
        dup2(fd, 0);
        close(fd);
        close(pipefd[1]);
        execvp(cmd1[0], cmd1);
        printf("%s: command not found\n", cmd1[0]);
        exit(1);
    }
    if (pid1 > 0)
    {
        pid2 = fork();
        if (pid2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            
            int outFD = open(outfile, O_APPEND | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(outFD, 1);
            close(outFD);  
            
            execvp(cmd2[0], cmd2);
            printf("%s: command not found\n", cmd2[0]);
            exit(1);
        }
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid2, NULL, 0);
}
void exec_pipe_opt_in_write(char** cmd1,char** cmd2,char* infile,char* outfile)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;

    pipe(pipefd);
    pid1 = fork();

    if (pid1 < 0)
    {
        perror("error with fork");
    }
    else if (pid1 == 0)
    {
        int fd = open(infile, O_RDONLY);

        close(pipefd[0]);
        dup2(pipefd[1], 1);
        dup2(fd, 0);
        close(fd);
        close(pipefd[1]);
        execvp(cmd1[0], cmd1);
        printf("%s: command not found\n", cmd1[0]);
        exit(1);
    }
    if (pid1 > 0)
    {
        pid2 = fork();
        if (pid2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            
            int outFD = open(outfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(outFD, 1);
            close(outFD);  
            
            execvp(cmd2[0], cmd2);
            printf("%s: command not found\n", cmd2[0]);
            exit(1);
        }
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid2, NULL, 0);
}
void logInfo(char *line)
{
    int fd1;
    fd1 = dup(1);
    int fd = open("log.txt", O_APPEND | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);    
    dup2(fd, 1);
    close(fd);
    printf("PID: %d : %s\n", getpid(),line);
    dup2(fd1, 1);
}