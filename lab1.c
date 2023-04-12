#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Declarações globais
int const n_threads = 2;
int const tam_vet = 10000;

int* vet;

// Função thread
void* multiplica_por_dois(void* args) 
{
    int thread_id = (int) args; 
    for(int i=thread_id; i<tam_vet; i+=n_threads)
    {
        *(vet + i) *= 2; 
    }

    pthread_exit(NULL);
}

// Função principal
int main() 
{
    pthread_t threads[n_threads];   // Vetor com o identificador das threads

    // Alocando memória para o vetor
    vet = (int*) malloc(sizeof(int)*tam_vet);

    // Preenchendo o vetor
    for(int i=0; i<tam_vet; i++)
    {
        vet[i] = i;
    }

    // Criando as threads
    for(int i=0; i<n_threads; i++) 
    {
        if(pthread_create(&threads[i], NULL, multiplica_por_dois, (void*) i))
        {
            printf("ERRO! Falha no pthread_create()\n");
            return -1;
        }
    }

    // Juntando os fluxos de execução
    for (int i=0; i<n_threads; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    // Verificando corretude da solução
    for(int i=0; i<tam_vet; i++)
    {
        if(vet[i] != 2*i)
        {
            printf("ERRO! Falha na verificação.\n");
            return -1;
        }
    }
    printf("FEITO! A solução está correta!\n");

    return 0;
}