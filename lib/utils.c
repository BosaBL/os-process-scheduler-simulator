#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Clears terminal output in LINUX.
 */
void clearScreen() { system("reset"); }

/*
 * Parses and validates a string in the form of P<int>(<int>,<int>,<int>,<int>).
 * if the string is valid, the parsed information will be stored into parsedStr
 */
void parseString(char *str, char *parsedStr) {
  char *format = "P(,,)";
  int f_idx = 0;

  int length = strlen(str);
  int counter = 0;

  for (int i = 0; i < length; i++) {
    if (str[i] == format[f_idx]) {
      f_idx++;
    }
    if (str[i] == ',')
      counter++;
  }

  if (f_idx != strlen(format) || counter > 3) {
    fprintf(stderr, "ERROR: Sintax error on input file, format should be "
                    "P<int>(<int>,<int>,<int>,<int>)\\n.\n");
    exit(1);
  }

  char *open_brace = strchr(str, '(');
  char *close_brace = strrchr(str, ')');
  int content_len = close_brace - open_brace - 1;

  char *str_ptr = ++open_brace;
  char last = ',';

  for (int i = 0; i < content_len; i++, str_ptr++) {
    if ((*str_ptr == ',' && last == *str_ptr) ||
        (!isdigit(*str_ptr) && *str_ptr != ',')) {
      fprintf(stderr, "ERROR: Sintax error on input file, format should be "
                      "P<int>(<int>,<int>,<int>,<int>)\\n.\n");
      exit(1);
    }

    last = *str_ptr;
  };

  strncpy(parsedStr, open_brace, content_len);
  parsedStr[content_len] = '\0';
};

/*
 * Counts the ammount of "real" new lines that a file has.
 * "real" new lines are define as a single consecutive line jump chararcter.
 */
int countFileLines(char *fileName) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count = 0;

  fp = fopen(fileName, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {

    if (read <= 1 || line[0] == '-') {
      continue;
    }

    count++;
  }

  fclose(fp);
  if (line)
    free(line);

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
