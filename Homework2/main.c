//Author: Cory McDonald
#include <stdio.h>
#include <string.h>

//Gotta have my bools
typedef int bool;
#define true 1
#define false 0

int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile);


int main ( int argc, char *argv[] )
{
    int returnCode = 0;
    bool active = true;
    //Making sure arguments were passed in
	if(argc>0)
	{
	    //Token to replace in the arguments passed in
		char s[2] = " ";
		char *token;
		//If the output is redirected we must know
		bool isOutputRedirected = false;
		char outputRedirectedTo[3] = ""; //Could be >>,>,<
		//If the command is piped out then we will want to read in the command the user wants to complete
		bool reset = true;
		bool pipe = false;

		token = strtok(argv[1], s); //Tokenizing
		while(token != NULL )
		{
			if (strstr(token, "quit")) //Quiting
			{
				active = false;
				printf("Program terminates successfully by the user\n");
				break;
			}
			else if(reset == true) //Taking in command, otherwise we will assume it is an argument
			{
				printf("The user command or program is: [%s]\n", token );
				reset = false;
			}
			else if(strstr(token, "|")) //Pipin'
			{
				reset = true;
				pipe =  true;
				if(pipe == true)
				{
				    returnCode = 7;
				}else
				{
				        returnCode = 4;
				}
				printf("Pipe: yes\n");
			}
			else if(strstr(token,">>") || strstr(token,">") || strstr(token,"<")) //Output redirected
			{
				isOutputRedirected = true;
				strncpy(outputRedirectedTo, token, sizeof(outputRedirectedTo));
				outputRedirectedTo[sizeof(outputRedirectedTo) - 1] = '\0';
				printf("Output Direction: %s\n", token);
			}
			else if (isOutputRedirected == true)
			{
				if(strstr(outputRedirectedTo,">>"))
				{
				    if(pipe == true)
				    {
				        returnCode = 5;
				    }else
				    {
				        returnCode = 2;
				    }
					printf("Output file: %s\n", token);
				}
				else if (strstr(outputRedirectedTo,">"))
				{
				    if(pipe == true)
				    {
				        returnCode = 6;
				    }else
				    {
				     returnCode = 3;
				    }
					printf("Output overwritten: %s\n", token);
				}
				else if(strstr(outputRedirectedTo,"<"))
				{
				    returnCode = 1;
					printf("Input: %s\n", token);
				}
			}
			else
			{
				printf("The command line argument to the user command and program is: [%s]\n", token );
			}
			
			token = strtok(NULL, s);
		}
	}
	return returnCode;
}

int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile)
{
	//TODO not quite sure what to do here. Going to wait until Dr. Lee informs of what to do.
	//Check to see if command exists
    return 0;
}