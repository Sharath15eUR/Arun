// Solution for MODULE 2 – Part 1: Basic Multi-Threaded Task Runner
// Submitted by Arun Manikandan
//
// TODO:
//   - Ask user for N (number of primes to sum).
//   - Spawn three threads:
//       * Thread A: sum first N primes.
//       * Thread B: print “Thread 1 running” every 2 seconds for 100 seconds.
//       * Thread C: print “Thread 2 running” every 3 seconds for 100 seconds.
//   - Wait for all threads to finish, then exit.
//
// Constraints:
//   - No more than 100 seconds of prints for B & C.
//   - Sum of primes must be correct.
//   - Keep code simple (no structs).

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

// Check primality in the straightforward way
int is_prime_simple(int x) {
    if (x < 2) return 0;
    if (x % 2 == 0 && x != 2) return 0;
    for (int i = 3; i <= (int)sqrt(x); i += 2) {
        if (x % i == 0) return 0;
    }
    return 1;
}

// Thread A: compute sum of first N primes
void* thread_sum_primes(void* arg) {
    int N = *(int*)arg;
    int count = 0, num = 2, sum = 0;
    while (count < N) {
        if (is_prime_simple(num)) {
            sum += num;
            count++;
        }
        num++;
    }
    printf("Sum of first %d primes = %d\n", N, sum);
    return NULL;
}

// Thread B: print every 2 seconds (total ~100s)
void* thread_print1(void* arg) {
    for (int t = 0; t < 100; t += 2) {
        printf("Thread 1 running\n");
        sleep(2);
    }
    return NULL;
}

// Thread C: print every 3 seconds (total ~100s)
void* thread_print2(void* arg) {
    for (int t = 0; t < 100; t += 3) {
        printf("Thread 2 running\n");
        sleep(3);
    }
    return NULL;
}

int main() {
    int N;
    printf("Enter N (number of primes to sum): ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "Invalid input. Exiting.\n");
        return 1;
    }

    pthread_t tA, tB, tC;

    // CREATE threads
    pthread_create(&tA, NULL, thread_sum_primes, &N);
    pthread_create(&tB, NULL, thread_print1, NULL);
    pthread_create(&tC, NULL, thread_print2, NULL);

    // WAIT for all threads
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);

    printf("All threads completed. Exiting.\n");
    return 0;
}

