#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

void sendMessage(int sock, char *msg) {
    
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;
    char *servIP;
    char *cmd;
    char *msg;
    char *line;
    unsigned int msgLen;
    char echoBuffer[RCVBUFSIZE];
    int bytesRcvd, totalBytesRvcd;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Port>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];
    echoServPort = atoi(argv[2]);

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("socket() failed");
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port = htons(echoServPort);
    printf("connecting...");
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        DieWithError("connect() failed");
    }
    printf("done\n");

    cmd[0] = '\0';
    msg[0] = '\0';

    for (;;) {
        printf("%% ");
        fgets(cmd, 100, stdin);

        if (strcmp(cmd, "send\n") == 0) {
            printf("- Type your message. End with a blank line -\n");
            do {
                line[0] = '\0';
                fgets(line, 100, stdin);
                strcat(msg, line);
            } while (strcmp(line, "\0") != 0);

            msgLen = strlen(msg);

            if (send(sock, msg, msgLen, 0) != msgLen) {
                DieWithError("send() sent an unexpected number of bytes");
            }

            totalBytesRvcd = 0;
            printf("Received:\n");
            while (totalBytesRvcd < msgLen) {
                if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                    DieWithError("recv() failed or connection closed prematurely");
                }
                totalBytesRvcd += bytesRcvd;
                echoBuffer[bytesRcvd] = '\0';
                printf(echoBuffer);
            }
        }
    }

    /*
    char line[100];
    printf("%% ");
    fgets(line, sizeof(line), stdin);

    while ((strcmp(line, "\n") != 0) || (strcmp(line, "\n") != 0)) {       
        totalBytesRvcd = 0;
        printf("Received: ");
        while (totalBytesRvcd < ansLen) {
            if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                DieWithError("recv() failed or connection closed prematurely");
            }
            totalBytesRvcd += bytesRcvd;
            echoBuffer[bytesRcvd] = '\0';
            printf(echoBuffer);
        }

        printf("%% ");
        fgets(line, sizeof(line), stdin);
    }
    */
    printf("quitting...");
    fflush(stdout);
    close(sock);
    sleep(3);
    printf("done\n");
    exit(0);
}