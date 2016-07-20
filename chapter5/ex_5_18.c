/*

Declaration explainer example from Section 5.12

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

static int tokentype;           // type of last token
static char token[MAXTOKEN];    // last token string
static char name[MAXTOKEN];     // identifier name
static char datatype[MAXTOKEN]; // data type = char, int, etc.
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
	int c;
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
		for (*p++ = (char) c; (*p++ = (char) getch()) != ']'; )
			;
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

void dcl(void);
void dirdcl(void);

void dirdcl(void)
{
	int type;

	if (tokentype == '(') {
		dcl();
		if (tokentype != ')')
			printf("error: missing )\n");
	} else if (tokentype == NAME)
		strcpy(name, token);
	else
		printf("error: expected name or (dcl)\n");

	while ( (type=gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
}

void dcl(void)
{
	int ns;
	for (ns = 0; gettoken() == '*'; )
		ns++;
	dirdcl();
	while (ns-- > 0)
		strcat(out, " pointer to");
}

int main()
{
	while (gettoken() != EOF) {
		strcpy(datatype, token);
		out[0] = '\0';
		dcl();
		if (tokentype != '\n')
			printf("syntax error\n");
		printf("%s:%s %s\n", name, out, datatype);
	}
	return 0;
}

/*

$ clang -Weverything chapter5/ex_5_18.c  && echo "char **argv" | ./a.out
argv: pointer to pointer to char

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*(*x[3])())[5]" | ./a.out
x: array[3] of pointer to function returning pointer to array[5] of char

*/
