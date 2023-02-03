/**
 * @file part_a.cpp
 * @author Elif Kızılkaya
 *
 * @brief This code is designed to catch the output of
 * an unknown executable file. Main program creates a
 * child process. It takes input from stdin and pass 
 * it to the child via pipe. The child executes the executable
 * file with the given input. Afterwards, main program recevies
 * the output via pipe and appends it into the specified file.
 *
 *
 * 
 * How to compile and run:
    make
    /part_a.out blackbox part_a_output.txt
 */

/*
*   JUST A SIMPLE NOTE:
*       I gave references to various websites, even if I did not 
        take any codes from them, I wanted to give reference since
        I looked at them while writing my code.
*/

/*   
*   ADITIONAL REFERENCES:
*   How to empty a char array? Stack Overflow. (1957, November 1). 
    Retrieved December 16, 2021, from https://stackoverflow.com/questions/1559487/how-to-empty-a-char-array 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
*   This code is originated from CMPE322 PS6 18.11.2021
*   From the files: io_capture.c & bidirectional_pipe.c & redirect.c
*/

int main(int argc, char *argv[]) {
    /*
    *   This part may contain examples from the following files:
    *   CMPE322 PS6 18.11.2021 io_capture.c & bidirectional_pipe.c
    */

    /**
     * p2c -> parent to child pipe
     * c2p -> child to parent pipe
     */

    int     p2c[2], c2p[2], nbytes;
    pid_t   pid; //process id
    char w_buffer[1000], r_buffer[10000]; //write buffer & read buffer

    /* checking for valid number of command-line arguments */  
    if (argc != 3){   
        fprintf(stderr, "More arguments are expected\n");
        return 1; 
   }     

    /**
     * The parent creates both pipes, which will be shared between the parent and the child.
     */
    pipe(p2c);
    pipe(c2p);

    if((pid=fork()) == -1) {
        fprintf(stderr, "fork() failed.\n");
        exit(-1);
    }

    if(pid > 0) {
        /**
         * This is the code for the parent process.
         */

        /**
         * The parent should close the ends of the pipes that it will not use.
         */
        close(p2c[0]);    // Parent will not read from p2c
        close(c2p[1]);    // Parent will not write to c2p      

        /**
         * 2 integer values are taken from stdin: input1 & input2
         */

        int input1; 
        int input2; 
        scanf("%d %d", &input1, &input2);
        
        /**
         * Send a request message to the child process.
         * Write message to the buffer, buf1
         */

        char buf1[256];
        sprintf(buf1, "%d %d", input1, input2);
        write(p2c[1], buf1, (strlen(buf1) + 1));
    

        /*
        *   This part may contain examples from following file:
        *   CMPE322 PS6 18.11.2021  redirect.c
        *
        *   Opening the file with the path specified by the arguments
        */

        int file;

        file = open(argv[2], CREATE_FLAGS, CREATE_MODE);
        if (file == -1) {
            perror("Failed to open output file");
            return 1;
        }
        if (dup2(file, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            return 1;
        }
        if (close(file) == -1) {
            perror("Failed to close the file");
            return 1;
        }
        
        /*
        *   Reading the response from the child to r_buffer 
        *   varibale. The child return an error text or an 
        *   integer value as the output of the executable file. 
        *  
        *   First, we empty the buffer with memset() function.
        */
        memset(r_buffer, 0, 10000);
        nbytes = read(c2p[0], r_buffer, sizeof(r_buffer));
        

        /*  This part is checking whether the response from child
         *  is error or integer.
         *   
         *  If it is integer, then converting to int and then converting
         *  to string would not effect the size. However, converting a text
         *  to int would make the size of the r_buffer 1.
         *  That is why, 1 is added in the if condition.
         *
         *   I utilized from the idea from this forum:
         *   https://stackoverflow.com/questions/16644906/how-to-check-if-a-string-is-a-number
         */

        char text[1000];
        sprintf(text, "%d", atoi(r_buffer)); //convert to int with atoi; then convert to string with sprintf

        /*
        *   If it is integer; then the sizes are equal.
        */
        if (strlen(r_buffer) == (strlen(text) + 1)) {
             printf("SUCCESS:\n%s", r_buffer);
        }

        /*
        *   If it is text; then the sizes are not equal.
        */
        else {

             printf("FAIL:\n%s", r_buffer);
        }
    }
        /*
        *   This part may contain examples from the following file:
        *   CMPE322 PS6 18.11.2021 io_capture.c
        */
    else {
        /**
         * This is the code for the child process.
         */
        char* path = argv[1]; //Path of the binary file blackbox
       
        /**
         * The child associates the read end of p2c with its own stdin.
         */
        dup2(p2c[0], STDIN_FILENO);
       
        /**
         * The child associates the write end of c2p with its own stdout and stderr.
         */
        dup2(c2p[1], STDOUT_FILENO);
        dup2(c2p[1], STDERR_FILENO);

        /**
         * The child can close all pipes since they are not needed anymore.
         */
        close(c2p[0]);
        close(c2p[1]);
        close(p2c[0]);
        close(p2c[1]);
        
        /*
        *   Replace the contents of the child process with the
        *   given executable file.
        *   path -> path of the executable
        *   ./blackbox -> command to execute the file
        */
        execl(path, path, NULL);    
    }

    return(0);
}

