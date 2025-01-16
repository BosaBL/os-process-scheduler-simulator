#ifndef UTILS_H
/*
 * Clears terminal output in LINUX.
 */
void clearScreen();

/*
 * Parses and validates a string in the form of P<int>(<int>,<int>,<int>,<int>).
 * if the string is valid, the parsed information will be stored into parsedStr
 */
char *parseString(char *str, char *parsedStr);

/*
 * Counts the ammount of "real" new lines that a file has.
 * "real" new lines are define as a single consecutive line jump chararcter.
 */
int countFileLines(char *fileName);

/*
 * Reverses a character array in-place.
 */
void reverse(char s[]);

/*
 * Transforms an integer to a character array.
 */
char *itoa(int n);
#endif // !UTILS_H
