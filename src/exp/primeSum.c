#include <stdio.h>

int main(void) {
    // sums of primes up to 1000
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
        }
    }
    printf("Sum of primes below 1000 is: %d\n", sum);
    return 0;
}
