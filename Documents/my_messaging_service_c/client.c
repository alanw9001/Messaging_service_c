#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUFSIZE 32

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in servAddr;
    unsigned short servPort = 5000;
    char *servIP = "localhost";
    //char *string;
    char *buffer[BUFSIZE];
    unsigned int stringLen;
    int bytesRcvd, totalBytesRvcd;

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("socket() failed");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);
    servAddr.sin_port = htons(servPort);

    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        DieWithError("connect() failed");
    }

    

    return 0;
}