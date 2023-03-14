#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define PORT 8080
#define MAX_CLIENTS 10 // Maximum number of clients that the server can handle
#define MESSAGE_SIZE 1024
#define client_count 0
int clients[MAX_CLIENTS] = {0}; // Array to store client sockets
fd_set active_fd_set;
void remove_client(int sock, fd_set *active_fd_set);

void *connection_handler(void *);
void sigpipe_handler(int signum)
{
    // Do nothing
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
  signal(SIGPIPE, sigpipe_handler);

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
    address.sin_addr.s_addr = INADDR_ANY;
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
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        } 
        printf("New client connected %d\n",new_socket -3);
        clients[new_socket-3]=new_socket;
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
    // char *welcome_message = "Welcome to the Chit-Chat a chat room!\n Created By Akash Sharma\n";
    // send(sock, welcome_message, strlen(welcome_message), 0);
    
    // Receive message from the client
    while ((read_size = recv(sock , client_message , MESSAGE_SIZE , 0)) > 0 )
    {
        // Null-terminate the message received
        client_message[read_size] = '\0';
        
        // Send the message to all connected clients
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if(clients[i] != 0 && clients[i] != sock) {
                char main_message[1034];
                sprintf(main_message, "\nClient:%d ", sock-3);
                 strcat(main_message, client_message);
                send(clients[i], main_message ,strlen(main_message), 0);
                // printf("Client %d: %s\n",sock-3,client_message);
            }
        }
        
        // Print the message received from the client
        printf("Client %d: %s\n",sock-3,client_message);
        
        // Clear the message buffer
        memset(client_message, 0, MESSAGE_SIZE);
    }
     
    if(read_size == 0) {
        printf("Client %d Dissconected \n ",sock-3);
        fflush(stdout);
    }
    else if(read_size == -1) {
        perror("recv failed");
    }
         
    // Remove the socket from the list of connected clients
       remove_client(sock, &active_fd_set);
         
    // Free the socket descriptor
    // free(socket_desc);
         
    return 0;
}
