#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;
int aux=0;

void* oiMaria(void *arg) 
{
    printf("Oi Maria!\n");
    pthread_mutex_lock(&mutex);
    aux += 1;
    if(aux == 2) 
    {
        pthread_cond_signal(&cond1);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* oiJose(void *arg) 
{
    printf("Oi José!\n");
    pthread_mutex_lock(&mutex);
    aux += 1;
    if(aux == 2) 
    {
        pthread_cond_signal(&cond1);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* senteSePorFavor(void *arg) 
{
    pthread_mutex_lock(&mutex);
    while(aux != 2)
    {
        pthread_cond_wait(&cond1, &mutex);
    }
    printf("Sente-se por favor.\n");
    aux += 1;
    pthread_cond_broadcast(&cond2);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
    
}

void* ateMaisJose(void *arg) 
{
    pthread_mutex_lock(&mutex);
    while(aux != 3)
    {
        pthread_cond_wait(&cond2, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    printf("Até mais José!\n");
    pthread_exit(NULL);
}

void* ateMaisMaria(void *arg) 
{
    pthread_mutex_lock(&mutex);
    while(aux != 3)
    {
        pthread_cond_wait(&cond2, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    printf("Até mais Maria!\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    pthread_t threads[5];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond1, NULL);
    pthread_cond_init (&cond2, NULL);

    pthread_create(&threads[0], NULL, oiMaria, NULL);
    pthread_create(&threads[1], NULL, oiJose, NULL);
    pthread_create(&threads[2], NULL, senteSePorFavor, NULL);
    pthread_create(&threads[3], NULL, ateMaisJose, NULL);
    pthread_create(&threads[4], NULL, ateMaisMaria, NULL);

    for (int i = 0; i < 5; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);

    return 0;
}
