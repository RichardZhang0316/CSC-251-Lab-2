/*
* Richard Zhang
* CSC 251
* Lab 2
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// define the maximum length of a command
#define maxLength 1024

// function declarations go here
void convert(char* input, char** argv);

int main() {
    char input[maxLength];
    // we set the size of argv to 100 to ensure it's long enough
    char* argv[100];
    // the number of arguments in a command
    int argc;
    // the pid of a process
    pid_t pid;
    // the index implying if a command need to be executed concurrently
    int background;

    // the initial prompt
    printf("Welcome to the shell!\nI cannot be terminated using ^C. Please type 'exit' if you want to exit.\n");

    // enter the infinite loop to mimic a shell
    while (1) {
        background = 0;
        // prompt
        printf("shell >> ");
        // get the input
        fgets(input, maxLength, stdin);
        // the only way to exit the program is writing exit in the command
        if (strcmp(input, "exit\n") == 0) {
            // terminate the program
            return 0;
        }

        // firstly, check if the command need to be executed concurrently
        if (input[strlen(input) - 2] == '&') {
            background = 1;
            // we want to remove the & symbol to ensure the normal execution of the program
            input[strlen(input) - 2] == '\0';
        }

        // convert the command to an array of arguments
        convert(input, argv);

        // fork and see is there is an child process
        pid = fork();
        if (pid < 0) {
            // this implies there is an error
            perror("fork() failed");
            exit(1);
        } else if (pid == 0) {
            // this implies this is a child process
            // in this case, we run the execvp() function. In most cases, the first argument in a command is the function to be called, and the rest is the function's arguments, such as flags
            if (execvp(argv[0], argv) < 0) {
                // this means the execution is failed
                perror("execvp() failed");
                exit(2);
            }
        } else {
            // this implies this is the parent process
            if (background == 0) {
                // this means we need to wait for the child process to complete itself. We don't want to get the child's info, so we just enter NULL for the argument
                wait(NULL);
            }
        }
        
    }
    return 0;
}

// this function converts a command to an array of arguments
void convert(char* input, char** argv) {
    // we need to traverse the input, so we first set the position index to 0. This means we will starts from 0
    int index = 0;
    char* argument;
    // we want to use space as our delimiter to split a command
    // firstly, we need to get the first argument in a command. The second argument of strtok() shows all the possible delimiter in a command, including space, tab, etc.
    argument = strtok(input, " \t\r\n");
    while (argument != NULL) {
        argv[index] = argument;
        index++;
        // get the next argument from the rest of the command
        argument = strtok(NULL, " \t\r\n");
    }
    // for the last cell of argv, we want to insert NULL to it to mark it's the end of the variable
    argv[index] = NULL;
}
