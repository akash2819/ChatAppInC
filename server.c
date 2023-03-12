#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10 // Maximum number of clients that the server can handle
#define MESSAGE_SIZE 1024
int clients[MAX_CLIENTS] = {0}; // Array to store client sockets
fd_set active_fd_set;
void remove_client(int sock, fd_set *active_fd_set);

void *connection_handler(void *);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("170.187.239.244");
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
//initialize active_fd_set
    FD_ZERO(&active_fd_set);
    FD_SET(server_fd, &active_fd_set);
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
// welcome screen 
 printf("_________ .__    .__  __            _________ .__            __   \n");
    printf("\\_   ___ \\|  |__ |__|/  |_          \\_   ___ \\|  |__ _____ _/  |_ \n");
    printf("/    \\  \\/|  |  \\|  \\   __\\  ______ /    \\  \\/|  |  \\\\__  \\\\   __\\\n");
    printf("\\     \\___|   Y  \\  ||  |   /_____/ \\     \\___|   Y  \\/ __ \\|  |  \n");
    printf(" \\______  /___|  /__||__|            \\______  /___|  (____  /__|  \n");
    printf("        \\/     \\/                           \\/     \\/     \\/      \n");
    printf("Created By Akash Sharma\n");
    printf("Waiting for incoming connections...\n");

    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        } 

        printf("New client connected\n");

        // Create a new thread to handle the connection
        pthread_t thread_id;
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &new_socket) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        // Detach the thread to allow it to run independently
        pthread_detach(thread_id);
    }

    return 0;
}

// void *connection_handler(void *socket_desc)
// {
//     int sock = *(int*)socket_desc;
//     int valread;
//     char buffer[1024] = {0};
//     char message[1024] = {0};

//     while(1) {
//         valread = read( sock , buffer, 1024);
//         printf("Client message: %s\n", buffer);
        
//         // Send the message to all the connected clients
//         for(int i = 0; i < MAX_CLIENTS; i++) {
//             if(clients[i] != 0 && clients[i] != sock) {
//                 send(clients[i], buffer, strlen(buffer), 0);
//             }
//         }
//     }

//     return NULL;
// }
void remove_client(int sock, fd_set *active_fd_set) {
    // Remove the socket from the active file descriptor set
    FD_CLR(sock, active_fd_set);

    // Close the socket
    close(sock);
}


void *connection_handler(void *socket_desc)
{
    // Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[MESSAGE_SIZE];
    
    // Send welcome message to the client
    char *welcome_message = "Welcome to the Chit-Chat a chat room!\n Created By Akash Sharma\n";
    send(sock, welcome_message, strlen(welcome_message), 0);
    
    // Receive message from the client
    while ((read_size = recv(sock , client_message , MESSAGE_SIZE , 0)) > 0 )
    {
        // Null-terminate the message received
        client_message[read_size] = '\0';
        
        // Send the message to all connected clients
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if(clients[i] != 0 && clients[i] != sock) {
                send(clients[i], client_message, strlen(client_message), 0);
            }
        }
        
        // Print the message received from the client
        printf("Client %d: %s\n", sock, client_message);
        
        // Clear the message buffer
        memset(client_message, 0, MESSAGE_SIZE);
    }
     
    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1) {
        perror("recv failed");
    }
         
    // Remove the socket from the list of connected clients
       remove_client(sock, &active_fd_set);
         
    // Free the socket descriptor
    free(socket_desc);
         
    return 0;
}

