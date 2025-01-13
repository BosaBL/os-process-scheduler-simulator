#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Clears terminal output in LINUX.
 */
void clearScreen() { system("clear"); }

char *parseString(char *str);

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

void reverse(char s[]) {
  int i, j;
  char c;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

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
