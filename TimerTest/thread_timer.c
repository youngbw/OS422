//
// Created by Huy on 5/16/15.
//

#include <pthread.h>
#include <stdio.h>

int totalTime = 6;

void * timer(void *secs) {
    int seconds = (int)(long) secs;
    int i;
    for (i = 0; i < totalTime; i+=seconds) {
        double targetTime = clock() + (seconds * CLOCKS_PER_SEC);
        while (clock() < targetTime);
        printf("%d seconds has passed!\n", seconds);
    }
    return NULL;
}

void wait(int seconds) {
    double targetTime = clock() + (seconds * CLOCKS_PER_SEC);
    while (clock() < targetTime);
}

main () {
    pthread_t threads[1];
    pthread_create(&threads[0], NULL, timer, (void *) 2);

    int i;
    for (i = 0; i < 6; i++) {
        printf("I'm doing something..\n");
        wait(1);
    }
    pthread_exit(NULL);
}

