//Author: Cory McDonald
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile);
void parse_input(char* input);

int main ( int argc, char *argv[] )
{
	int returnCode = 0;
	bool active = true;
	char userCommand[256];
    //Making sure arguments were passed in
	if(argc > 1)
	{
		parse_input(argv[1]);
	}else
	{
		while(active)
		{
			printf("myshell-%% ");
			//This makes sure we don't go past the limit of 256 characters. That would be very, very bad
			fgets (userCommand, 256, stdin);
			//Removing newline from command
			if ((strlen(userCommand)>0) && (userCommand[strlen (userCommand) - 1] == '\n'))
				userCommand[strlen (userCommand) - 1] = '\0';
			if(userCommand != NULL)
			{
				parse_input(userCommand);
			}
		}
	}
	return returnCode;
}
void parse_input(char* input)
{
	//Token to replace in the arguments passed in
	char s[2] = " ";
	char *token;
	//If the output is redirected we must know
	bool isOutputRedirected = false;
	char outputRedirectedTo[3] = ""; //Could be >>,>,<
	//If the command is piped out then we will want to read in the command the user wants to complete
	bool reset = true;

	token = strtok(input, s); //Tokenizing
	while(token != NULL )
	{
		if (strstr(token, "quit") || strstr(token, "exit")) //Quiting
		{
			printf("Program terminates successfully by the user\n");
			break;
		}
		else if(reset == true) //Taking in command, otherwise we will assume it is an argument
		{
		    //If command is not found do not continue.
			printf("The user command or program is: [%s]\n", token );
			reset = false;
		}
		else if(strstr(token, "|")) //Piping include a space
		{
			reset = true;
			if(strlen(token) > 1) //more than 1 character, an argument has been attached to it
			{
				char substringToken[3];
				memcpy( substringToken, &token[0], 2 ); //Is this safe? 
				substringToken[2] = '\0';
				printf("The command line argument to the user command and program is: [%s]\n", substringToken );
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
				printf("Output file: %s\n", token);
			}
			else if (strstr(outputRedirectedTo,">"))
			{
				printf("Output overwritten: %s\n", token);
			}
			else if(strstr(outputRedirectedTo,"<"))
			{
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
int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile)
{
	//TODO not quite sure what to do here. Going to wait until Dr. Lee informs of what to do.
	//Check to see if command exists
	return 0;
}