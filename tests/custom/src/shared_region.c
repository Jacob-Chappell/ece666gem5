#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#define NTHREADS 4
#define ITERS 200000

volatile uint64_t shared_arr[512];

void* work(void* arg) {
    long tid = (long)arg;

    for (int i = 0; i < ITERS; i++) {
        shared_arr[(i + tid) % 512]++;
    }

    return 0;
}

int main() {
    pthread_t t[NTHREADS];

    for (long i = 0; i < NTHREADS; i++)
        pthread_create(&t[i], 0, work, (void*)i);

    for (int i = 0; i < NTHREADS; i++)
        pthread_join(t[i], 0);

    printf("done %lu\n", shared_arr[0]);
    return 0;
}
