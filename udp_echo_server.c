#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 2000

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[1024];
    
    // Server socket creation    
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to server address
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Server listening on port %d\n", PORT);
    
    ssize_t bytes_received;
    while(1) {
        // Receive message from client
        client_addr_len = sizeof(client_addr);
        bytes_received = recvfrom(server_socket, buffer, 1024, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        buffer[bytes_received] = '\0';
        printf("Message from client: %s", buffer);
        
        // Echo back to client
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_addr_len);
    }
    close(server_socket);
    return 0;
}

