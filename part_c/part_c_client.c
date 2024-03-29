/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

/**
 * @file part_c_client.c
 * @author Elif Kızılkaya
 
 * @brief Part_a code is upgraded to Remote Procedure Call (RPC)
 * so that server can execute the blackbox (unknown executable file) 
 * for any client that requests. To achieve this, RPCGEN is used.
 * I made edits on the automatically generated code. Client code 
 * takes the 2 integer value from stdin and send them to the server
 * beside the path. Then, it appends the result which came from the
 * server.
 * 
 * How to compile and run:
    make
    ./part_c_logger.out part_c.log PORT_NUMBER &
	./part_c_server.out LOGGER_IP_ADDRESS PORT_NUMBER &
	./part_c_client.out blackbox part_c_output.txt SERVER_IP_ADDRESS
 */

#include "part_c.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <stdio.h>
#include <unistd.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
*   This code includes examples from CMPE322 PS6 18.11.2021
*   From the files: redirect.c
*/

void
part_c_prog_1(char *host , char* path1, char* path2, int input1, int input2)
{
	CLIENT *clnt;
	char * *result_1;
	parameters  part_c_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PART_C_PROG, PART_C_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	part_c_1_arg.path = path1; //path of the executable blackbox
	part_c_1_arg.number1 = input1; //input 1
	part_c_1_arg.number2 = input2; //input 2
	result_1 = part_c_1(&part_c_1_arg, clnt); //result is coming from the server

	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	/*
    *   This part may contain examples from the following files:
    *   CMPE322 PS6 18.11.2021 redirect.c
    */
	else{

		// Opening the file with the path specified by the argument
		int file;

        file = open(path2, CREATE_FLAGS, CREATE_MODE);
        if (file == -1) {
            perror("Failed to open output gile");
            return;
        }
        if (dup2(file, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            return;
        }
        if (close(file) == -1) {
            perror("Failed to close the file");
            return;
        }

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
        sprintf(text, "%d", atoi(*result_1)); //convert to int with atoi; then convert to string with sprintf

        /*
        *   If it is integer; then the sizes are equal.
        */
        if (strlen(*result_1) == (strlen(text) + 1)) {
             printf("SUCCESS:\n%s", *result_1);
        }
        /*
        *   If it is text; then the sizes are not equal.
        */
        else {

             printf("FAIL:\n%s", *result_1);
        }



	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	/*
	* Checking the number of arguments in command line
	*/
	if (argc < 4) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[3]; //Take the IP address of the server
	int input1;
	int input2;
	scanf("%d %d", &input1, &input2); //Take the 2 integer value from stdin
	part_c_prog_1 (host, argv[1], argv[2], input1, input2); //Start RPC
exit (0);
}
