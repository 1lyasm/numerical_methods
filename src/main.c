#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[31m"
#define RESET_TEXT "\033[0m"

static void fail(char *msg) {
  fprintf(stderr, "\n%s\n", msg);
  exit(EXIT_FAILURE);
}

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
  int maximumOperatorCount = 64, maximumValueCount = 64;
  int maximumTokenLength = 64;
  int i;
  char **operators = malloc((size_t)maximumOperatorCount * sizeof(char *));
  char **values = malloc((size_t)maximumValueCount * sizeof(char *));
  size_t lineSize = 128;
  char *line = malloc(lineSize * sizeof(char));
  int wantsToExit = 0;

  for (i = 0; i < maximumOperatorCount; ++i) {
    operators[i] = malloc((size_t)maximumTokenLength * sizeof(char));
  }
  for (i = 0; i < maximumValueCount; ++i) {
    values[i] = malloc((size_t)maximumTokenLength * sizeof(char));
  }

  do {
    printMainMenuMessage();
    scanf(" %d", &input);
    if (input == 11) {
      wantsToExit = 1;
    } else {
      printf("\nEnter the function: ");
      // Read line twice to skip extra line
      getline(&line, &lineSize, stdin);
      if (getline(&line, &lineSize, stdin) == -1) {
        fail("getline failed");
      } else {
        int lineLength = (int)strlen(line) - 1;
        line[lineLength] = 0;
        printf("\nLine: '%s'\n", line);
        printf("\nLine length: %d", lineLength);
      }
    }
  } while (!wantsToExit);

  free(line);
  for (i = 0; i < maximumValueCount; ++i) {
    free(values[i]);
  }
  for (i = 0; i < maximumOperatorCount; ++i) {
    free(operators[i]);
  }
  free(values);
  free(operators);

  return 0;
}
