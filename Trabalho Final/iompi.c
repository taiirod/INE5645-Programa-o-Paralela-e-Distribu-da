// mpicc pontoAPonto.c -o pontoAPonto
// Original 25 palavras
//      mpiexec -np 3 ./pontoAPonto All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh
// Dobro 50 palavras
//      mpiexec -np 3 ./pontoAPonto All And Boy Book Call Car Chair Children City Dog Door Enemy End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh Listen Man Name Never Next New Noise Often Pair Pick Play Room See Sell Sit Speak Smile Sister Think Then Walk Water Work Write Woman Yes

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <sys/time.h>
#include <ctype.h>

#define FILENAME "arquivo.txt"

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
    return sum / (num_elements - 1);
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

    int i;
    int world_size, world_rank;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int bufsize, nrchar;
    char *buf; /* Buffer for reading */
    MPI_Offset filesize;
    MPI_File myfile;   /* Shared file */
    MPI_Status status; /* Status returned from read */

    gettimeofday(&t1, NULL);
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size < 2)
    {
        fprintf(stderr, "Must at least two processes for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

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

    /* Open the file */
    MPI_File_open(MPI_COMM_WORLD, FILENAME, MPI_MODE_RDONLY, MPI_INFO_NULL, &myfile);
    /* Get the size of the file */
    MPI_File_get_size(myfile, &filesize);
    /* Calculate how many elements that is */
    filesize = filesize / sizeof(char);
    /* Calculate how many elements each processor gets */
    bufsize = filesize / world_size;
    /* Allocate the buffer to read to, one extra for terminating null char */
    buf = (char *)malloc((bufsize + 1) * sizeof(char));
    /* Set the file view */
    MPI_File_set_view(myfile, world_rank * bufsize * sizeof(char), MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    /* Read from the file */
    MPI_File_read(myfile, buf, bufsize, MPI_CHAR, &status);
    /* Find out how many elemyidnts were read */
    MPI_Get_count(&status, MPI_CHAR, &nrchar);
    /* Set terminating null char in the string */
    buf[nrchar] = (char)0;
    printf("Process %2d read %d characters: \n", world_rank, nrchar);
    // printf("  %s\n", buf);

    if (world_rank == 0)
    {
        printf("Done\n");
    }

    while (buf)
    {
        for (int i = 0; i < argc + 1; i++)
        {
            numOcorrencias(line, argv[i], i);
        }
    }
    for (int i = 0; i < argc + 1; i++)
    {
        printf("[PROCESSO: %i] Palavra: %s, foi encontrada: %i vezes\n", world_rank, argv[i], ocorrencias_palavra_chave[i]);
    }

    MPI_Finalize();
    return 0;
}