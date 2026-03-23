#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

void* worker(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d started (pid: %d, tid: %d)\n", id, getpid(), syscall(SYS_gettid));

    while (1) {
        for (volatile int i = 0; i < 100000000; i++);
            sleep(1);
    }

    return NULL;
}

int main() {
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Main Thread (pid: %d, tid: %d)\n", getpid(), syscall(SYS_gettid));
    printf("Detected CPU cores: %ld\n", num_cores);

    pthread_t* threads = malloc(sizeof(pthread_t) * num_cores);
    int* ids = malloc(sizeof(int) * num_cores);

    for (long i = 0; i < num_cores; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, worker, &ids[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    while (1) {
        sleep(1);
    }

    free(threads);
    free(ids);
    return 0;
}