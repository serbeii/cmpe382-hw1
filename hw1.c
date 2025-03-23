#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int find_prime(int n);
void create_data(int n);

int main() {
    int child1, child2;
    int pipe_c1_parent[2];
    int pipe_parent_c1[2];
    printf("Enter number of Integers to generate:");
    int num;
    scanf("%d", &num);
    create_data(num);
    pipe(pipe_c1_parent);
    pipe(pipe_parent_c1);
    child1 = fork();
    if (child1 == 0) {
        FILE* file;
        file = fopen("file1.txt", "r");
        int n;
        int x;
        int total = 0;
        read(pipe_parent_c1[0], &n, sizeof(n));
        for (int i = 0; i < n; i++) {
            fscanf(file, "%d", &x);
            // printf("file1: %d\n", x);
            total += find_prime(x);
        }
        write(pipe_c1_parent[1], &total, sizeof(total));
        int opp;
        read(pipe_parent_c1[0], &opp, sizeof(opp));
        if (opp > total) {
            printf("I am Child process P1: The winner is child process P2\n");
        }
        return 0;
    } else {
        int pipe_c2_parent[2];
        int pipe_parent_c2[2];
        pipe(pipe_c2_parent);
        pipe(pipe_parent_c2);
        child2 = fork();
        if (child2 == 0) {
            FILE* file;
            file = fopen("file2.txt", "r");
            int n;
            int x;
            int total = 0;
            read(pipe_parent_c2[0], &n, sizeof(n));
            for (int i = 0; i < n; i++) {
                fscanf(file, "%d", &x);
                // printf("file2: %d\n", x);
                total += find_prime(x);
            }
            write(pipe_c2_parent[1], &total, sizeof(total));
            int opp;
            read(pipe_parent_c2[0], &opp, sizeof(opp));
            if (opp > total) {
                printf(
                    "I am Child process P2: The winner is child process P1\n");
            }
            return 0;
        } else {
            write(pipe_parent_c1[1], &num, sizeof(num));
            write(pipe_parent_c2[1], &num, sizeof(num));
            int results[2];
            read(pipe_c1_parent[0], &results[0], sizeof(results[0]));
            read(pipe_c2_parent[0], &results[1], sizeof(results[1]));
            write(pipe_parent_c2[1], &results[0], sizeof(results[0]));
            write(pipe_parent_c1[1], &results[1], sizeof(results[1]));
            wait(NULL);
            wait(NULL);
            printf("The number of positive integers in each file: %d\n", num);
            printf("The number of prime numbers in File1: %d\n", results[0]);
            printf("The number of prime numbers in File2: %d\n", results[1]);
        }
    }
}

int find_prime(int n) {
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;
    for (int i = 2; i * i < n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

void create_data(int n) {
    FILE* file1;
    FILE* file2;
    file1 = fopen("file1.txt", "w+");
    file2 = fopen("file2.txt", "w+");
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        fprintf(file1, "%d\n", rand() % 873);
        fprintf(file2, "%d\n", rand() % 873);
    }
    fclose(file1);
    fclose(file2);
}
