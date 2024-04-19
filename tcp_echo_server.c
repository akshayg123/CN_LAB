#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define port 2000


int main()
{
	int server_socket,client_socket;
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_addr_len;
	char buffer[1024];

	//server socket creation	
	server_socket=socket(AF_INET,SOCK_STREAM,0);
	
	//server address structure
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(port);

	//bind socket to server address
	bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));


	//listen to msg
	if(listen(server_socket,5)==-1)
	{
		perror("failure");
		exit(EXIT_FAILURE);
	}
	printf("server listening%d\n",port);
	while(1)
	{
		// Accept incoming connection
		client_addr_len=sizeof(client_addr);
		if((client_socket=accept(server_socket,(struct sockaddr *)&client_addr,&client_addr_len))==-1)
		{
			perror("not accepted");
			exit(EXIT_FAILURE);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		// Echo back received data
		ssize_t bytes_received;
		while((bytes_received=recv(client_socket,buffer,1024,0))>0)
		{
			send(client_socket,buffer,bytes_received,0);
		}
		 // Close client socket
		close(client_socket);
	}
	return 0;
}



		
		

	
	
	
	
