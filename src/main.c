#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    Plus
} Operator;

void printMenuMessage() {
  printf("\n");
  printf("1. Bisection method\n");
  printf("2. Regula falsi method\n");
  printf("3. Newton-Raphson method\n");
  printf("4. Inverse of an N x N matrix (N should be a dynamic value, "
                  "not small numbers like 3-4)\n");
  printf("5. Gaussian elimination\n");
  printf("6. Gauss-Seidel methods\n");
  printf("7. Numerical differentiation (with central, forward and "
                  "backward differences options)\n");
  printf("8. Simpson's method\n");
  printf("9. Trapezoidal method\n");
  printf("10. Gregory-Newton interpolation without variable transformation\n");
  printf("11. Exit\n");
  printf("\n");
  printf("Choice: ");
}

int main() {
    char *input = malloc(64 * sizeof(char));
    int choice;
    Operator *operators;
    double *constants;
    do {
        printMenuMessage();
        scanf(" %s", input);
        choice = input[0] - '0';
        if (strlen(input) > 1) {
            choice *= 10;
            choice += input[1] - '0';
        }
    } while (choice != 11);
    free(input);
    return 0;
}