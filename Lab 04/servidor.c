// clear && gcc-12 servidor.c -o servidor && ./servidor

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{

    int clients[] = {9734, 9735, 9736};
    int qtd_clients = sizeof(clients) / sizeof(clients[0]);

    int sockfd[qtd_servers];
    int len[qtd_servers];
    struct sockaddr_in address[qtd_servers];
    int result[qtd_servers];

    int ch;

    // Configurando conexão Socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port);

    // Bind na conexão
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);
    while (1)
    {
        printf("server waiting\n");
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        read(client_sockfd, &ch, 1);
        printf("char from client = %i\n", ch);
        ch++;
        write(client_sockfd, &ch, 1);
        close(client_sockfd);
    }
}
