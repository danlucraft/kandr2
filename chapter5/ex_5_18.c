/*

Make this program recover from input errors.

--- 

Test cases:

  - empty input                 : "" and "  "
  - missing name                : "char"
  - missing )                   : "char (foo"
  - two missing )               : "char ((foo"
  - extra )                     : "char foo)"
  - * in wrong place            : "*char foo"
  - ( in wrong place            : "char foo*"
  - missing [                   : "char foo]"
  - missing ]                   : "char foo["
  - unclosed internal ]         : "char (*(*x[3)())
  - unclosed function arguments : "char (*foo)("
  - unclosed internal function arguments : "char (*(*foo)([3])([5]"

All of these should exit with status code 1.

I could have done more, but I think I get the point. The error handling code
is a bit janky, and spread throughout the various functions... but then so is
the token parsing code.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

static char *tokentype_names[] = {"unknown", "newline", "eof", ")", "*", "name", "parens", "brackets", "]", "[", "("};

char *describe_tokentype(int type);
char *describe_tokentype(int type)
{
	switch(type) {
		case 10:
			return tokentype_names[1];
		case -1:
			return tokentype_names[2];
		case 41:
			return tokentype_names[3];
		case 42:
			return tokentype_names[4];
		case 0:
			return tokentype_names[5];
		case 1:
			return tokentype_names[6];
		case 2:
			return tokentype_names[7];
		case ']':
			return tokentype_names[8];
		case '[':
			return tokentype_names[9];
		case '(':
			return tokentype_names[10];
		default:
			printf("unknown token type: %d\n", type);
	}
	return tokentype_names[0];
}

int dcl(void);
int dirdcl(void);

int dirdcl(void)
{
	int type;

	if (tokentype == '(') {
		if (dcl()) {
			if (tokentype != ')') {
				printf("syntax error: expected ) got %s\n", describe_tokentype(tokentype));
				return 0;
			}
		} else {
			return 0;
		}
	} else if (tokentype == NAME)
		strcpy(name, token);
	else {
		printf("syntax error: expected name or declaration got %s\n", describe_tokentype(tokentype));
		return 0;
	}

	while ( (type=gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
	if (type == -1) {
		return 0;
	}
	return 1;
}

int dcl(void)
{
	int ns;
	for (ns = 0; gettoken() == '*'; )
		ns++;
	if (!dirdcl())
		return 0;
	while (ns-- > 0)
		strcat(out, " pointer to");
	return 1;
}

int main()
{
	while (gettoken() != EOF) {
		if (tokentype == '*') {
			printf("syntax error: expected simple type (char, int) got *\n");
			return 1;
		}
		// do nothing if empty input
		if (token[0] != '\0') {
			strcpy(datatype, token);
			out[0] = '\0';
			if (dcl()) {
				if (tokentype == '(') {
					printf("syntax error: unclosed (\n");
					return 1;
				} else if (tokentype != '\n') {
					printf("syntax error: expected newline got %s\n", describe_tokentype(tokentype));
					return 1;
				} else {
					printf("%s:%s %s\n", name, out, datatype);
					return 0;
				}
			} else {
				// error: message should already have been printed
				return 1;
			}
		}
	}
	return 0;
}

/*

// Still works:

$ clang -Weverything chapter5/ex_5_18.c  && echo "int *****foo" | ./a.out
foo: pointer to pointer to pointer to pointer to pointer to int

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*(*foo)()[3])[5]" | ./a.out
foo: pointer to function returning array[3] of pointer to array[5] of char

// Errors:

$ clang -Weverything chapter5/ex_5_18.c  && echo "" | ./a.out
$

$ clang -Weverything chapter5/ex_5_18.c  && echo "char" | ./a.out
syntax error: expected name or declaration but got newline

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (foo" | ./a.out; echo $?
syntax error: expected ) got newline

$ clang -Weverything chapter5/ex_5_18.c  && echo "char ((foo" | ./a.out; echo $?
syntax error: expected ) got newline

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo)" | ./a.out
syntax error: expected newline got )

$ clang -Weverything chapter5/ex_5_18.c  && echo "*char foo" | ./a.out
syntax error: expected simple type (char, int) got *

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo*" | ./a.out
syntax error: expected newline got *

$ clang -Weverything chapter5/ex_5_18.c  && echo "foo bar baz" | ./a.out
syntax error: expected newline got name

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo]" | ./a.out
syntax error: expected newline got ]

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo[" | ./a.out
syntax error: expected ] got end

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*(*x[3)())[5]" | ./a.out
syntax error: found ')' inside []

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*foo)(" | ./a.out
syntax error: unclosed (

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*foo)([5]" | ./a.out
syntax error: unclosed (

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*(*foo)([3])[5]" | ./a.out
syntax error: expected ) got (

(Previously:

$ clang -Weverything chapter5/ex_5_18.c  && echo "" | ./a.out
error: expected name or (dcl)
syntax error
:

$ clang -Weverything chapter5/ex_5_18.c  && echo "char" | ./a.out
error: expected name or (dcl)
syntax error
: char

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (foo" | ./a.out
error: missing )
syntax error
foo: char

$ clang -Weverything chapter5/ex_5_18.c  && echo "char ((foo" | ./a.out
error: missing )
error: missing )
syntax error
foo: char

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo)" | ./a.out
syntax error
foo: char
error: expected name or (dcl)
syntax error
foo: foo

$ clang -Weverything chapter5/ex_5_18.c  && echo "*char foo" | ./a.out
syntax error
char:
error: expected name or (dcl)
syntax error
char: foo

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo*" | ./a.out
syntax error
foo: char
error: expected name or (dcl)
syntax error
foo: foo

$ clang -Weverything chapter5/ex_5_18.c  && echo "foo bar baz" | ./a.out
syntax error
bar: foo
error: expected name or (dcl)
syntax error
bar: baz

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo]" | ./a.out
syntax error
foo: char
error: expected name or (dcl)
syntax error
foo: foo

$ clang -Weverything chapter5/ex_5_18.c  && echo "char foo[" | ./a.out
Bus error: 10

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*(*x[3)())[5]" | ./a.out
error: missing )
error: missing )
syntax error
x: array[3)())[5] of pointer to pointer to char

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*foo)(" | ./a.out
syntax error
foo: pointer to char
error: expected name or (dcl)
syntax error
foo: foo

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*foo)([5]" | ./a.out
syntax error
foo: pointer to char
error: expected name or (dcl)
syntax error
foo: [5]

$ clang -Weverything chapter5/ex_5_18.c  && echo "char (*(*foo)([3])[5]" | ./a.out
error: missing )
syntax error
foo: pointer to pointer to array[3] of char
error: expected name or (dcl)
syntax error
foo: [5]

)
*/
