//
// Created by Huy on 5/16/15.
//

#include <stdio.h>
#include <time.h>

void wait(int seconds) {
    double targetTime = clock() + (seconds * CLOCKS_PER_SEC);
    while (clock() < targetTime);
}

int main() {
    int i;
    for (i = 0; i < 3; i++) {
        double start = clock();
        wait(2);
        double end = clock();
        printf("Hello Huy, it took this long:  %lf \n", (end-start)/CLOCKS_PER_SEC);
    }
    return 0;
}