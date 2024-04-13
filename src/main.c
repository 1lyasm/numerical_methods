#include <stdio.h>

void menumsg(void) {
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
    char inp = '\0';
    while (inp != 'q') {
        menumsg();
        scanf(" %c", &inp);
    }
    return 0;
}
