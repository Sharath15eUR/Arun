// Solution for MODULE 2 – Part 2: Signal-Handled & Timed Threads
// Submitted by Arun Manikandan
//
// TODO:
//   - Ask user for N.
//   - Install SIGINT/SIGTERM handler to ignore termination.
//   - Thread A: sum first N primes using 6k ± 1 optimization, measure and print elapsed time.
//   - Thread B & C: same periodic prints as before, but respect signal and continue.
//   - After all threads finish, print total runtime.
//
// Constraints:
//   - Must catch Ctrl+C without exiting.
//   - Time measurement in seconds.
//   - No structs; same overall behavior.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>

// Handler: catch and note SIGINT/SIGTERM
volatile sig_atomic_t sig_caught = 0;
volatile sig_atomic_t max_sig_to_exit = 3; // To track and end the program if it exceeds (Else we haave to wait till the program is done for)

void handle_signal(int sig) {
    sig_caught++;
    max_sig_to_exit--;

    printf("\nCaught signal %d – continuing execution.\n", sig);

    if (max_sig_to_exit == 0) {
        printf("Received 3 signals – force exiting...\n");
        exit(1); // force exit immediately
    }
}


// Optimized primality test (6k ± 1)
bool is_prime_opt(int x) {
    if (x <= 1) return false;
    if (x <= 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (int i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0)
            return false;
    }
    return true;
}

// Thread A: sum N primes, time the compute
void* thread_A(void* arg) {
    int N = *(int*)arg;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int count = 0, num = 2, sum = 0;
    while (count < N) {
        if (is_prime_opt(num)) {
            sum += num;
            count++;
        }
        num++;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double secs = (end.tv_sec - start.tv_sec)
                + (end.tv_nsec - start.tv_nsec)/1e9;

    printf("Thread A: Sum of first %d primes = %d\n", N, sum);
    printf("Thread A: Computation took %.6f seconds\n", secs);
    return NULL;
}

// Thread B: print every 2s up to 100s, note signals
void* thread_B(void* arg) {
    time_t t0 = time(NULL);
    while (time(NULL) - t0 < 100) {
        printf("Thread 1 running\n");
        sleep(2);
        if (sig_caught) {
            printf("Thread 1: signal caught, continuing\n");
            sig_caught = 0;
        }
    }
    return NULL;
}

// Thread C: print every 3s up to 100s, note signals
void* thread_C(void* arg) {
    time_t t0 = time(NULL);
    while (time(NULL) - t0 < 100) {
        printf("Thread 2 running\n");
        sleep(3);
        if (sig_caught) {
            printf("Thread 2: signal caught, continuing\n");
            sig_caught = 0;
        }
    }
    return NULL;
}

int main() {
    // Install signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    int N;
    printf("Enter N (number of primes to sum): ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "Invalid input. Exiting.\n");
        return 1;
    }

    pthread_t tA, tB, tC;
    time_t start_all = time(NULL);

    // CREATE threads
    pthread_create(&tA, NULL, thread_A, &N);
    pthread_create(&tB, NULL, thread_B, NULL);
    pthread_create(&tC, NULL, thread_C, NULL);

    // WAIT for all threads
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);

    time_t end_all = time(NULL);
    printf("Total runtime: %ld seconds\n", end_all - start_all);
    printf("All threads completed. Exiting.\n");
    return 0;
}
