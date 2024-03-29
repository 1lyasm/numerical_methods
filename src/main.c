#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLUE_TEXT "\033[34m"
#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[31m"

#define RESET_TEXT "\033[0m"

enum FunctionType {
  Polynomial,
  Exponential,
  Sine,
  Cosine,
  Tangent,
  Cotangent,
  Logarithmic,
  Arcsin,
  Arccos,
  Arctan,
  Arccot,
  Linear
};

enum Operator {
  Exponentiation,
  Addition,
  Subtraction,
  Multiplication,
  Division
};

typedef struct {
  double coefficient;
  enum FunctionType type;
  struct Function *argument;
} Function;

typedef struct {
  Function *leftOperand;
  enum Operator operator;
  struct FunctionCombination *rightOperand;
} FunctionCombination;

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

static int compareWord(char *string, char *target) {
  int areEqual = 1;

  for (; *string != '\0' && *target != '\0'; ++string, ++target) {
    if (*string != *target) {
      areEqual = 0;
    }
  }

  if (*target != '\0') {
    areEqual = 0;
  }

  return areEqual;
}

static char *functionTypeToString(enum FunctionType functionType) {
  size_t MAX_STRING_LENGTH = 64;
  char *string = failMalloc((MAX_STRING_LENGTH + 1) * sizeof(char));
  if (functionType == Polynomial) {
    strcpy(string, "Polynomial");
  } else if (functionType == Exponential) {
    strcpy(string, "Exponential");
  } else if (functionType == Sine) {
    strcpy(string, "Sine");
  } else if (functionType == Cosine) {
    strcpy(string, "Cosine");
  } else if (functionType == Tangent) {
    strcpy(string, "Tangent");
  } else if (functionType == Cotangent) {
    strcpy(string, "Cotangent");
  } else if (functionType == Logarithmic) {
    strcpy(string, "Logarithmic");
  } else if (functionType == Arcsin) {
    strcpy(string, "Arcsin");
  } else if (functionType == Arccos) {
    strcpy(string, "Arccos");
  } else if (functionType == Arctan) {
    strcpy(string, "Arctan");
  } else if (functionType == Arccot) {
    strcpy(string, "Arccot");
  } else if (functionType == Linear) {
    strcpy(string, "Linear");
  } else {
    strcpy(string, "Unknown"); // Handle any other cases
  }
  return string;
}

/*
 * Parses one function from the input string and stops
 */
static Function *constructFunctionFromString(char *functionString) {
  Function *function = failMalloc(sizeof(Function));
  char *current = functionString;
  int parsed = 1;
  char *functionTypeString;

  if (*current == 'x') {
    function->coefficient = 1;
    function->type = Linear;
    function->argument = NULL;
  } else if (compareWord(current, "sin") == 1) {
    function->coefficient = 1;
    function->type = Sine;
    function->argument = NULL;
  } else if (compareWord(current, "cos") == 1) {
    function->coefficient = 1;
    function->type = Cosine;
    function->argument = NULL;
  } else {
      printf(RED_TEXT "\nCould not parse function, try again\n" RESET_TEXT);
      parsed = 0;
  }

  if (parsed == 1) {
      functionTypeString = functionTypeToString(function->type);
      debug("constructFunctionFromString: function->coefficient: %f, "
            "function->type: %s, function->argument: %p\n",
            function->coefficient, functionTypeString, function->argument);
      free(functionTypeString);
  }

  return function;
}

static FunctionCombination *
constructFunctionCombinationFromString(char *functionString) {
  FunctionCombination *functionCombination;

  functionCombination = failMalloc(sizeof(FunctionCombination));

  functionCombination->leftOperand =
      constructFunctionFromString(functionString);

  return functionCombination;
}

static void freeFunctionCombination(FunctionCombination *functionCombination) {
  /*
   * TODO: free nested function combinations in right operand
   */
  free(functionCombination->leftOperand);
  free(functionCombination);
}

/*
 * Reads a function from stdin, constructs a Function type, and returns it
 * Caller must free the returned pointer
 */
static FunctionCombination *readFunctionCombination(void) {
  size_t MAX_LINE_SIZE = 256;

  char *functionString;
  FunctionCombination *functionCombination;

  functionString = failMalloc(MAX_LINE_SIZE * sizeof(char));

  printf(GREEN_TEXT "\nEnter the function: " RESET_TEXT);
  readLine(functionString, (int)MAX_LINE_SIZE, stdin);

  functionCombination = constructFunctionCombinationFromString(functionString);

  freeFunctionCombination(functionCombination);
  free(functionString);

  return functionCombination;
}

static void bisection(void) {
  FunctionCombination *functionCombination;

  functionCombination = readFunctionCombination();
}

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
