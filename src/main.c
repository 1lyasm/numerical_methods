#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[31m"
#define RESET_TEXT "\033[0m"

typedef enum { Constant, Operator } TokenType;

typedef struct {
    TokenType tokenType;
    double constantValue;
    char *operatorValue;
} Token;

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
                    " Inverse of an N x N matrix (N should be a "
                    "dynamic value, "
                    "not small numbers like 3-4)\n");
    printf(RED_TEXT "5." RESET_TEXT " Gaussian elimination\n");
    printf(RED_TEXT "6." RESET_TEXT " Gauss-Seidel methods\n");
    printf(
        RED_TEXT
        "7." RESET_TEXT
        " Numerical differentiation (with central, forward and "
        "backward differences options)\n");
    printf(RED_TEXT "8." RESET_TEXT " Simpson's method\n");
    printf(RED_TEXT "9." RESET_TEXT " Trapezoidal method\n");
    printf(RED_TEXT "10." RESET_TEXT
                    " Gregory-Newton interpolation without "
                    "variable transformation\n");
    printf(RED_TEXT "11." RESET_TEXT " Exit\n");
    printf("\n");

    printf(GREEN_TEXT "Choice: " RESET_TEXT);
}

static void printTokens(Token *tokens, int start, int end) {
    int i;

    printf("\nTokens: [ ");
    for (i = start; i <= end; ++i) {
        if (tokens[i].tokenType == Constant) {
            printf("%lf", tokens[i].constantValue);
        } else {
            printf("'%s'", tokens[i].operatorValue);
        }
        if (i != end) {
            printf(", ");
        }
    }
    printf(" ]\n");
}

static double evaluate(double x, Token *tokens, int start,
                       int end) {
    double result = -1;
    int i;
    int leftParenthesisIndex = -1;
    int rightParenthesisIndex = -1;

    for (i = start; i <= end && leftParenthesisIndex < 0; ++i) {
        if (tokens[i].tokenType == Operator &&
            tokens[i].operatorValue[0] == '(') {
            leftParenthesisIndex = i;
        }
    }
    if (leftParenthesisIndex >= 0) {
        for (i = leftParenthesisIndex;
             i <= end && rightParenthesisIndex < 0; ++i) {
            if (tokens[i].tokenType == Operator &&
                tokens[i].operatorValue[0] == ')') {
                rightParenthesisIndex = i;
            }
        }
    }
    if (leftParenthesisIndex >= 0 &&
        rightParenthesisIndex >= 0) {
        printf("\nLeft parenthesis index: %d\n",
               leftParenthesisIndex);
        printf("\nRight parenthesis index: %d\n",
               rightParenthesisIndex);
        evaluate(x, tokens, leftParenthesisIndex + 1,
                 rightParenthesisIndex - 1);
    } else {
        printTokens(tokens, start, end);

        if (tokens[start + 1].operatorValue[0] == '*') {
            result = tokens[start].constantValue *
                     tokens[start + 2].constantValue;
        } else if (tokens[start + 1].operatorValue[0] == '+') {
            result = tokens[start].constantValue +
                     tokens[start + 2].constantValue;
        }

        printf("\nChild result: %lf\n", result);
    }

    return result;
}

static void bisect(Token *tokens, int tokenCount) {
    double x = 1;
    double result;

    result = evaluate(x, tokens, 0, tokenCount - 1);
    printf("\nBisect: result: %lf\n", result);
}

int main() {
    int input;
    int maximumTokenCount = 64;
    int i;
    Token *tokens =
        malloc((size_t)maximumTokenCount * sizeof(Token));
    size_t lineSize = 128;
    char *line = malloc(lineSize * sizeof(char));
    int wantsToExit = 0;

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
                int tokenCount = 0;

                line[lineLength] = 0;
                for (i = 0; i < lineLength; ++i) {
                    int consumedToken = 1;

                    if (line[i] >= '0' && line[i] <= '9') {
                        double value;
                        int readCount;

                        sscanf(line + i, "%lf%n", &value,
                               &readCount);
                        i += readCount - 1;

                        tokens[tokenCount].tokenType = Constant;
                        tokens[tokenCount].constantValue = value;
                    } else if (line[i] == '+' ||
                               line[i] == '-' ||
                               line[i] == '*' ||
                               line[i] == '/' ||
                               line[i] == '(' ||
                               line[i] == ')') {
                        tokens[tokenCount].tokenType = Operator;
                        tokens[tokenCount].operatorValue =
                            calloc(2, sizeof(char));
                        tokens[tokenCount].operatorValue[0] =
                            line[i];
                    } else {
                        consumedToken = 0;
                    }
                    if (consumedToken) {
                        ++tokenCount;
                    }
                }
                printTokens(tokens, 0, tokenCount - 1);

                if (input == 1) {
                    bisect(tokens, tokenCount);
                }
            }
        }
    } while (!wantsToExit);

    free(line);
    for (i = 0; i < maximumTokenCount; ++i) {
        if (tokens[i].tokenType == Operator) {
            free(tokens[i].operatorValue);
        }
    }
    free(tokens);

    return 0;
}
