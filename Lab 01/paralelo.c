// Compilar:
// clear && gcc -pthread paralelo.c -o paralelo
// &&
// ./paralelo 3 3

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 2
#define IMPRIME

// Criação de struct para poder passar mais de um parametro para a função bubble
typedef struct dados_t
{
    int *elementos;
    int tam;
} dados_bubble;

void imprime(int **array, int num_arrays, int size)
{
    int i, j;
    for (i = 0; i < num_arrays; i++)
    {
        for (j = 0; j < size; j++)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

void bubble(void *arg)
{
    // *arg é parametro que contém o struct criado anteriormente
    dados_bubble *db;
    // é feito o cast de arg para dados_bubble para pegar as informações de cada campo do struct
    db = (dados_bubble *)arg;
    int i, j;
    int tam = db->tam;
    int temp;

    for (i = 0; i < tam; i++)
    {
        for (j = 0; j < tam - i - 1; j++)
        {
            if (db->elementos[j] > db->elementos[j + 1])
            {
                temp = db->elementos[j];
                db->elementos[j] = db->elementos[j + 1];
                db->elementos[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char **argv)
{

    int **elementos, num_array, tam_array, i, j, num_threads;
    struct timeval t1, t2;
    double t_total;

    if (argc != 3)
    {
        printf("Digite %s Num_arrays Num_elementos\n", argv[0]);
        exit(0);
    }
    num_array = atoi(argv[1]);
    tam_array = atoi(argv[2]);
    // srand(time(NULL));

    pthread_t t[NUM_THREADS];

    /*Aloca memória para os vetores*/
    elementos = (int **)malloc(num_array * sizeof(int *));
    for (i = 0; i < num_array; i++)
        elementos[i] = (int *)malloc(tam_array * sizeof(int));

    /*Popula os arrays com elementos aleatorios entre 0 e 1000*/
    for (i = 0; i < num_array; i++)
        for (j = 0; j < tam_array; j++)
            elementos[i][j] = rand() % 1000;

#ifdef IMPRIME
    printf("Antes da ordenacao!\n");
    imprime(elementos, num_array, tam_array);
    printf("\n");
#endif

    gettimeofday(&t1, NULL);
    for (i = 0; i < NUM_THREADS; i++)
    {
        for (i = 0; i < num_array/2; i++)
        {
            dados_bubble *dados;
            dados = malloc(sizeof(dados_bubble));
            // atribuição de informações para os atributos do struct
            dados->tam = tam_array;
            dados->elementos = elementos[i];
            pthread_create(&t[0], NULL, bubble, (void *)dados);
            /* code */
        }

        // instanciação e alocação de memoria do struct dados_bubble
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(t[i], NULL);
    }
    gettimeofday(&t2, NULL);

    t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

#ifdef IMPRIME
    printf("apos a ordenacao!\n");
    imprime(elementos, num_array, tam_array);
    printf("\n");
#endif

    /*Libera memoria alocada*/
    for (i = 0; i < num_array; i++)
    {
        free(elementos[i]);
    }
    free(elementos);

    printf("tempo total = %f\n", t_total);

    return 0;
}
