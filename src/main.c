#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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

    printf(RED_TEXT);
    vprintf(format, arguments);
    printf(RESET_TEXT);

    va_end(arguments);
}

static void printMainMenuMessage(void) {
    printf("\n");
    printf(BLUE_TEXT "1." RESET_TEXT " Bisection method\n");
    printf(BLUE_TEXT "2." RESET_TEXT " Regula falsi method\n");
    printf(BLUE_TEXT "3." RESET_TEXT " Newton-Raphson method\n");
    printf(BLUE_TEXT "4." RESET_TEXT " Inverse of an N x N matrix (N should be a dynamic value, not small numbers like 3-4)\n");
    printf(BLUE_TEXT "5." RESET_TEXT " Gaussian elimination\n");
    printf(BLUE_TEXT "6." RESET_TEXT " Gauss-Seidel methods\n");
    printf(BLUE_TEXT "7." RESET_TEXT " Numerical differentiation (with central, forward and backward differences options)\n");
    printf(BLUE_TEXT "8." RESET_TEXT " Simpson's method\n");
    printf(BLUE_TEXT "9." RESET_TEXT " Trapezoidal method\n");
    printf(BLUE_TEXT "10." RESET_TEXT " Gregory-Newton interpolation without variable transformation\n");
    printf("\n");

    printf(GREEN_TEXT "Choice: " RESET_TEXT);
}

static void runMainMenu(void) {
    size_t MAX_INPUT_LEN = 256;

    char *inputLine;
    char inputCharacter;

    inputLine = failMalloc((MAX_INPUT_LEN + 1) * sizeof(char));

    do {
        printMainMenuMessage();

        scanf(" %s", inputLine);
        inputCharacter = inputLine[0];
    } while (inputCharacter != 'q');

    free(inputLine);
}

int main(void) {
    runMainMenu();

    return 0;
}

