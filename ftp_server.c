#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    int welcomeSocket, newSocket;
    char buffer[1024];
    char filename[256];
    FILE *file;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (listen(welcomeSocket, 5) == 0)
        printf("listening\n");
    else {
        printf("error\n");
        return 1; 
    }
    addr_size = sizeof(serverStorage);
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    

    size_t filename_len;
    recv(newSocket, &filename_len, sizeof(filename_len), 0);
    

    recv(newSocket, filename, filename_len, 0);
    filename[filename_len] = '\0'; 
    printf("received file: %s\n", filename);
    

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("error opening file.\n");
        close(newSocket);
        close(welcomeSocket);
        return 1;
    }
    

    printf("contents (%s):\n", filename);
    

    ssize_t bytes_received;
    while ((bytes_received = recv(newSocket, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file); 
        printf("%.*s", (int)bytes_received, buffer); 
    }
    
    if (bytes_received == -1) {
        perror("error receiving file content");
    }
    
    fclose(file);
    close(newSocket);
    close(welcomeSocket);

    return 0;
}
