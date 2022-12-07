#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int numOcorrencias(char *linha, char *palavra, int contador)
{
    for (int i = 0; palavra[i]; i++)
    {
        palavra[i] = tolower(palavra[i]);
    }
    for (int i = 0; linha[i]; i++)
    {
        linha[i] = tolower(linha[i]);
    }

    int cont = 0;
    int i;
    int j = 0;
    for (i = 0; i < strlen(linha); i++)
    {
        if (linha[i] == palavra[j])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        if (j == strlen(palavra))
        {
            ocorrencias_palavra_chave[contador]++;
        }
    }
    return 0;
}

int main()
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;
  char ch = 'A';

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  //address.sin_addr.s_addr = inet_addr("192.168.0.15");
  address.sin_port = htons(9734);

  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if(result == -1) {
		perror("oops: client1");
		exit(1);
  }
  palavrasPorProcesso = (argc - 1) / (world_size - 1);

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen("arquivo.txt", "r");
    if (fp == NULL)
    {
        printf("Não conseguiu abrir o arquivo %s\n", fp);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        for (int i = numeroRecebido; i < numeroLimite + 1; i++)
        {
            numOcorrencias(line, argv[i], i);
        }
    }
    for (int i = numeroRecebido; i < numeroLimite + 1; i++)
    {
        printf("[PROCESSO: %i] Palavra: %s, foi encontrada: %i vezes\n", world_rank, argv[i], ocorrencias_palavra_chave[i]);
    }
    fclose(fp);

    MPI_Finalize();
	
	exit(0);
}
