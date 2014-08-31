#include <stdio.h>
#include <string.h>

int main ( int argc, char *argv[] )
{
	if(argc>0)
	{ 
		char s[2] = " ";
		char *token;
		bool outputRedirected = false;
		bool reset = true;

		token = strtok(argv[1], s);
		while(token != NULL ) 
		{
			//Taking in command, otherwise we will assume it is an argument
			if (strstr(token, "quit"))
			{
				printf("Program terminates successfully by the user\n");
				break;
			}
			else if(reset == true)
			{
				printf("The user command or program is: [%s]\n", token );
				//Execute and push results to stack?
				reset = false;
			}
			else if(strstr(token, "|"))
			{
				reset = true;
				//If there is anything on the stack then were going 
				printf("Pipe: yes\n");
			}
			else if(strstr(token,">>") || strstr(token,">") || strstr(token,"<"))
			{
				outputRedirected = true;
				printf("Output Direction: %s\n", token);
			}
			else if (outputRedirected)
			{
				if(strstr(token,">>"))
				{
					printf("Output file: %s\n", token);
				}
				else if (strstr(token,">"))
				{
					printf("Output overwritten: %s\n", token);
				}
				else if(strstr(token,"<"))
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
}