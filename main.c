#include <stdlib.h>

#include <stdio.h>

#define N 1000000

#define J 500000

int main() {

    printf("N: %d\n", N);

    SQ *q = sq_create();

    for (int i = 0; i < N; i++) {

        sq_give_buzzer(q);

    }

    for (int i = J; i < N; i++) {

        sq_take_bribe(q, i);

    }

    for (int i = 0; i < J; i++) {

        sq_kick_out(q, i);

    }

    for (int i = 0; i < N; i++) {

        sq_seat(q);

    }
    printf("Done");
    sq_free(q);

    return 0;

} 
