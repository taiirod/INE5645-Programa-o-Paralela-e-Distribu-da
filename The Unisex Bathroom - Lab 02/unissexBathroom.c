// compilar com gcc -pthread
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

#define CAPACITY 3

int men = 0;
int women = 0;
int totalmen = 0;
int totalwomen = 0;

pthread_mutex_t ManMx;
pthread_cond_t ManCon;
pthread_mutex_t WomanMx;
pthread_cond_t WomanCon;

void man_needs_bathroom()
{
}

void woman_needs_bathroom()
{
}

void man_leaves_bathroom()
{
}

void woman_leaves_bathroom()
{
}

void *male_threads(void *arg)
{
}

void *female_threads(void *arg)
{
}

int main(int argc, char *argv[])
{
}