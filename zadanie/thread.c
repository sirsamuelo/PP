#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

typedef struct {
    int index;
} ThreadData;

void* TRD1(void* arg) {
    pthread_mutex_lock(&mutex);
    counter++;
    printf("Thread ID=%lu incremented counter to %d\n", pthread_self(), counter);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* TRD0(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    printf("Main thread created thread with index=%d\n", data->index);
    
    pthread_t childThreads[2];
    for (int i = 0; i < 2; i++) {
        pthread_create(&childThreads[i], NULL, TRD1, NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(childThreads[i], NULL);
    }

    return NULL;
}

int main() {
    pthread_t threads[4];
    ThreadData data[4];

    for (int i = 0; i < 4; i++) {
        data[i].index = i;
        pthread_create(&threads[i], NULL, TRD0, &data[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL); // zabezpecuje aby program neskoncil prilis skoro, sluzi 
        //ako synchronizacny nastroj. Keby tu toto nebolo tak hlavne vlakno main skonci skor kym sa 
        //ostatne stihnu dokoncit
    }

    return 0;
}
