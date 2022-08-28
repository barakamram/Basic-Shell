#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h> 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define TRUE 1

int main(int argc, char const* argv[]){
  // create server socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5678);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(server_sock, (struct sockaddr*)&server_addr,
         sizeof(server_addr));

    // listen
    listen(server_sock, 1);

    int client_socket = accept(server_sock, NULL, NULL);

    while (recv(client_socket, strcat, sizeof(strcat), 0)){
        char data[255];
        printf("%s\n",data);
        if (!strncmp(data, "LOCAL",5)){
            return 0;
        }
    }
    return 0;
}
