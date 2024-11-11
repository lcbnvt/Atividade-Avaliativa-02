#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_FASES 3

pthread_barrier_t barreira;

void *tarefa(void *arg) {
    int id = *(int *)arg;
    for (int fase = 1; fase <= NUM_FASES; fase++) {
        // Simula trabalho da fase
        printf("Thread %d executando fase %d\n", id, fase);
        sleep(rand() % 3 + 1); // Tempo entre 1 e 3 segundos
        printf("Thread %d completou fase %d\n", id, fase);

        // Sincronização na barreira
        int res = pthread_barrier_wait(&barreira);
        if (res != 0 && res != PTHREAD_BARRIER_SERIAL_THREAD) {
            fprintf(stderr, "Erro na barreira\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    // Inicialização da barreira
    if (pthread_barrier_init(&barreira, NULL, NUM_THREADS) != 0) {
        fprintf(stderr, "Erro ao inicializar a barreira\n");
        return EXIT_FAILURE;
    }

    // Criação das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, tarefa, &ids[i]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d\n", ids[i]);
            return EXIT_FAILURE;
        }
    }

    // Aguardando o término das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destruição da barreira
    pthread_barrier_destroy(&barreira);

    return 0;
}
