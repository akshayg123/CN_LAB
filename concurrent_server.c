#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 2000
#define MAX_PENDING_CONNECTIONS 5
#define BUFFER_SIZE 1024
#define RESPONSE_SIZE 2048 

void handle_client(int clientSocket, int clientNumber) {
    char buffer[BUFFER_SIZE];
    char response[RESPONSE_SIZE]; 
    ssize_t bytes_received;

    while ((bytes_received = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0'; 
        printf("message from client %d: %s\n", clientNumber, buffer);
        

        printf("enter response: ");
	fgets(response,RESPONSE_SIZE,stdin);
       	send(clientSocket, response, strlen(response), 0);
       	printf("response sent to Client \n");
    }

    if (bytes_received == 0) {
        printf("client %d disconnected.\n", clientNumber);
    } else {
        perror("error receiving data .");
    }

    close(clientSocket);
}

int main() {
    int welcomeSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    int clientNumber = 1; 


    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);


    if (bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    if (listen(welcomeSocket, MAX_PENDING_CONNECTIONS) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("server listening on port %d...\n", PORT);

    while (1) {

        clientSocket = accept(welcomeSocket, (struct sockaddr *)&clientAddr, &addrSize);
        if (clientSocket == -1) {
            perror("accept failed");
            continue;
        }

        printf("new client . number is : %d\n", clientNumber);


        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            close(clientSocket);
            continue;
        } else if (pid == 0) { 
            close(welcomeSocket); 
            handle_client(clientSocket, clientNumber);
            exit(EXIT_SUCCESS);
        } else { 
            close(clientSocket); 
            clientNumber++; 
        }
    }


    close(welcomeSocket);

    return 0;
}
