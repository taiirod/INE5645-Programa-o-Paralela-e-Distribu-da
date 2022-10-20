#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>

#define LOOP 10

pthread_mutex_t ManMx;
pthread_cond_t ManCon;
int men = 0;
int totalmen = 0;
int totalwomen = 0;

pthread_mutex_t WomanMx;
pthread_cond_t WomanCon;
int women = 0;

void man_wants_to_enter()
{
    pthread_mutex_lock(&ManMx);
    while (women > 0 || men > 2) // assuming that the maximum capacity of the bathroom is 3.
    {
        pthread_cond_wait(&ManCon, &ManMx); // the male thread has to wait until there is no women in the bathroom and there is less than 3 males
    }
    totalmen++;
    men++;
    women = 0;
    printf("Man Entered!   Number of Men currently in the bathroom:  %d\n", men);
    srand(time(NULL));
    sleep(rand() % 5);
    if (men < 3)
        pthread_cond_signal(&ManCon); // the male thread is unblocked if there happens to be less than 3 males currently in the bathroom
    pthread_mutex_unlock(&ManMx);
    return;
}

void woman_wants_to_enter()
{
    pthread_mutex_lock(&WomanMx);
    while (men > 0 || women > 2)
    {
        pthread_cond_wait(&WomanCon, &WomanMx); // the male thread has to wait until there is no women in the bathroom and there is less than 3 males
    }
    totalwomen++;
    women++;
    men = 0;
    if (women < 3)
        pthread_cond_signal(&WomanCon); // the female thread is unblocked if there happens to be less than 3 females currently in the bathroom
    printf("        Woman Entered! Number of Women currently in the bathroom: %d\n", women);
    srand(time(NULL));
    sleep(rand() % 5);
    pthread_mutex_unlock(&WomanMx);
    return;
}

void man_leaves()
{

    pthread_mutex_lock(&ManMx);
    men--;
    printf("Man Left! Number of Men currently in the bathroom: %d\n", men);
    pthread_mutex_unlock(&ManMx);

    if (men == 0)
    {
        pthread_mutex_lock(&WomanMx);
        pthread_cond_signal(&WomanCon); // if after a man leaves and the total number of males becomes zero, the female threads are unblocked
        pthread_mutex_unlock(&WomanMx);
    }
    return;
}

void woman_leaves()
{

    pthread_mutex_lock(&WomanMx);
    women--;
    printf("       Woman Left!  Number of Women currently in the bathroom: %d\n", women);

    pthread_mutex_unlock(&WomanMx);
    if (women == 0)
    {
        pthread_mutex_lock(&ManMx);
        pthread_cond_signal(&ManCon); // if after a woman leaves and the total number of females becomes zero, the male threads are unblocked
        pthread_mutex_unlock(&ManMx);
    }
    return;
}

void *male(void *arg) // function used with male threads
{

    for (int i = 0; i < LOOP; i++)
    {
        man_wants_to_enter();
        // printf("Men Present!\n");
        man_leaves();
    }
    return NULL;
}

void *female(void *arg) // function used with male threads
{
    for (int i = 0; i < LOOP; i++)
    {
        woman_wants_to_enter();
        //  printf("Women Present!\n");
        woman_leaves();
    }
    return NULL;
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
        pthread_create(&gender[i], NULL, male, NULL); // Depending on the number of males inputted from the user, pthreades will be created
    }
    for (int i = MaleNum; i < Total; i++)
    {
        pthread_create(&gender[i], NULL, female, NULL); // Depending on the number of females inputted from the user, pthreades will be created
    }

    for (int i = 0; i < Total; i++)
    {
        pthread_join(gender[i], NULL); // to make sure all threads complete before main finishes
    }
    printf("-----------Total Women Entries-------- %d\n", totalwomen);
    printf("-----------Total Men Entries---------- %d\n", totalmen);
    return 0;
}