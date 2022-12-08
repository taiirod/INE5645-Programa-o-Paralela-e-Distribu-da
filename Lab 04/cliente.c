// clear && gcc-12 cliente.c -o cliente && ./cliente

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

    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    int ch = 1;

    for (int i = 0; i < qtd_clients; i++)
    {
        printf("Digite a porta desse servidor: ");
        scanf("%d", &server_port);
    }
    

    for (int i = 0; i <= 7; i++)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(9734);

        len = sizeof(address);

        result = connect(sockfd, (struct sockaddr *)&address, len);

        if (result == -1)
        {
            perror("oops: client1");
            exit(1);
        }
        write(sockfd, &ch, 1);
        read(sockfd, &ch, 1);
        printf("char from server = %i\n", ch);
        close(sockfd);
    }
    exit(0);
}
