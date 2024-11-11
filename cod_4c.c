#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t recurso_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t recurso_b = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg) {
    pthread_mutex_lock(&recurso_a);
    printf("Thread 1 adquiriu Recurso A\n");
    sleep(1); // Simula trabalho

    pthread_mutex_lock(&recurso_b);
    printf("Thread 1 adquiriu Recurso B\n");

    // Seção crítica
    printf("Thread 1 executando na seção crítica\n");

    pthread_mutex_unlock(&recurso_b);
    pthread_mutex_unlock(&recurso_a);

    pthread_exit(NULL);
}

void *thread2(void *arg) {
    pthread_mutex_lock(&recurso_b);
    printf("Thread 2 adquiriu Recurso B\n");
    sleep(1); // Simula trabalho

    pthread_mutex_lock(&recurso_a);
    printf("Thread 2 adquiriu Recurso A\n");

    // Seção crítica
    printf("Thread 2 executando na seção crítica\n");

    pthread_mutex_unlock(&recurso_a);
    pthread_mutex_unlock(&recurso_b);

    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
