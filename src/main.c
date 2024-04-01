#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLUE_TEXT "\033[34m"
#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[31m"

#define RESET_TEXT "\033[0m"

enum TokT { LPar, RPar, Const, Sin, Cos, Log, Plus, Caret, Var};

union TokVal {
  double num;
  char *text;
};

enum ValT {Num, Text, None};

typedef struct {
  enum TokT tokT;
  union TokVal tokVal;
  enum ValT valT;
} Tok;

static void *fmalloc(size_t nByte) {
  void *buffer = malloc(nByte);

  if (buffer == NULL) {
    fprintf(stderr, "fmalloc: malloc failed\n");
    exit(EXIT_FAILURE);
  }

  return buffer;
}

static void *fcalloc(size_t nItems, size_t itemSize) {
  void *buffer = calloc(nItems, itemSize);

  if (buffer == NULL) {
    fprintf(stderr, "fmalloc: calloc failed\n");
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

/*
 * Caller should free the returned pointer
 */
static char *tokTToStr(enum TokT tokT) {
  size_t N_MAX_CHAR = 256;
  char *str = fmalloc((N_MAX_CHAR + 1) * sizeof(char));
  if (tokT == LPar) {
    strcpy(str, "LPar");
  } else if (tokT == RPar) {
    strcpy(str, "RPar");
  } else if (tokT == Const) {
    strcpy(str, "Const");
  } else if (tokT == Sin) {
    strcpy(str, "Sin");
  } else if (tokT == Cos) {
    strcpy(str, "Cos");
  } else if (tokT == Log) {
    strcpy(str, "Log");
  } else {
    fprintf(stderr, "tokTToStr: token type has invalid value, exiting\n");
    exit(EXIT_FAILURE);
  }
  return str;
}

static void debugToks(Tok *toks, size_t nTok) {
  size_t i;
  debug("debugToks: tokens: [");
  for (i = 0; i < nTok; ++i) {
    char *str = tokTToStr(toks[i].tokT);

    if (toks[i].valT == Num) {
      debug("{%s, %lf} ", str, toks[i].tokVal.num);
    } else if (toks[i].valT == Text) {
      debug("{%s, %s} ", str, toks[i].tokVal.text);
    } else if (toks[i].valT == None) {
        debug("{%s} ", str);
    } else {
        fprintf(stderr, "debugToks: invalid value for value type\n");
        exit(EXIT_FAILURE);
    }
    free(str);
  }
  debug("]\n");
}

static size_t compareWord(char *target, size_t *strIdx, char *str,
                          size_t strLen) {
  size_t targetLen = strlen(target);
  size_t i;
  size_t areEq = 0;

  if (*strIdx + targetLen <= strLen) {
    areEq = 1;
    for (i = 0; i < targetLen && areEq == 1; ++i) {
      if (str[*strIdx + i] != target[i]) {
        areEq = 0;
      }
    }
  }

  if (areEq == 1) {
    *strIdx += targetLen;
  }

  return areEq;
}

static void addTok(Tok *toks, size_t *nTok, enum TokT tokT, union TokVal *newVal, enum ValT valT) {
  toks[*nTok].tokT = tokT;
  toks[*nTok].valT = valT;
  if (valT == Num) {
      toks[*nTok].tokVal.num = newVal->num;
  } else if (valT == Text) {
      toks[*nTok].tokVal.text = newVal->text;
  } else if (valT == None) {

  } else {
      fprintf(stderr, "addTok: invalid value for value type\n");
      exit(EXIT_FAILURE);
  }
  ++*nTok;
}

static int checkConst(double *res, size_t *strIdx, char *str, size_t strLen) {
    int isConst = 0;
    int nRead;
    int scanRes = sscanf(&(str[*strIdx]), " %lf%n", res, &nRead);
    debug("checkConst: result: %lf\n", *res);
    if (!(scanRes == 0 || scanRes == EOF)) {
        isConst = 1;
    }
    if (isConst == 1) {
        *strIdx += nRead;
        debug("checkConst: saw constant, nRead: %d, new strIdx: %zu\n", nRead, *strIdx);
    }
    return isConst;
}

static Tok *tokenize(char *str, size_t *nTok) {
  size_t N_MAX_TOK = 1024;
  Tok *toks = fmalloc(N_MAX_TOK * sizeof(Tok));
  size_t strIdx = 0;
  size_t strLen = strlen(str);
  double const_;
  union TokVal newVal;

  /*
   * TODO: Add other types of tokens, such as plus, caret and variable, to token list
   */

  *nTok = 0;

  while (strIdx < strLen) {
    if (compareWord("sin", &strIdx, str, strLen) == 1) {
      addTok(toks, nTok, Sin, &newVal, None);
    } else if (compareWord("cos", &strIdx, str, strLen) == 1) {
      addTok(toks, nTok, Cos, &newVal, None);
    } else if (compareWord("(", &strIdx, str, strLen) == 1) {
      addTok(toks, nTok, LPar, &newVal, None);
    } else if (compareWord(")", &strIdx, str, strLen) == 1) {
      addTok(toks, nTok, RPar, &newVal, None);
    } else if (strIdx + strlen("log_?") <= strLen &&
               compareWord("log_", &strIdx, str, strLen) == 1) {
      size_t TEXT_LEN = 1;
      char *text = fcalloc((TEXT_LEN + 1), sizeof(char));

      debug("tokenize: taking log branch\n");

      text[0] = str[strIdx];
      ++strIdx;

      debug("tokenize: text: %s\n", text);
      debug("tokenize: text[0]: %c\n", text[0]);

      newVal.text = text;
      addTok(toks, nTok, Log, &newVal, Text);
    } else if (checkConst(&const_, &strIdx, str, strLen) == 1) {
        debug("tokenize: const_: %lf\n", const_);
        newVal.num = const_;
      addTok(toks, nTok, Const, &newVal, Num);
    } else {
      ++strIdx;
    }
  }

  debug("tokenize: token count: %lu\n", *nTok);
  debugToks(toks, *nTok);

  return toks;
}

/*
 * Caller must free the returned pointer
 */
static Tok *readToks(size_t *nTok) {
  size_t MAX_LINE_SIZE = 256;

  char *funcStr;
  Tok *toks;

  funcStr = fmalloc(MAX_LINE_SIZE * sizeof(char));

  printf(GREEN_TEXT "\nEnter the function: " RESET_TEXT);
  readLine(funcStr, (int)MAX_LINE_SIZE, stdin);

  if (funcStr[strlen(funcStr) - 1] == '\n') {
    funcStr[strlen(funcStr) - 1] = '\0';
  }

  toks = tokenize(funcStr, nTok);

  free(funcStr);

  return toks;
}

static void freeToks(Tok *toks, size_t nTok) {
  size_t i;

  for (i = 0; i < nTok; ++i) {
      if (toks[i].valT == Text) {
        free(toks[i].tokVal.text);
      }
  }

  free(toks);
}

static void bisection(void) {
  Tok *toks;
  size_t nTok;

  debug("\nbisection: called\n");

  toks = readToks(&nTok);

  freeToks(toks, nTok);
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

  line = fmalloc(MAX_LINE_SIZE * sizeof(char));

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
