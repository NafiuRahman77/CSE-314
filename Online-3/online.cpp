#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <ctime>

typedef long long ll;

std::vector<sem_t> prod;
std::vector<int> buffer;
sem_t mutex;
sem_t consumer_sem;

int count = 0;
int iter = 3;
int n, m;

void initializeResources() {
    prod.resize(m);
    buffer.resize(n);
    for (int i = 0; i < m; i++) {
        sem_init(&prod[i], 0, 2);
    }
    for (int i = 0; i < n; i++)
        buffer[i] = 0;

    sem_init(&mutex, 0, 1);
    sem_init(&consumer_sem, 0, 0);
}

void* producer_thread(void* arg) {
    ll index = (ll)arg;
    printf("Producer %lld is active.\n", index);

    while (iter) {
        sem_wait(&prod[index]);
        sem_wait(&mutex);
        buffer[count++] = (index + 1) * 10 + rand() % 10;
        if (count == n)
            sem_post(&consumer_sem);
        sem_post(&mutex);
    }
    pthread_exit(NULL);
}

void* consumer_thread(void* arg) {
    printf("Consumer is active.\n");
    while (iter) {
        sem_wait(&consumer_sem);
        sem_wait(&mutex);
        printf("Buffer: ");
        for (int i = 0; i < n; i++)
            printf("%d ", buffer[i]);
        printf("\n");
        count = 0;
        iter--;
        sem_post(&mutex);

        for (int i = 0; i < m; i++) {
            sem_post(&prod[i]);
            sem_post(&prod[i]);
        }
    }
    pthread_exit(NULL);
}

int main() {
    std::cin >> n;
    std::cin >> iter;
    m = n / 2;
    
    initializeResources();

    std::vector<pthread_t> producers(m);
    pthread_t consumer;

    pthread_create(&consumer, NULL, consumer_thread, NULL);
    for (ll i = 0; i < m; i++)
        pthread_create(&producers[i], NULL, producer_thread, (void*)i);

    pthread_join(consumer, NULL);
    for (ll i = 0; i < m; i++)
        pthread_join(producers[i], NULL);

    return 0;
}
