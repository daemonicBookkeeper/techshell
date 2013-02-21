#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tokenizer.h"

#define MAX_LINE_LENGTH 1000
#define ARGS_ARRAY_SIZE 100

typedef enum { ARGUMENT, COMMAND, TERMINATOR } mode;


//storage for the line and a pointer to print args/commands with


int main() {	
	int i, j;
	char * line;
	char * iter;
	aToken * working;

	//is there a pipe?
	bool pipe = false;
	
	//memory to store args and commands in
	working = malloc(sizeof(aToken));
	aToken * args[ARGS_ARRAY_SIZE];
	
	//allocating memory to the args array
	for(i = 0; i < ARGS_ARRAY_SIZE; i++) args[i] = malloc(sizeof(aToken));

	//no exit condition?
	do {	
		//matched this allocation with a free at the end of the loop
		line = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));
		if(fgets(line, MAX_LINE_LENGTH + 1, stdin) != NULL) {
			startToken(line);
			i = 0;
			
			do {			
				/*
					If token is basic, single quoted,
					or double quoted, it is a command
					or argument; store it. A pipe 
					changes the output to PIPE and the
					next input to PIPE. Pipe, 
					semicolon, or EOL all print the
					command and arguments along with
					the input/output types. 
							 			*/
				*working = getNextToken();
				switch(working->type) {
					case BASIC: *args[i] = *working; i++; break;
					case SINGLE_QUOTE: *args[i] = *working; i++; break;
					case DOUBLE_QUOTE: *args[i] = *working; i++; break;
					case PIPE:	iter = args[0]->start; 
							printf("Executing command: %s\n",iter);	
							if(pipe) printf("Input: PIPE\n");
							else { printf("Input: STDIN\n"); pipe = true; }
							printf("Output: PIPE\n");
							if(i < 2) printf("No arguments\n");
							else {
								for(j = 1; j < i; j++) {
									iter = args[j]->start;
									printf("Arg %d: %s\n",j,iter);
									}
							}
							i = 0;
							break;
							
					case SEMICOLON: 
					case EOL:	iter = args[0]->start; 
							printf("Executing command: %s\n",iter);	
							if(pipe) { printf("Input: PIPE\n"); pipe = false; }
							else printf("Input: STDIN\n");
							printf("Output: STDOUT\n");
							if(i < 2) printf("No arguments\n");
							else {
								for(j = 1; j < i; j++) {
									iter = args[j]->start;
									printf("Arg %d: %s\n",j,iter);
									}
							}
							i = 0;
							break;
					default: printf("Default case.\n"); break;
				}
			} while (working->type != EOL);

		}
		free(line);
	} while (line != NULL);
	for(i = 0; i < ARGS_ARRAY_SIZE; i++) free(args[i]);
	return 0;
}
	
