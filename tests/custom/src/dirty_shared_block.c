#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CACHELINE_BYTES 64
#define REGION_BYTES 4096

static pthread_barrier_t barrier;

static volatile uint64_t *shared_line;

void *thread0(void *arg)
{
    *shared_line = 1;  // establish M state

    for (volatile int i = 0; i < 100000; i++) {} // delay

    *shared_line = 2;  // keep ownership alive
    return NULL;
}

void *thread1(void *arg)
{
    for (volatile int i = 0; i < 10000; i++) {} // slight offset

    uint64_t val = *shared_line; // should trigger intervention
    printf("read: %lu\n", val);

    return NULL;
}

int main(void)
{
    void *ptr = NULL;

    if (posix_memalign(&ptr, REGION_BYTES, REGION_BYTES) != 0) {
        perror("posix_memalign");
        return 1;
    }

    shared_line = (volatile uint64_t *)ptr;
    *shared_line = 0;

    printf("shared_line=%p\n", (void*)shared_line);
    fflush(stdout);

    pthread_barrier_init(&barrier, NULL, 2);

    pthread_t t0, t1;
    pthread_create(&t0, NULL, thread0, NULL);
    pthread_create(&t1, NULL, thread1, NULL);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    pthread_barrier_destroy(&barrier);
    free(ptr);

    return 0;
}
