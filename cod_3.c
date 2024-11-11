#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TAM_BUFFER 5

int buffer[TAM_BUFFER];
int count = 0, in = 0, out = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;

void *produtor(void *arg) {
    while (1) {
        int item = rand() % 100;

        pthread_mutex_lock(&mutex);
        while (count == TAM_BUFFER) {
            // Buffer cheio, produtor espera
            pthread_cond_wait(&cond_prod, &mutex);
        }
        // Adiciona item ao buffer
        buffer[in] = item;
        in = (in + 1) % TAM_BUFFER;
        count++;
        printf("Produtor produziu: %d (count = %d)\n", item, count);
        pthread_cond_signal(&cond_cons); // Sinaliza consumidor
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3); // Tempo aleatório antes de produzir o próximo item
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            // Buffer vazio, consumidor espera
            pthread_cond_wait(&cond_cons, &mutex);
        }
        // Remove item do buffer
        int item = buffer[out];
        out = (out + 1) % TAM_BUFFER;
        count--;
        printf("Consumidor consumiu: %d (count = %d)\n", item, count);
        pthread_cond_signal(&cond_prod); // Sinaliza produtor
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 5); // Tempo aleatório antes de consumir o próximo item
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t_produtor, t_consumidor;

    // Criação das threads
    pthread_create(&t_produtor, NULL, produtor, NULL);
    pthread_create(&t_consumidor, NULL, consumidor, NULL);

    // Aguardando o término das threads (nunca ocorre neste exemplo)
    pthread_join(t_produtor, NULL);
    pthread_join(t_consumidor, NULL);

    return 0;
}
