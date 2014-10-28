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
void unitTest();


int main ( int argc, char *argv[] )
{
    bool debug = true;
    if(debug == true)
    {
        unitTest();
    }
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
        printf("todo \n");
        break;
      case 5:
        exec_pipe(cmd1, cmd2);
        break;
      case 6:
        exec_pipe_in(cmd1, cmd2,infile);
        break;
      default:
        printf("Not handled at this time");
        break;
    }
    if (i < 9)
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

void unitTest()
{
    // Bad practices ftw
    char infile[CSTRSIZE];
    char outfile[CSTRSIZE];
    char *cmd1[CMDSIZE];
    char *cmd2[CMDSIZE];
    int i;
    int success =0;

    cmd1[0] = NULL;
    cmd2[0] = NULL;
    
    char returnCode1Test1[50] = "ls";
    char returnCode1Test2[50] = "ls –l";
    char returnCode1Test3[50] = "ls –l –a";

    char returnCode2Test1[50] = "wc < filename";

    char returnCode3Test1[50] = "ls >> outputfile";
    char returnCode3Test2[50] = "ls –l >> outputfile";
    char returnCode3Test3[50] = "ls –l –a >> outputfile";
    char returnCode3Test4[50] = "wc < filename >> outputfile";

    char returnCode4Test1[50] = "ls > outputfile";
    char returnCode4Test2[50] = "ls –l > outputfile";
    char returnCode4Test3[50] = "ls –l –a > outputfile";
    char returnCode4Test4[50] = "wc < filename > outputfile";


    char returnCode5Test1[50] = "ls | grep c";
    char returnCode5Test2[50] = "ls –l | grep c";
    char returnCode5Test3[50] = "ls –l –a | grep c";

    char returnCode6Test1[50] = "wc < filename | grep 3";

    char returnCode7Test1[50] = "ls | grep c >> outputfile";
    char returnCode7Test2[50] = "ls –l | grep c >> outputfile";
    char returnCode7Test3[50] = "ls –l –a | grep c >> outputfile";
    char returnCode7Test4[50] = "wc < filename | grep 3 >> outputfile";

    char returnCode8Test1[50] = "ls | grep c > outputfile";
    char returnCode8Test2[50] = "ls –l | grep c > outputfile";
    char returnCode8Test3[50] = "ls –l –a | grep c > outputfile";
    char returnCode8Test4[50] = "wc < filename | grep 3 > outputfile";

    char returnCode9Test1[50] = "cat file1 | grep c | wc";
    char returnCode9Test2[50] = "netbeans&";


    i = parse_command(returnCode1Test1, cmd1, cmd2, infile, outfile);
    if(i==1) { success++; } else { printf("%s test failed. 1 != %d\n", returnCode1Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode1Test2, cmd1, cmd2, infile, outfile);
    if(i==1) { success++; } else { printf("%s test failed. 1 != %d\n", returnCode1Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode1Test3, cmd1, cmd2, infile, outfile);
    if(i==1) { success++; } else { printf("%s test failed. 1 != %d\n", returnCode1Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    i = parse_command(returnCode2Test1, cmd1, cmd2, infile, outfile);
    if(i==2) { success++; } else { printf("%s test failed. 2 != %d\n", returnCode2Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    i = parse_command(returnCode3Test1, cmd1, cmd2, infile, outfile);
    if(i==3) { success++; } else { printf("%s test failed. 3 != %d\n", returnCode3Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode3Test2, cmd1, cmd2, infile, outfile);
    if(i==3) { success++; } else { printf("%s test failed. 3 != %d\n", returnCode3Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode3Test3, cmd1, cmd2, infile, outfile);
    if(i==3) { success++; } else { printf("%s test failed. 3 != %d\n", returnCode3Test3 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode3Test4, cmd1, cmd2, infile, outfile);
    if(i==3) { success++; } else { printf("%s test failed. 3 != %d\n", returnCode3Test4 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    i = parse_command(returnCode4Test1, cmd1, cmd2, infile, outfile);
    if(i==4) { success++; } else { printf("%s test failed. 1 != %d\n", returnCode4Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode4Test2, cmd1, cmd2, infile, outfile);
    if(i==4) { success++; } else { printf("%s test failed. 1 != %d\n", returnCode4Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode4Test3, cmd1, cmd2, infile, outfile);
    if(i==4) { success++; } else { printf("%s test failed. 1 != %d\n", returnCode4Test3 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode4Test4, cmd1, cmd2, infile, outfile);
    if(i==4) { success++; } else { printf("%s test failed. 4 != %d\n", returnCode4Test4 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    i = parse_command(returnCode5Test1, cmd1, cmd2, infile, outfile);
    if(i==5) { success++; } else { printf("%s test failed. 5 != %d\n", returnCode5Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode5Test2, cmd1, cmd2, infile, outfile);
    if(i==5) { success++; } else { printf("%s test failed. 5 != %d\n", returnCode5Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode5Test3, cmd1, cmd2, infile, outfile);
    if(i==5) { success++; } else { printf("%s test failed. 5 != %d\n", returnCode5Test3 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    i = parse_command(returnCode6Test1, cmd1, cmd2, infile, outfile);
    if(i==6) { success++; } else { printf("%s test failed. 6 != %d\n", returnCode6Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    i = parse_command(returnCode7Test1, cmd1, cmd2, infile, outfile);
    if(i==7) { success++; } else { printf("%s test failed. 7 != %d\n", returnCode7Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode7Test2, cmd1, cmd2, infile, outfile);
    if(i==7) { success++; } else { printf("%s test failed. 7 != %d\n", returnCode7Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode7Test3, cmd1, cmd2, infile, outfile);
    if(i==7) { success++; } else { printf("%s   test failed. 7 != %d\n", returnCode7Test3 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode7Test4, cmd1, cmd2, infile, outfile);
    if(i==7) { success++; } else { printf("%s test failed. 7 != %d\n", returnCode7Test4 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;


    i = parse_command(returnCode8Test1, cmd1, cmd2, infile, outfile);
    if(i==8) { success++; } else { printf("%s test failed. 8 != %d\n", returnCode8Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode8Test2, cmd1, cmd2, infile, outfile);
    if(i==8) { success++; } else { printf("%s test failed. 8 != %d\n", returnCode8Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode8Test3, cmd1, cmd2, infile, outfile);
    if(i==8) { success++; } else { printf("%s test failed. 8 != %d\n", returnCode8Test3 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode8Test4, cmd1, cmd2, infile, outfile);
    if(i==8) { success++; } else { printf("%s test failed. 8 != %d\n", returnCode8Test4 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    i = parse_command(returnCode9Test1, cmd1, cmd2, infile, outfile);
    if(i==9) { success++; } else { printf("%s test failed. 9 != %d\n", returnCode9Test1 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;
    i = parse_command(returnCode9Test2, cmd1, cmd2, infile, outfile);
    if(i==9) { success++; } else { printf("%s test failed. 9 != %d\n", returnCode9Test2 ,i); } i=0;cmd1[0] = NULL; cmd2[0] = NULL;

    printf("%d/26 provided tests completed successfully\n", success);
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
                    returnCode = 5;
                    
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
                // printf("The command line argument to the user command and program is: [%s]\n", substringToken );
            }
            // printf("Pipe: yes\n");
        }
        else if (strstr(token, ">>") || strstr(token, ">") || strstr(token, "<")) //Output redirected
        {
            isOutputRedirected = true;
            strncpy(outputRedirectedTo, token, sizeof(outputRedirectedTo));
            outputRedirectedTo[sizeof(outputRedirectedTo) - 1] = '\0';
            // printf("Output Direction: %s\n", token);
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
                strcpy(infile, token);
                if (pipe == true)
                {
                    returnCode = 6;
                }else
                {
                    returnCode = 2;
                }
            }
        }
        else
        {
            if (pipe == true)
            {
                cmd2[cmd2Index] =  token + '\0';
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
            int outFD = open(outfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            if(infile[0] != '\0')
            {
                int inFD = open(infile, O_RDONLY);
                dup2(inFD, 0);
            }
            dup2(outFD, 1);
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

}
void exec_pipe(char** cmd1, char** cmd2)
{
  pid_t firstFork;
  pid_t secondFork;
  pid_t pid2;
  int pipefd[2];
  
  if ((firstFork = fork()) == -1)
  {
      perror("fork error");
  }
  else if (firstFork == 0)
  {
    //This will execute the second command
    if ((secondFork = fork()) == -1)
    {
        perror("fork error");
    }else if(secondFork == 0)
    {
      dup2(pipefd[0], 0); // replace standard input with input of cmd1
      close(pipefd[1]); //Close standard output of the pipe
      execvp(cmd2[0], cmd2);
      printf("%s: command not found\n", cmd1[0]);
    }else
    {
      //Wait for the fork before continuing
      waitpid(secondFork, NULL, 0);
    }
    
    // replace standard output with output part of pipe
    dup2(pipefd[1], 1); //So the output of CMD1 will be redirected
    // close unused input half of pipe
    close(pipefd[0]);
    
    execvp(cmd1[0], cmd1);
    printf("%s: command not found\n", cmd1[0]);
      
  }else
  {
    // Parent process, wait for the firstFork and secondFork to finish
    waitpid(firstFork, NULL, 0);
  }
  
}
void exec_pipe_in(char** cmd1, char** cmd2, char* infile)
{
    
}
void exec_pipe_opt_in_append(char** cmd1,char** cmd2,char* infile,char* outfile)
{
  
}
void exec_pipe_opt_in_write(char** cmd1,char** cmd2,char* infile,char* outfile)
{
  
}