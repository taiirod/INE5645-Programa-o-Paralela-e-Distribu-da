#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>

#define CAPACITY 3
#define LOOP 3

// Create bathroom struct
typedef struct restroom
{
    int men_count;   // count of men
    int women_count; // count od women
    sem_t men;       // men mutex
    sem_t women;     // women mutex
    sem_t n_men;     // men multiplex var (n.men allowed)
    sem_t n_women;   // women multiplex var(n.women allowed)
    sem_t avialable; // if bathroom is avilable or not
    sem_t turnstile; // to prevent starvation
} bathroom_t;

// Initialize the bathroom struct
static bathroom_t unisex_bathroom = {
    .men_count = 0,
    .women_count = 0,
    .men = 1,
    .women = 1,
    .n_men = CAPACITY,
    .n_women = CAPACITY,
    .avialable = 1,
    .turnstile = 1};

void homen_quer_entrar(bathroom_t *unisex_bathroom)
{
    printf("Homem quer entrar\n");
    fflush(stdout);
}

void mulher_quer_entrar(bathroom_t *unisex_bathroom)
{
    printf("Mulher quer entrar\n");
    fflush(stdout);
}

void homen_saiu(bathroom_t *unisex_bathroom)
{
}

void mulher_saiu(bathroom_t *unisex_bathroom)
{
}

static void *
homem(void *restroom)
{
    bathroom_t *unisex_bathroom = (bathroom_t *)restroom;
    homen_quer_entrar(unisex_bathroom);
}

static void *
mulher(void *restroom)
{
    bathroom_t *unisex_bathroom = (bathroom_t *)restroom;
    mulher_quer_entrar(unisex_bathroom);
}

int main(int argc, char *argv[])
{

    pthread_t threads[CAPACITY];

    for (int i = 0; i < LOOP; i++)
    {
        if (rand() % 2 == 0)
        {
            pthread_create(&threads[i], NULL, homem, &unisex_bathroom);
        }
        else
        {
            pthread_create(&threads[i], NULL, mulher, &unisex_bathroom);
        }
    }
    // randomize(threads, total);
    for (int i = 0; i < CAPACITY; i++)
    {
        if (threads[i])
        {
            pthread_join(threads[i], NULL);
        }
    }
    return EXIT_SUCCESS;
}