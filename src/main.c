#include <stdio.h>

#define BLUE_TEXT "\033[34m"
#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[31m"

#define RESET_TEXT "\033[0m"

static void printMainMenuMessage(void) {
  printf("\n");
  printf(RED_TEXT "1." RESET_TEXT " Bisection method\n");
  printf(RED_TEXT "2." RESET_TEXT " Regula falsi method\n");
  printf(RED_TEXT "3." RESET_TEXT " Newton-Raphson method\n");
  printf(RED_TEXT "4." RESET_TEXT
                  " Inverse of an N x N matrix (N should be a dynamic value, "
                  "not small numbers like 3-4)\n");
  printf(RED_TEXT "5." RESET_TEXT " Gaussian elimination\n");
  printf(RED_TEXT "6." RESET_TEXT " Gauss-Seidel methods\n");
  printf(RED_TEXT "7." RESET_TEXT
                  " Numerical differentiation (with central, forward and "
                  "backward differences options)\n");
  printf(RED_TEXT "8." RESET_TEXT " Simpson's method\n");
  printf(RED_TEXT "9." RESET_TEXT " Trapezoidal method\n");
  printf(RED_TEXT
         "10." RESET_TEXT
         " Gregory-Newton interpolation without variable transformation\n");
  printf(RED_TEXT "11." RESET_TEXT " Exit\n");
  printf("\n");

  printf(GREEN_TEXT "Choice: " RESET_TEXT);
}

int main() {
  int input;

  do {
      printMainMenuMessage();
      scanf(" %d", &input);
  } while (input != 11);

  return 0;
}

