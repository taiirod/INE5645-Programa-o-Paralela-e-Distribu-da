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
#include <string.h>
#include <mpi.h>
#include <sys/time.h>
#include <ctype.h>

#define FILENAME "arquivo.txt"

int palavrasPorProcesso;
char bufRecebido;
int ocorrencias_palavra_chave[0];
int ocorrenciasSomadas[0];

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

    int numeroDePalavras = argc;

    int i;
    int world_size, world_rank;

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int bufsize, nrchar;
    char *buf; /* Buffer for reading */
    MPI_Offset filesize;
    MPI_File myfile;   /* Shared file */
    MPI_Status status; /* Status returned from read */

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (numeroDePalavras < 2)
    {
        printf("Digite %s palavras[]\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // printf("Abrindo arquivo...\n");
    MPI_File_open(MPI_COMM_WORLD, FILENAME, MPI_MODE_RDONLY, MPI_INFO_NULL, &myfile);

    // printf("Calculando tamanho do arquivo...\n");
    MPI_File_get_size(myfile, &filesize);

    // printf("Calculando numero de elementos do arquivo...\n");
    filesize = filesize / sizeof(char);

    // printf("Calculando quantos elementos cada processo recebe...\n");
    bufsize = filesize / world_size;

    // printf("Alocando memoria para buffer...\n");
    buf = (char *)malloc((bufsize + 1) * sizeof(char));

    // printf("Configurando file view...\n");
    MPI_File_set_view(myfile, world_rank * bufsize * sizeof(char), MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);

    // printf("Lendo arquivo...\n");
    MPI_File_read(myfile, buf, bufsize, MPI_CHAR, &status);

    // printf("Conta quantos elementos ja foram lidos...\n");
    MPI_Get_count(&status, MPI_CHAR, &nrchar);

    // printf("Define ultimo char da string como nulo...\n");
    buf[nrchar] = (char)0;

    char filename[100];
    snprintf(filename, 100, "buffer_%d.txt", world_rank);

    fp = fopen(filename, "w+");
    fputs(buf, fp);
    fp = fopen(filename, "r");

    int ocorrenciasSomadas[numeroDePalavras];

    while ((read = getline(&line, &len, fp)) != -1)
    {

        for (int i = 1; i < numeroDePalavras; i++)
        {
            numOcorrencias(line, argv[i], i);
        }
    }

    MPI_Reduce(&ocorrencias_palavra_chave, &ocorrenciasSomadas, numeroDePalavras, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        for (int i = 1; i < numeroDePalavras; i++)
        {
            printf("Palavra: %s, foi encontrada: %d vezes\n", argv[i], ocorrenciasSomadas[i]);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    gettimeofday(&t2, NULL);

    if (world_rank == 0)
    {
        double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

        printf("\n");
        printf("----------------------------------------------------------\n");
        printf("Tempo total de execução [COLETIVA] = %f\n", t_total);
        printf("----------------------------------------------------------\n");
    }

    MPI_Finalize();
    return 0;
}