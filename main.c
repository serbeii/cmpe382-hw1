#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define RANGE_MAX 872

int count_primes(int n, FILE* file);
int is_prime(int num);

int main() {
    srand(time(NULL));
    int n;
    printf("Enter the amount of Integers to generate: ");
    scanf("%d", &n);
    FILE* file1;
    file1 = fopen("file1.txt", "w+");
    FILE* file2;
    file2 = fopen("file2.txt", "w+");
    for (int i = 0; i < n; i++) {
        int val1 = rand() % RANGE_MAX;
        int val2 = rand() % RANGE_MAX;
        fprintf(file1, "%d\n", val1);
        fprintf(file2, "%d\n", val2);
    }

    fseek(file1, 0, SEEK_SET);
    fseek(file2, 0, SEEK_SET);

    file1 = fopen("file1.txt", "r");
    file2 = fopen("file2.txt", "r");
    // printf("works0 %d\n", getpid());
    // parent to child
    int p_p2c_1[2];
    int p_p2c_2[2];
    // child to parent
    int p_c2p_1[2];
    int p_c2p_2[2];
    pipe(p_p2c_1);
    pipe(p_p2c_2);
    pipe(p_c2p_1);
    pipe(p_c2p_2);
    int child1 = fork();
    int child2;
    if (child1 == 0) {
        int n;
        read(p_p2c_1[0], &n, sizeof(n));
        // printf("works1 %d\n", getpid());
        int total = count_primes(n, file1);
        // printf("total p1: %d\n", total);
        write(p_c2p_1[1], &total, sizeof(total));
        close(p_p2c_1[1]);
        int x;
        read(p_p2c_1[0], &x, sizeof(x));
        printf("X in P1: %d\n", x);
        if (x > total) {
            printf("I am Child process P1: The winner is child process P2\n");
        }
        exit(0);
    } else {
        child2 = fork();
        if (child2 == 0) {
            int n;
            read(p_p2c_2[0], &n, sizeof(n));
            // printf("works2 %d\n", getpid());
            int total = count_primes(n, file2);
            // printf("total p2: %d\n", total);
            write(p_c2p_2[1], &total, sizeof(total));
            close(p_p2c_2[1]);
            int x;
            read(p_p2c_2[0], &x, sizeof(x));
            printf("X in P2: %d\n", x);
            if (x > total) {
                printf(
                    "I am Child process P2: The winner is child process P1\n");
            }
            exit(0);
        } else {
            // printf("works3 %d\n", getpid());
            write(p_p2c_1[1], &n, sizeof(n));
            write(p_p2c_2[1], &n, sizeof(n));
            int firstval, secondval;
            read(p_c2p_1[0], &firstval, sizeof(firstval));
            read(p_c2p_2[0], &secondval, sizeof(secondval));
            write(p_p2c_1[1], &secondval, sizeof(secondval));
            write(p_p2c_2[1], &firstval, sizeof(firstval));
            wait(NULL);
            printf("The number of positive integers in each file: %d\n", n);
            printf("The number of prime numbers in File1: %d\n", firstval);
            printf("The number of prime numbers in File2: %d\n", secondval);
        }
    }
}

int count_primes(int n, FILE* file) {
    int count = 0;
    int curr;
    int result;
    while ((result = fscanf(file, "%d", &curr)) == 1) {
        if (is_prime(curr)) {
            count++;
        }
    }
    // printf("curr: %d, result: %d\n", count, result);
    return count;
}

int is_prime(int num) {
    for (int i = 2; i < num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}
