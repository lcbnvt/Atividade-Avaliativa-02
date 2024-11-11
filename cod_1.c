#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_leitores = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_escritores = PTHREAD_MUTEX_INITIALIZER;
int leitores = 0;
int recurso_compartilhado = 0;

void *leitor(void *arg) {
    int id = *(int *)arg;
    while (1) {
        // Entrada da região crítica para leitores
        pthread_mutex_lock(&mutex_leitores);
        leitores++;
        if (leitores == 1) {
            pthread_mutex_lock(&mutex_escritores);
        }
        pthread_mutex_unlock(&mutex_leitores);

        // Seção crítica
        printf("Leitor %d lendo o valor: %d\n", id, recurso_compartilhado);
        sleep(1); // Simula tempo de leitura

        // Saída da região crítica para leitores
        pthread_mutex_lock(&mutex_leitores);
        leitores--;
        if (leitores == 0) {
            pthread_mutex_unlock(&mutex_escritores);
        }
        pthread_mutex_unlock(&mutex_leitores);

        sleep(rand() % 3); // Tempo aleatório antes da próxima leitura
    }
    pthread_exit(NULL);
}

void *escritor(void *arg) {
    int id = *(int *)arg;
    while (1) {
        // Entrada na região crítica para escritores
        pthread_mutex_lock(&mutex_escritores);

        // Seção crítica
        recurso_compartilhado = rand() % 100;
        printf("Escritor %d atualizou o valor para: %d\n", id, recurso_compartilhado);
        sleep(1); // Simula tempo de escrita

        // Saída da região crítica para escritores
        pthread_mutex_unlock(&mutex_escritores);

        sleep(rand() % 5); // Tempo aleatório antes da próxima escrita
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t_leitores[5], t_escritores[2];
    int id_leitores[5], id_escritores[2];

    // Criação das threads leitoras
    for (int i = 0; i < 5; i++) {
        id_leitores[i] = i + 1;
        pthread_create(&t_leitores[i], NULL, leitor, &id_leitores[i]);
    }

    // Criação das threads escritoras
    for (int i = 0; i < 2; i++) {
        id_escritores[i] = i + 1;
        pthread_create(&t_escritores[i], NULL, escritor, &id_escritores[i]);
    }

    // Aguardando o término das threads (nunca ocorre neste exemplo)
    for (int i = 0; i < 5; i++) {
        pthread_join(t_leitores[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(t_escritores[i], NULL);
    }

    return 0;
}

