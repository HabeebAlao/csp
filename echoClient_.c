#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[])
{
    char recvbuffer[BUFSIZE]; // I/O buffer
    int numBytes = 0;
    int numBytes2 = 0;

    if (argc < 3) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Address> <Server Port>");

    char *servIP = argv[1];
    char *echoString = argv[3];

    // First arg: server IP address (dotted quad
    in_port_t servPort = atoi(argv[2]);

    // Create a reliable, stream socket using TCP
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        DieWithSystemMessage("socket() failed");

    // Construct the server address structure
    struct sockaddr_in servAddr;            // Server address
    memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
    servAddr.sin_family = AF_INET;          // IPv4 address family

    // Convert address
    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
    if (rtnVal == 0)
        DieWithUserMessage("inet_pton () failed", "invalid address string");
    else if (rtnVal < 0)
        DieWithSystemMessage("inet_pton () failed");
    servAddr.sin_port = htons(servPort);

    // Establish the connection to the echo server
    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("connect() failed");

    char sendbuffer[BUFSIZE];
    // code to transmit what the user entered

    // clntSock is connected to a client!
    snprintf(sendbuffer, sizeof(sendbuffer), "%s\r\n", echoString);
    ssize_t numBytesSent = send(sock, echoString, strlen(echoString), 0); //Send date and time string to the client 
    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");



    // take what was echoed back from the server and print out to stdoutput
    while ((numBytes2 = recv(sock, recvbuffer, BUFSIZE - 1, 0)) > 0)
    {
        recvbuffer[numBytes2] = '\0'; // Terminate the string!
        // Print the echo buffer
        fputs(recvbuffer, stdout);
        /* Receive up to the buffer size (minus 1 to leave space for
        a null terminator) bytes from the sender */
    }


    if (numBytes2 < 0)
        DieWithSystemMessage("recv() failed");
    // else if (numBytes == 0)
    //    DieWithUserMessage ("recv()", "connection closed prematurely");
    fputc('\n', stdout); // Print a final line feed

    close(sock);
    exit(0);
}
