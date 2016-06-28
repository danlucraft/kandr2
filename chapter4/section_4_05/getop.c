#include <ctype.h>
#include "calc.h"
#include <stdio.h>

/* getop:   get next operator or numeric operand */
int getop(char s[])
{
	int i = 0;
	int c;

	while ((c = getch(), s[0] = (char) c) == ' ' || c == '\t')
		;

	s[1] = '\0';
	i = 1;

	if (streq(s, "\n"))
		return OP;
	if (s[0] == EOF)
		return EOF;

	while ((c = getch(), s[i++] = (char) c) != ' ' && c != '\t' && c != '\n' && c != EOF)
		;
	ungetch(c);

	s[i - 1] = '\0';

	// the rest of this function checks if the string in s is a number
	int j = 0;

	if (s[j] == '-')
		j++;

	if (!isdigit(s[j]) && s[j] != '.')
		return OP;

	while (isdigit(s[j]))
		j++;

	if (s[j] != '.' && s[j] != '\0')
		return OP;

	if (s[j] == '.')
		j++;

	while (isdigit(s[j]))
		j++;

	if (s[j] != '\0')
		return OP;

	return NUMBER;
}

