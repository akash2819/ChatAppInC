#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *input_handler(void *);
void *receive_handler(void *);

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect the socket to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
printf("_________ .__    .__  __            _________ .__            __   \n");
    printf("\\_   ___ \\|  |__ |__|/  |_          \\_   ___ \\|  |__ _____ _/  |_ \n");
    printf("/    \\  \\/|  |  \\|  \\   __\\  ______ /    \\  \\/|  |  \\\\__  \\\\   __\\\n");
    printf("\\     \\___|   Y  \\  ||  |   /_____/ \\     \\___|   Y  \\/ __ \\|  |  \n");
    printf(" \\______  /___|  /__||__|            \\______  /___|  (____  /__|  \n");
    printf("        \\/     \\/                           \\/     \\/     \\/      \n");
    printf("Connected to server\n");

    // Create threads to handle input and receive
    pthread_t input_thread_id, receive_thread_id;

    if (pthread_create(&input_thread_id, NULL, input_handler, (void*) &sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    if (pthread_create(&receive_thread_id, NULL, receive_handler, (void*) &sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    // Wait for threads to complete
    pthread_join(input_thread_id, NULL);
    pthread_join(receive_thread_id, NULL);

    return 0;
}

void *input_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    char message[1024] = {0};

    while(1) {
        printf("Enter message: ");
        fgets(message, 1024, stdin);
        send(sock , message , strlen(message) , 0 );
    }

    return NULL;
}

void *receive_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    int valread;
    char buffer[1024] = {0};

    while(1) {
        valread = read( sock , buffer, 1024);
        printf("%s\n", buffer);
    }

    return NULL;
}
