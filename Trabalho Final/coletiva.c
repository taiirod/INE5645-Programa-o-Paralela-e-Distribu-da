// mpicc pontoAPonto.c -o pontoAPonto
// Metade 13 palavras
//      mpiexec -np 2 ./coletiva All And Boy Book Call Car Chair Children City Dog Door Enemy End
// Original 25 palavras
//      mpiexec -np 2 ./coletiva All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh
// Dobro 50 palavras
//      mpiexec -np 2 ./coletiva All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh Listen Man Name Never Next New Noise Often Pair Pick Play Room See Sell Sit Speak Smile Sister Think Then Walk Water Work Write Woman Yes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <ctype.h>
#include <sys/time.h>

#define TAMANHO_MAXIMO 100

struct timeval t1, t2;
int palavrasPorProcesso;
int numerosRecebidos[TAMANHO_MAXIMO];
int ocorrencias_palavra_chave[0];


void printarTempoDeExecucao()
{
    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
    printf("\n");
    printf("----------------------------------------------------------\n");
    printf("Tempo total de execução [COLETIVA] = %f\n", t_total);
    printf("----------------------------------------------------------\n");
}

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

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (world_size < 2)
    {
        fprintf(stderr, "Must at least two processes for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc < 2)
    {
        printf("Digite %s palavras[]\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

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
    
    palavrasPorProcesso = (argc-1) / world_size;
    int numeros[TAMANHO_MAXIMO];

    for (int i = 0; i < argc - 1; i++)
    {
        numeros[i] = i + 1;
    }

    MPI_Scatter(numeros, palavrasPorProcesso, MPI_INT, numerosRecebidos, palavrasPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    printf("[PROCESSO: %i] - inicio do array: %d | final do array: %d \n", world_rank, numerosRecebidos[0], numerosRecebidos[palavrasPorProcesso - 1]);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        for (int i = numerosRecebidos[0]; i <= numerosRecebidos[palavrasPorProcesso - 1]; i++)
        {
            numOcorrencias(line, argv[i], i);
        }
    }

    for (int i = numerosRecebidos[0]; i <= numerosRecebidos[palavrasPorProcesso - 1]; i++)
    {
        printf("[PROCESSO: %i] Palavra: %s, foi encontrada: %i vezes na posicao %i\n", world_rank, argv[i], ocorrencias_palavra_chave[i], i);
    }
    fclose(fp);
    
    MPI_Finalize();
}