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
    printf("works0 %d\n", getpid());
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    int child1 = fork();
    int child2 = 1;
    write(pipe1[1], "writing", 16);
    write(pipe2[1], "writing", 16);
    if (child1 == 0) {
        child2 = fork();
        if (child2 == 0) {
            goto second;
        }
        printf("works1 %d\n", getpid());
        int total = count_primes(n, file1);
        printf("total p1: %d\n", total);
        write(pipe1[1], "done", 16);
        exit(0);
    }
second:
    if (child2 == 0) {
        printf("works2 %d\n", getpid());
        int total = count_primes(n, file2);
        printf("total p2: %d\n", total);
        write(pipe2[1], "done", 16);
        exit(0);
    } else if (child1 != 0) {
        printf("works3 %d\n", getpid());
        wait(NULL);
        wait(NULL);
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
    printf("curr: %d, result: %d",count,result);
    return count;
}

int is_prime(int num) {
    for (int i = 2; i < num; i++) {
        if (num % i == 0) {
            return 1;
        }
    }
    return 0;
}
