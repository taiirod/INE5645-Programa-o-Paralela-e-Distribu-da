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
#define LOOP 2

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

void man_wants_to_enter(bathroom_t *unisex_bathroom)
{
    sem_wait(&unisex_bathroom->turnstile);
    sem_wait(&unisex_bathroom->men);
    unisex_bathroom->men_count++;
    if (unisex_bathroom->men_count == 1)
        sem_wait(&unisex_bathroom->avialable);
    sem_post(&unisex_bathroom->men);
    sem_post(&unisex_bathroom->turnstile);
    sem_wait(&unisex_bathroom->n_men);
}

void woman_wants_to_enter(bathroom_t *unisex_bathroom)
{
    sem_wait(&unisex_bathroom->turnstile);
    sem_wait(&unisex_bathroom->women);
    unisex_bathroom->women_count++;
    if (unisex_bathroom->women_count == 1)
        sem_wait(&unisex_bathroom->avialable);
    sem_post(&unisex_bathroom->women);
    sem_post(&unisex_bathroom->turnstile);
    sem_wait(&unisex_bathroom->n_women);
}

void man_leaves(bathroom_t *unisex_bathroom)
{
    printf("Homen saiu\n");
    fflush(stdout);
    sem_post(&unisex_bathroom->n_men);
    sem_wait(&unisex_bathroom->men);
    unisex_bathroom->men_count--;
    if (unisex_bathroom->men_count == 0)
    {
        sem_post(&unisex_bathroom->avialable);
        printf("Vazio\n");
    }
    sem_post(&unisex_bathroom->men);
}

void woman_leaves(bathroom_t *unisex_bathroom)
{
    printf("Mulher saiu\n");
    fflush(stdout);
    sem_post(&unisex_bathroom->n_women);
    sem_wait(&unisex_bathroom->women);
    unisex_bathroom->women_count--;
    if (unisex_bathroom->women_count == 0)
    {
        sem_post(&unisex_bathroom->avialable);
        printf("Vazio\n");
    }
    sem_post(&unisex_bathroom->women);
}

// men thread function
static void *
men(void *restroom)
{
    bathroom_t *unisex_bathroom = (bathroom_t *)restroom;
    for (int i = 0; i < LOOP; i++)
    {
        man_wants_to_enter(unisex_bathroom);
        printf("Homem presente\n");
        fflush(stdout);
        sleep(rand() % 5);
        man_leaves(unisex_bathroom);
    }
    pthread_exit(0);
}

// women thread function
static void *
women(void *restroom)
{
    bathroom_t *unisex_bathroom = (bathroom_t *)restroom;
    for (int i = 0; i < LOOP; i++)
    {
        woman_wants_to_enter(unisex_bathroom);
        printf("Mulher presente\n");
        fflush(stdout);
        sleep(rand() % 5);
        woman_leaves(unisex_bathroom);
    }
    pthread_exit(0);
}

void usando_o_banheiro (){

}

int main(void)
{

    int MaleNum;
    int FemaleNum;

    printf("Enter the Number of Males: ");
    scanf("%d", &MaleNum);
    printf("Enter the Number of Females: ");
    scanf("%d", &FemaleNum);

    int Total = MaleNum + FemaleNum;

    pthread_t gender[Total];
    for (int i = 0; i < MaleNum; i++) // creating a multithreaded applications
    {
        pthread_create(&gender[i], NULL, men, &unisex_bathroom); // Depending on the number of males inputted from the user, pthreades will be created
    }
    for (int i = MaleNum; i < Total; i++)
    {
        pthread_create(&gender[i], NULL, women, &unisex_bathroom); // Depending on the number of females inputted from the user, pthreades will be created
    }

    for (int i = 0; i < Total; i++)
    {
        pthread_join(gender[i], NULL); // to make sure all threads complete before main finishes
    }
    return 0;
}