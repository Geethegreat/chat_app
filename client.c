#include "chat.h"

WSADATA wsa;
pthread_t t1, t2;

int socket_desc;
struct sockaddr_in server_addr;
char server_message[2000], client_message[2000];

char *hostName = "127.0.0.1";
int port = 8989;

pthread_mutex_t mutex;

void *receive()
{
  
    while (socket_desc != INVALID_SOCKET)
    {
        	pthread_mutex_lock(&mutex);
        if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
        {
            printf("Error while receiving server's msg\n");
            break;
        }
        printf("\nServer's response: %s", server_message);
        memset(client_message, '\0', sizeof(client_message));

        memset(client_message, '\0', sizeof(client_message));
        	pthread_mutex_unlock(&mutex);
    }
}

void *sendd()
{

    while (socket_desc != INVALID_SOCKET)
    {

        printf("\nEnter message:");
        scanf("%[^\n]%*c\n", client_message);

        if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
        {
            printf("Unable to send message\n");
        }

        memset(server_message, '\0', sizeof(server_message));
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

    // int socket_desc;
    // struct sockaddr_in server_addr;
    // char server_message[2000], client_message[2000];

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Unable to create socket\n");
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(hostName);

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Unable to connect %d\n", WSAGetLastError());
        return -1;
    }
    printf("Connected with server successfully\n");

    int rt = pthread_create(&t2, NULL, receive, NULL);
    if(rt != 0) {
        printf("receive thread not created");
    }

    

    int st = pthread_create(&t1, NULL, sendd, NULL);

    if(st != 0) {
        printf("send thread not created");
    } 


    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_exit(NULL);
    pthread_exit(NULL);

	pthread_mutex_destroy(&mutex);

    close(socket_desc);

    return 0;
}
