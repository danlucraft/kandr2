/*

Modify this program so that it does not add redundant parentheses to 
declarations.

---

The issue seems to be that it adds parentheses around a pointer even if there
is no need. There is only a need if the next token is a [] or () which binds
tighter than *.

Test cases:

 - "x * char"      should give "char *x"    not "char (*x)"
 - "x [] * char"   should give "char *x[]"  not "char (*x[])"
 - "x [] * * char" should give "char **x[]" not "char (*(*x[]))"

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

static int tokentype;           // type of last token
static char token[MAXTOKEN];    // last token string
static char out[1000];          // output string

#define BUFSIZE 100

static char buf[BUFSIZE]; /* buffer for ungetch */
static int bufp = 0;      /* next free position in buf */

int getch(void);
int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c);
void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = (char) c;
}

int gettoken(void);
int gettoken(void)
{
	int c, r;
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;

	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = (char) c; (*p++ = (char) (r = getch())) != ']'; ) {
			if (r == '\n' || r == EOF) {
				printf("syntax error: expected ] got end\n");
				return -1;
			}
			if (!isdigit(r)) {
				printf("syntax error: found '%c' inside []\n", r);
				return -1;
			}
		}
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = (char) c; isalnum(c = (char) getch()); )
			*p++ = (char) c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	} else {
		return tokentype = c;
	}
}

int main()
{
	int type;
	int previous_was_pointer = 0;

	char temp[MAXTOKEN];

	while (gettoken() != EOF) {
		strcpy(out, token);
		while ((type = gettoken()) != '\n')
			if (type == PARENS || type == BRACKETS) {
				if (previous_was_pointer) {
					sprintf(temp, "(%s)", out);
					strcpy(out, temp);
				}
				previous_was_pointer = 0;
				strcat(out, token);
			} else if (type == '*') {
				sprintf(temp, "*%s", out);
				strcpy(out, temp);
				previous_was_pointer = 1;
			} else if (type == NAME) {
				sprintf(temp, "%s %s", token, out);
				strcpy(out, temp);
				previous_was_pointer = 0;
			} else {
				printf("invalid input at %s\n", token);
			}
		printf("%s\n", out);
	}
	return 0;
}

/*

$ clang -Weverything chapter5/ex_5_19.c  && echo "x () * [] * () char" | ./a.out
char (*(*x())[])()

// Test cases:

$ clang -Weverything chapter5/ex_5_19.c  && echo "x * char" | ./a.out
char *x

$ clang -Weverything chapter5/ex_5_19.c  && echo "x [] * char" | ./a.out
char *x[]

*/
