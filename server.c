#include "chat.h"

WSADATA wsa;
SOCKET s, new_socket, new_socket1;
struct sockaddr_in server, client;
int c;
char *hostName = "127.0.0.1";
int port = 8989;
char client_message[2000], message[2000];
pthread_t t1, t2, t3;
pthread_mutex_t mutex;

void *receive()
{
	while (new_socket != INVALID_SOCKET)
	{
		
		if(recv(new_socket, client_message, sizeof(client_message), 0) < 0)
		{
			printf("Error while receiving client's msg\n");
			break;
		}
		printf("\nClinet's message: %s\n", client_message);

		memset(message, '\0', sizeof(message));
		memset(client_message, '\0', sizeof(client_message));
	

	}
}

void *connectt(void *t)
{
	
	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		
		puts("\nConnection accepted\n");
		if ((pthread_create(&t3, NULL, receive, NULL)) != 0)
		{
			printf("receive thread not created");			
		}
		break;
	}

}

void *sendd()
{
	while (new_socket != INVALID_SOCKET)
	{
		
        printf("\nEnter message: ");
        fgets(message, sizeof(message), stdin);

        // Remove the newline character from input
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n')
        message[len - 1] = '\0';

		send(new_socket, message, strlen(message), 0);

		memset(message, '\0', sizeof(message));
		memset(client_message, '\0', sizeof(client_message));
	}
}



int main(int argc, char *argv[])
{	
	pthread_mutex_init(&mutex, NULL);

	if (argc > 1)
		hostName = argv[1];
	if (argc > 2)
		port = atoi(argv[2]);

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	// Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	// Prepare the sockaddr_in structures
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(hostName);
	server.sin_port = htons(port);

	// Bind
	if ((bind(s, (struct sockaddr *)&server, sizeof(server))) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	// Listen to incoming connections
	listen(s, 3);

	// Accept and incoming connection
	puts("Waiting for incoming connections...");

	if ((pthread_create(&t1, NULL, connectt, NULL)) != 0)
	{
		return 1;
	}


	if ((pthread_create(&t2, NULL, sendd, NULL)) != 0)
	{
		printf("send thread not created");		
		return 1;
	}



	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	pthread_exit(NULL);
	pthread_exit(NULL);


	pthread_mutex_destroy(&mutex);
	closesocket(new_socket);

	WSACleanup();

	return 0;
}
