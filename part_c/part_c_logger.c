/**
 * @file part_c_logger.c
 * @author Elif Kızılkaya
 
 * @brief Logger program receives the result found by the
 * part_c_server through the given port and logs them into
 * the specified file. First, it creates a sockets and binds
 * it. Afterwards, it listens the client (part_c_server) and
 * accepts the connection. Then, it reads the message and
 * appends it into the log file.
 *
 * How to compile and run:
    make
    ./part_c_logger.out part_c.log PORT_NUMBER &
    ./part_c_server.out LOGGER_IP_ADDRESS PORT_NUMBER &
    ./part_c_client.out blackbox part_c_output.txt SERVER_IP_ADDRESS
 */

/*  
    REFERENCES:
*   Socket programming in C/C++. GeeksforGeeks. (2019, May 31). 
    Retrieved December 15, 2021, from https://www.geeksforgeeks.org/socket-programming-cc/     

*   TCP server-client implementation in C. GeeksforGeeks. (2021, September 23). 
    Retrieved December 16, 2021, from https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/ 

*   C program not writing to text file. Stack Overflow. 
    Retrieved December 16, 2021, from https://stackoverflow.com/questions/33763921/c-program-not-writing-to-text-file

*   C Library Function - fopen(). (n.d.). 
    Retrieved December 16, 2021, from https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm  
    
*   Connect to a socket multiple times. Stack Overflow. (1961, February 1). 
    Retrieved December 16, 2021, from https://stackoverflow.com/questions/14114812/connect-to-a-socket-multiple-times 
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int main(int argc, char const *argv[])
{

/*
*   Opening the file to be written
*   
*   I utilized from this website:
*   https://stackoverflow.com/questions/14114812/connect-to-a-socket-multiple-times
*
*   While writing the code; I encounter a problem and
*   solved the problem by utilizing this website:
*   https://stackoverflow.com/questions/33763921/c-program-not-writing-to-text-file
*/
   FILE * output;
   output = fopen (argv[1], "a+");
   if(!output){
    perror("Cannot open the file");
   }
   
/*
*   Server side of the TCP socket.
*
*   I utilized from this website:
*   https://www.geeksforgeeks.org/socket-programming-cc/
*/
    int server_sckt, buf_socket, valread; //Server socket variables
    struct sockaddr_in addr; //Address of the server side (logger)
    int opt = 1;
    int addrlen = sizeof(addr); //address size
    char buffer[1024] = {0}; //buffer to receive the message

       
    /*
    *    Create socket file descriptor
    */
    if ((server_sckt = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket could not be established");
        return -1;
    }
       
    /*
    *   To achieve that port and address can be reused
    *   this function is required.
    */
    if (setsockopt(server_sckt, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Setsockopt gives error");
        return -1;
    }
    addr.sin_family = AF_INET; // (IPv4 protocol)
    addr.sin_addr.s_addr = INADDR_ANY; //ip address
    addr.sin_port = htons(atoi(argv[2])); //port
       
    /*
    *   Attaching the socket to the port.
    */
    if (bind(server_sckt, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind failed");
        return -1;
    }
   
/*
*    Server listens, accepts and reads the message came from the kernel
*    in infinite loop after creating and binding the socket; because the 
*    message can be sent at any time.
*
*    While writing the code, I encounter a problem and solved it
*    by utilizing the following website:    
*    https://stackoverflow.com/questions/14114812/connect-to-a-socket-multiple-times
*/

    while(1){
        /*
        *   Server listens to client
        */
        if (listen(server_sckt, 1) < 0)
    {
        perror("Logger is listening...");
        return -1;
    }
    /*
    *   To establish the connection between the server (client for socket) 
    *   and logger (server for socket), logger accepts so that the data can
    *   be transferred.
    */
        if ((buf_socket = accept(server_sckt, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) < 0)
    {
        perror("Logger tried to accept...");
        return -1;
    }
    
    /*
    *   Empty the buffer, because new data is coming
    */
    bzero(buffer, sizeof(buffer));
    /*
    *   Read the message coming from the client
    */
    valread = read(buf_socket , buffer, 1024);
    /*
    *   Write the message (output) to the output file
    */
    fprintf(output, "%s", buffer);
    fflush(output); //We finished the writing
     /*
    *   Empty the buffer, because new data can come
    */
    bzero(buffer, sizeof(buffer));
    }

fclose(output); //close the file
    return 0;
}