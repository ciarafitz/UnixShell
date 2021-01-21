/***************************************
	File:	main.c

	Author:	Ciara Fitzpatrick

	Date: September 5th, 2020

	Task: LSH is a simple implementation of a shell in C
	      Actions: Read, parse, fork, exec, & wait
		
		Builtins: cd, help. exit

***************************************/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***************************************
h: is the header in the C standard library for the C programming 
language which contains macro definitions, constants & declarations
of functions & types 

Shell: program that interprets commands so the OS can understand
	& run accordingly
***************************************/

/* Builtins Functon Declarations */

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/* Builtin commands */

char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char**) = {
	&lsh_cd,
	&lsh_help,
	&lsh_exit
};

int lsh_num_builtins(){
	return sizeof(builtin_str) / sizeof(char *);
}

////* Builtin function implementations *////

/* Builtin command:change directory 
	args[0] is 'cd', args[1] is directory,
	Return 1 to continue executing */

int lsh_cd(char **args)
{
	if(args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	} else{
		if (chdir(args[1]) != 0){
			perror("lsh");
		}
	}
	return 1;
}

/* Builtin command: print help
	args not defined, 
	Return 1 to continue exiting */

int lsh_help(char **args)
{
	int i;
	printf("Ciara Fitzpatrick's LSH\n");
	printf("Please type program names and arguments. Press enter to continue.\n");
	printf("The builtins are as followed:\n");

	for (i = 0; i < lsh_num_builtins(); i++){
		printf("   %s\n", builtin_str[i]);
	}

	printf("The man command is useful for information on other programs.\n");
	return  1;

} 

/* Builtin command: exit
	args not defined
	Return 0 to TERMINATE */

int lsh_exit(char **args)
{
	return 0;
}


/* Launchg program, wait for it to terminate */

int lsh_launch(char **args)
{
	pid_t pid;	//PID = Process ID of the parent
	int status;

	pid = fork();
	if (pid == 0) {
		//Child process
		if (execvp(args[0],args) == -1){
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		//Error forking
		perror("lsh");
	} else{
		// Parent proces
		do {
			waitpid(pid, &status, WUNTRACED);
		}	while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

/**
	Execute shell built in or launch program
	Argument Null terminated list of arguments
	Return 1 if shell is continuing to run, 0 for terminate
**/

int lsh_execute(char **args)
{
	int i;

	if(args[0] == NULL) {
		// If any empty command is inputted
		return 1;
	}
	for ( i = 0; i < lsh_num_builtins(); i++){
		if(strcmp(args[0], builtin_str[i]) == 0){
			return (*builtin_func[i])(args);
	    }
	}
	
	return lsh_launch(args);

}

/**
	Read input line from stdin
	Return line from the stdin

**/
char *lsh_read_line(void)
{
#ifdef LSH_USE_STD_GETLINE
	char *line = NULL;
	ssize_t bufsize = 0; //using getline to allocate a buffer
	if (getline(&line, &bufsize, stdin) == -1){
		if (feof(stdin)) {
			exit (EXIT_SUCCESS); //EOF: end of file
		} else {
			perror("lsh: getline\n");
			exit(EXIT_FAILURE);
		}
	}
	return line;
#else
#define LSH_RL_BUFSIZE 1024
	int bufsize = LSH_RL_BUFSIZE;
	int position= 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer){
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);  // Operating System not having the resources 
							// to allocate any more memory for the simulation
	}


	while (1) {
	// Read in a character 
		c = getchar();

		if (c == EOF) {
			exit(EXIT_SUCCESS); //if c = EOF then exit success
		} else if (c == '\n') {
			buffer[position] = '\0'; 
			return buffer;	//if not then c is new line, and pointer set to null 0 
		} else {
			buffer[position] = c; // if not then c set to getchar
		}
		position++;

		// Reallocate if buffer is exceeeded
		if (position >= bufsize) {
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer){
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
#endif

}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/**
	Split a line into tokens
	Return null terminate arrawy of tokens
**/
char **lsh_split_line(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token, **tokens_backup;

	if(!tokens){
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while(token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize){
			bufsize += LSH_TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if(!tokens){
				free(tokens_backup);
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}


/** Loop to get input and execute it **/
void lsh_loop(void)
{
	char *line;
	char **args;
	int status;

	do{
		printf("> ");
		line = lsh_read_line();
		args = lsh_split_line(line);
		status = lsh_execute(args);


		free(line);
		free(args);
	} while (status);
}

int main(int argc, char **argv)
{
	//load in files

	//run loop
	lsh_loop();

	//shutdown/cleanup

	return EXIT_SUCCESS;

}





