#include <stdio.h>

// Sum of all prime numbers below 1000. Used to verify EXEC type menu items.
int main(int argc, char* argv[]) {
    int sum = 0;
    for (int num = 2; num < 1000; num++) {
        int is_prime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            sum += num;
            if (argc > 1) {
                printf("DEBUG: Found prime: %d, current sum: %d\n", num, sum);
            }
        }
    }
    printf("Sum of primes below 1000 is: %d\n", sum);

    return 0;
}
