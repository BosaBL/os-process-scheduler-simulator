#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Clears terminal output in LINUX.
 */
void clearScreen() { system("clear"); }

/*
 * Parses and validates a string in the form of P<int>(<int>,<int>,<int>,<int>).
 * if the string is valid, the parsed information will be stored into parsedStr
 */
void parseString(char *str, char *parsedStr) {
  char *format = "P(,,,)";
  int f_idx = 0;

  int length = strlen(str);

  for (int i = 0; i < length; i++) {
    if (str[i] == format[f_idx]) {
      f_idx++;
    }
  }

  if (f_idx != strlen(format)) {
    fprintf(stderr, "ERROR: Sintax error on input file, format should be "
                    "P<int>(<int>,<int>,<int>,<int>).\n");
    exit(1);
  }

  const char *open_brace = strchr(str, '(');
  const char *close_brace = strrchr(str, ')');
  int content_len = close_brace - open_brace - 1;

  strncpy(parsedStr, open_brace + 1, content_len);
  parsedStr[content_len] = '\0';
};

/*
 * Counts the ammount of "real" new lines that a file has.
 * "real" new lines are define as a single consecutive line jump chararcter.
 */
int countFileLines(char *fileName) {
  int count = 0;
  int last = '\n';
  FILE *fp = fopen(fileName, "r");

  if (fp == NULL) {
    fprintf(stderr, "FATAL: File (%s) does not exist.", fileName);
    exit(1);
  }

  for (char c = getc(fp); c != EOF; c = getc(fp)) {
    if (c == '\n' && last != '\n') {
      ++count;
    }
    last = c;
  }

  fclose(fp);

  return count;
};

/*
 * Reverses a character array in-place.
 */
void reverse(char s[]) {
  int i, j;
  char c;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/*
 * Transforms an integer to a character array.
 *
 */
char *itoa(int n) {
  int i, sign;
  char *s;

  if ((sign = n) < 0) /* record sign */
    n = -n;           /* make n positive */
  i = 0;
  do {                     /* generate digits in reverse order */
    s[i++] = n % 10 + '0'; /* get next digit */
  } while ((n /= 10) > 0); /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);

  return s;
}
