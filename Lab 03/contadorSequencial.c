// gcc-12 contadorSequencial.c -o contadorSequencial
// ./contadorSequencial All And Boy Book Call Car Chair Children City Dog  Door Enemy  End Enough Eat Friend Father Go Good Girl Food Hear House Inside Laugh Listen Man Name Never Next New Noise Often Pair Pick Play Room See Sell Sit Speak Smile Sister Think Then Walk Water Work Write Woman Yes

#define _GNU_SOURC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

struct timeval t1, t2;
int ocorrencias_palavra_chave[0];

void printarTempoDeExecucao()
{
    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
    printf("\n");
    printf("----------------------------------------------------------\n");
    printf("Tempo total de execução = %f\n", t_total);
    printf("----------------------------------------------------------\n");
}

int numOcorrencias(char *linha, char *palavra, int contador)
{

    char *p = strstr(linha, palavra);
    if ((p == linha) || (p != NULL && !isalnum((unsigned char)p[-1])))
    {
        p += strlen(palavra);
        if (!isalnum((unsigned char)*p))
        {
            ocorrencias_palavra_chave[contador]++;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    gettimeofday(&t1, NULL);
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    *ocorrencias_palavra_chave = malloc(argc * sizeof(int));

    fp = fopen("lotr.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    if (argc < 3)
    {
        printf("Digite %s palavras[]\n", argv[0]);
        exit(0);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        for (int i = 1; i < argc; i++)
        {
            numOcorrencias(line, argv[i], i);
        }
    }

    for (int i = 1; i < argc; i++)
    {
        printf("Palavra: %s, foi encontrada: %i vezes\n", argv[i], ocorrencias_palavra_chave[i]);
    }

    gettimeofday(&t2, NULL);
    printarTempoDeExecucao();

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}