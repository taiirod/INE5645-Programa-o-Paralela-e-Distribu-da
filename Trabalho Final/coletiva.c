// mpicc coletiva.c -o coletiva
//  25 palavras
//      mpiexec -np 1 ./coletiva All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh
//  50 palavras
//      mpiexec -np 1 ./coletiva All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh Listen Man Name Never Next New Noise Often Pair Pick Play Room See Sell Sit Speak Smile Sister Think Then Walk Water Work Write Woman Yes 
//  100 palavras
//      mpiexec -np 1 ./coletiva All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh Listen Man Name Never Next New Noise Often Pair Pick Play Room See Sell Sit Speak Smile Sister Think Then Walk Water Work Write Woman Yes us most day give these any because want new even way well first work our how two use after back also think over its come only look now then than other see them could some good your year into people take know him just no time like can make when

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <ctype.h>
#include <sys/time.h>

#define TAMANHO_MAXIMO 100

int palavrasPorProcesso;
int numerosRecebidos[TAMANHO_MAXIMO];
int ocorrencias_palavra_chave[0];
const int MASTER = 0;

double calculaExecTotal(double *array, int num_elements)
{
    double sum = 0;
    int i;
    for (i = 0; i < num_elements; i++)
    {
        sum += array[i];
    }
    return sum / num_elements;
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
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
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

    palavrasPorProcesso = (argc - 1) / world_size;
    int numeros[TAMANHO_MAXIMO];
    for (int i = 0; i < argc - 1; i++)
    {
        numeros[i] = i + 1;
    }

    MPI_Scatter(numeros, palavrasPorProcesso, MPI_INT, numerosRecebidos, palavrasPorProcesso, MPI_INT, 0, MPI_COMM_WORLD);

    int numeroLimite = numerosRecebidos[palavrasPorProcesso - 1];

    while ((read = getline(&line, &len, fp)) != -1)
    {
        for (int i = numerosRecebidos[0]; i <= numeroLimite; i++)
        {
            numOcorrencias(line, argv[i], i);
        }
    }

    for (int i = numerosRecebidos[0]; i <= numeroLimite; i++)
    {
        printf("[PROCESSO: %d] Palavra: %s, foi encontrada: %d vezes\n", world_rank, argv[i], ocorrencias_palavra_chave[i]);
    }
    gettimeofday(&t2, NULL);
    fclose(fp);

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

    double *totais = NULL;
    if (world_rank == 0)
    {
        totais = (double *)malloc(sizeof(double) * world_size);
        assert(totais != NULL);
    }

    MPI_Gather(&t_total, 1, MPI_DOUBLE, totais, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        double execTotal = calculaExecTotal(totais, world_size);

        printf("\n");
        printf("----------------------------------------------------------\n");
        printf("[PROCESSO: %d] Tempo total de execução [COLETIVA] = %f\n", world_rank, execTotal);
        printf("----------------------------------------------------------\n");
    }

    

    MPI_Finalize();
    return 0;
}