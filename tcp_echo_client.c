#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define ip "127.0.0.2"
#define port 2000

int main()
{
	int client_socket;
	struct sockaddr_in server_addr;
	char buffer[1024];
	
	//create tcp client socket	
	client_socket=socket(AF_INET,SOCK_STREAM,0);
	
	//server address structure
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	if(inet_pton(AF_INET,ip,&server_addr.sin_addr)<=0)
	{
		perror("invalid");
		exit(EXIT_FAILURE);
	}

	connect(client_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
	printf("connected\n");
	
	ssize_t bytes_received;
	while(1)
	{
		printf("enter message:");
		fgets(buffer,1024,stdin);
		
		send(client_socket,buffer,strlen(buffer),0);

		bytes_received=recv(client_socket,buffer,1024,0);			
		
		buffer[bytes_received] = '\0';
		printf("echo from server:%s",buffer);
	}
	close(client_socket);
	return 0;
}





	








