#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLUE_TEXT "\033[34m"
#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[31m"

#define RESET_TEXT "\033[0m"

static void *failMalloc(size_t nByte) {
  void *buffer = malloc(nByte);

  if (buffer == NULL) {
    fprintf(stderr, "failMalloc: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  return buffer;
}

static void debug(const char *format, ...) {
  va_list arguments;

  va_start(arguments, format);

  printf(BLUE_TEXT);
  vprintf(format, arguments);
  printf(RESET_TEXT);

  va_end(arguments);
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

/*
 * Wrapper for fgets that exits current process if fgets fails
 */
static void readLine(char *string, int nCharacter, FILE *stream) {
  char *readResult;

  do {
    readResult = fgets(string, nCharacter, stream);
  } while (strlen(string) < 2 || strchr(string, '\n') == NULL);

  if (readResult == NULL) {
    fprintf(stderr, RED_TEXT "readLine: fgets failed\n" RESET_TEXT);
    exit(EXIT_FAILURE);
  }
}

/*
 * Returns result of sscanf
 */
static int readChoice(int *choice, char *line, int maxLineSize) {
  int scanResult;

  readLine(line, maxLineSize, stdin);
  scanResult = sscanf(line, " %d", choice);

  return scanResult;
}

static int validateInput(int scanResult, int choice, int firstChoice,
                         int lastChoice) {
  int isValid = 0;

  if (scanResult <= 0) {
    printf(RED_TEXT "\nCould not scan the input");
  } else if (choice < firstChoice || choice > lastChoice) {
    printf(RED_TEXT "\nChoice is out of bounds");
  } else {
    isValid = 1;
  }

  if (isValid == 0) {
    printf(", try again\n" RESET_TEXT);
  }

  return isValid;
}

static void bisection(void) { debug("\nbisection: called\n"); }

static void regulaFalsi(void) { debug("\nregulaFalsi: called\n"); }

static void newtonRaphson(void) { debug("\nnewtonRaphson: called\n"); }

static void inverse(void) { debug("\ninverse: called\n"); }

static void gaussianElimination(void) {
  debug("\ngaussianElimination: called\n");
}

static void gaussSeidel(void) { debug("\ngaussSeidel: called\n"); }

static void differentiation(void) { debug("\ndifferentiation: called\n"); }

static void simpsons(void) { debug("\nsimpsons: called\n"); }

static void trapezoidal(void) { debug("\ntrapezoidal: called\n"); }

static void gregoryNewton(void) { debug("\ngregoryNewton: called\n"); }

static void runMainMenu(void) {
  int EXIT_CHOICE = 11;
  int FIRST_CHOICE = 1;
  size_t MAX_LINE_SIZE = 256;

  char *line;
  int choice;
  int scanResult;
  int isValid;

  line = failMalloc(MAX_LINE_SIZE * sizeof(char));

  do {
    printMainMenuMessage();

    scanResult = readChoice(&choice, line, (int)MAX_LINE_SIZE);
    isValid = validateInput(scanResult, choice, FIRST_CHOICE, EXIT_CHOICE);

    if (isValid == 1 && choice != EXIT_CHOICE) {
      if (choice == 1) {
        bisection();
      } else if (choice == 2) {
        regulaFalsi();
      } else if (choice == 3) {
        newtonRaphson();
      } else if (choice == 4) {
        inverse();
      } else if (choice == 5) {
        gaussianElimination();
      } else if (choice == 6) {
        gaussSeidel();
      } else if (choice == 7) {
        differentiation();
      } else if (choice == 8) {
        simpsons();
      } else if (choice == 9) {
        trapezoidal();
      } else if (choice == 10) {
        gregoryNewton();
      } else {
        fprintf(stderr, "runMainMenu: choice has invalid value\n");
        exit(EXIT_FAILURE);
      }
    }
  } while (choice != EXIT_CHOICE);

  free(line);
}

int main(void) {
  runMainMenu();

  return 0;
}
