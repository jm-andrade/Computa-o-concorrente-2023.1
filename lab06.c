/*
Lab 06
Aluno: João Marcelo de Andrade
DRE: 119135067
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//Variaveis globais

#define N 5 /* Tamanho do buffer*/

int N_prod; //Produtores
int N_cons; //Consumidores
int buffer[N]; //Buffer
int buffer_liberado = 1; //Variavel de controle de lotação do buffer 0->não está lotado | 1->Esté lotado
sem_t slots_vazios;
sem_t slots_ocupados;   // semaforos variaveis de controle de consumidores e produtores
sem_t mutex; //semaforo de exclusão mutua               */

int produzir_valor()
{
    int valor = 0;
    for (int i = 0; i < 100; i++){ 
        valor += rand()%10; 
    }
    return valor;
}

void consumir_valor(int valor)
{
    for (int i = 0, count = 0; i < 10000; i++){ 
        if(valor%i == 0){ 
            count++;
        } 
    }
}

void insere(int valor, int id_t)
{
    static int in = 0;  
    printf("Produtor [%d] quer inserir.\n", id_t);
    sem_wait(&slots_vazios);
    sem_wait(&mutex);
    if(buffer_liberado == 1){
        printf("Produtor [%d] está inserindo.\n", id_t);
        buffer[in] = valor;
        in++;
        if(in == N){
            in = 0;
            buffer_liberado = 0;
            sem_post(&slots_ocupados);
        }    
    }
    sem_post(&slots_ocupados);
    sem_post(&mutex);
}

int retira(int id_t)
{
    static int out = 0;
    printf("Consumidor [%d] quer retirar.\n", id_t);
    sem_wait(&slots_ocupados);
    sem_wait(&mutex);
    printf("Consumidor [%d] está retirando.\n", id_t);
    int valor = buffer[out];        
    if(out == N){
        out = 0;
        buffer_liberado = 1;
        for (int i = 0; i < N; i++){ 
            sem_post(&slots_vazios); 
        } 
    }

    if(buffer_liberado == 0){ 
        sem_post(&slots_ocupados); 
    }

    sem_post(&mutex);
    return valor;
}

void* produzir(void* arg)
{
    int* prod_id = (int*) arg;
    printf("Rodando thread produtora [%d]!\n", *prod_id);

    while(1){
        insere(produzir_valor(), *prod_id);
    }
    pthread_exit(NULL);
} 

void* consumir(void* arg)
{
    int* cons_id = (int*) arg;
    int valor;
    printf("Rodando thread consumidora [%d]!\n", *cons_id);
    while (1){
        valor = retira(*cons_id);
        consumir_valor(retira(*cons_id));
    }
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    if(argc < 3){
        printf("Erro: A chamada do programa deve ser da forma ./lab06 <Número de produtores> <Número de consumidores> \n");
        exit(-1);
    }
    N_prod = atoi(argv[1]);
    N_cons = atoi(argv[2]);

    // inicializar semaforos
    sem_init(&mutex, 0, 1);   
    sem_init(&slots_vazios, 0, N);
    sem_init(&slots_ocupados, 0, 0);

    //Criação das threads produtoras e consumidoras
    pthread_t threads_produtoras[N_prod];
    pthread_t threads_consumidoras[N_cons];

    for(int i = 0; i < N_prod; i++){
        printf("Criando produtora [%d]\n",i);
        if(pthread_create(&threads_produtoras[i], NULL, produzir, (void*)&i)){
            printf("ERRO na criação das threads produtoras.\n");
            exit(-1);
        }
    }

    for(int i = 0; i < N_cons; i++){
        printf("Criando consumidora [%d]\n",i);
        if(pthread_create(&threads_consumidoras[i], NULL, consumir, (void*)&i)){
            printf("ERRO na criação das threads consumidoras.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < N_prod; i++) { 
        pthread_join(threads_produtoras[i], NULL); 
    }
    for (int i = 0; i < N_cons; i++) { 
        pthread_join(threads_consumidoras[i], NULL);
    }

    return 0;
}