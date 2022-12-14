// mpicc pontoAPonto.c -o pontoAPonto
//  25 palavras
//      mpiexec -np 2 ./pontoAPonto All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh
//  50 palavras
//      mpiexec -np 2 ./pontoAPonto All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh Listen Man Name Never Next New Noise Often Pair Pick Play Room See Sell Sit Speak Smile Sister Think Then Walk Water Work Write Woman Yes
//  100 palavras
//      mpiexec -np 2 ./pontoAPonto All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh Listen Man Name Never Next New Noise Often Pair Pick Play Room See Sell Sit Speak Smile Sister Think Then Walk Water Work Write Woman Yes us most day give these any because want new even way well first work our how two use after back also think over its come only look now then than other see them could some good your year into people take know him just no time like can make when

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <sys/time.h>
#include <ctype.h>

int palavrasPorProcesso;
int numeroRecebido;
int numeroLimite;
int ocorrencias_palavra_chave[0];
int processo[10];

double calculaExecTotal(double *array, int num_elements)
{
    double sum = 0;
    int i;
    for (i = 0; i < num_elements; i++)
    {
        sum += array[i];
    }
    return sum / (num_elements-1);
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

int main(int argc, char *argv[])
{
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    int i;
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

    /* World_rank (Processo 0) responsavel por configurar execução
        Calcula quantas palavras cada processo iria procurar
        Define o numero de inicio e final de execução de cada processo
        Ex: Se tem 6 palavras e 3 processos, apenas o 1 e o 2 executarão a busca.
        Sendo assim, teremos 3 palavras em cada processo.
        Então:  
            Proc1 recebe numeroParaMandar = 1 e numeroLimite = 3
            Proc2 recebe numeroParaMandar = 4 e numeroLimite = 6
        Assim então, cada processo sabe a sua posição inicial e final de iteração para busca de palavras.
    */
    if (world_rank == 0)
    {
        palavrasPorProcesso = (argc - 1) / (world_size - 1);
        int numeroParaMandar = 1;
        for (int i = 1; i < world_size; i++)
        {
            MPI_Send(&numeroParaMandar, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            numeroParaMandar = numeroParaMandar + palavrasPorProcesso;
            numeroLimite = numeroParaMandar - 1;
            // nesse caso, foi utilizada tag 1 para garantir que mensagem seria recebida corretamente
            MPI_Send(&numeroLimite, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    }
    else
    {
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
        MPI_Recv(&numeroRecebido, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&numeroLimite, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

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
        gettimeofday(&t2, NULL);

        fclose(fp);
    }
    double t_total;

    if (world_rank != 0)
    {
        t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
    }

    double *totais = NULL;
    totais = (double *)malloc(sizeof(double) * world_size);
    assert(totais != NULL);

    MPI_Gather(&t_total, 1, MPI_DOUBLE, totais, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        double execTotal = calculaExecTotal(totais, world_size);

        printf("\n");
        printf("----------------------------------------------------------\n");
        printf("[PROCESSO: %d] Tempo total de execução [PONTO A PONTO] = %f\n", world_rank, execTotal);
        printf("----------------------------------------------------------\n");
    }

    MPI_Finalize();
    return 0;
}