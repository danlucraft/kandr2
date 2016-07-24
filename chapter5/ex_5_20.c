/*

Expand this program to handle declarations with function argument types, 
qualifiers like const, and so on.

--

Qualifier test cases:

 - const char x
 - const x      - syntax error
 - const *x     - syntax error
 - unsigned int x
 - const unsigned int x

Function arguments:

 - char (*x)(int a)
 - char (*x)(int a, char b)
 - char (*x)(int a, char b, void ***c)
 - char (*x)(int a, int (*pf)())
 - char (*x)(int a, char (*(*x(unsigned int p, unsigned double q))[3])())

Error test cases:

 - empty:        ""
 - missing name: char **(*f)(int)
 - extra *:      char **(*f)(int a, *char)
 - trailing ):   char (*x))))

---

This one was a lot of fun! Had to rewrite it to remove the global variables so
it was possible to nest calls to the new outer declration-with-type function,
in order to handle the nested declarations in the functioni arguments.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKEN 1000

enum { NAME, PARENS, BRACKETS };

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

int gettoken(char *token, int *tokentype);
int gettoken(char *token, int *tokentype)
{
	int c, r;
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;

	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			*tokentype = PARENS;
			return 1;
		} else {
			ungetch(c);
			*tokentype = '(';
			return 1;
		}
	} else if (c == '[') {
		for (*p++ = (char) c; (*p++ = (char) (r = getch())) != ']'; ) {
			if (r == '\n' || r == EOF) {
				printf("syntax error: expected ] got end\n");
				return 0;
			}
			if (!isdigit(r)) {
				printf("syntax error: found '%c' inside []\n", r);
				return 0;
			}
		}
		*p = '\0';
		*tokentype = BRACKETS;
		return 1;
	} else if (isalpha(c)) {
		for (*p++ = (char) c; isalnum(c = (char) getch()); )
			*p++ = (char) c;
		*p = '\0';
		ungetch(c);
		*tokentype = NAME;
		return 1;
	} else {
		*tokentype = c;
		return 1;
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

int dcl(int *tokentype, char *token, char *name, char *datatype, char *out);
int dirdcl(int *tokentype, char *token, char *name, char *datatype, char *out);
int dcl_with_type(char *result, int *following_tokentype);

int consume_args(int *tokentype, char *token, char *name, char *datatype, char *out);
int consume_args(int *tokentype, char *token, char *name, char *datatype, char *out)
{
	char result[MAXTOKEN];
	int following_tokentype;
	if (dcl_with_type(result, &following_tokentype)) {
		strcat(out, result);
		if (following_tokentype == ')') {
			strcat(out, ") and returning");
		} else if (following_tokentype == ',') {
			strcat(out, ", ");
			if (!consume_args(tokentype, token, name, datatype, out))
				return 0;
		}
		return 1;
	} else {
		return 0;
	}
}

int dirdcl(int *tokentype, char *token, char *name, char *datatype, char *out)
{
	if (*tokentype == '(') {
		if (dcl(tokentype, token, name, datatype, out)) {
			if (*tokentype != ')') {
				printf("syntax error: expected ) got %s\n", describe_tokentype(*tokentype));
				return 0;
			}
		} else {
			return 0;
		}
	} else if (*tokentype == NAME) {
		if (name[0] == '\0') {
			strcpy(name, token);
		}
	} else {
		printf("syntax error: expected name or declaration got %s\n", describe_tokentype(*tokentype));
		return 0;
	}

	if (gettoken(token, tokentype)) {
		while (*tokentype == '(' || *tokentype == PARENS || *tokentype == BRACKETS) {
			if (*tokentype == '(') {
				strcat(out, " function taking (");
				if (!consume_args(tokentype, token, name, datatype, out))
					return 0;
			} else if (*tokentype == PARENS) {
				strcat(out, " function returning");
			} else {
				strcat(out, " array");
				strcat(out, token);
				strcat(out, " of");
			}
			if (!gettoken(token, tokentype))
				return 0;
		}
	} else {
		return 0;
	}
	return 1;
}

int dcl(int *tokentype, char *token, char *name, char *datatype, char *out)
{
	int ns;
	for (ns = 0; gettoken(token, tokentype) && *tokentype == '*'; )
		ns++;
	if (!dirdcl(tokentype, token, name, datatype, out))
		return 0;
	while (ns-- > 0) {
		strcat(out, " pointer to");
	}
	return 1;
}

int is_type_qualifier(char *tok);
int is_type_qualifier(char *tok)
{
	return strcmp(tok, "const")    == 0 || 
		   strcmp(tok, "unsigned") == 0;
}

int dcl_with_type(char *result, int *following_tokentype)
{
	int tokentype;           // type of last token
	char token[MAXTOKEN];    // last token string
	char name[MAXTOKEN];     // identifier name
	char datatype[MAXTOKEN]; // data type = char, int, etc.
	char out[1000];          // output string

	name[0] = '\0';

	if (!gettoken(token, &tokentype))
		return 0;

	if (tokentype == '*') {
		printf("syntax error: expected simple type (char, int) got *\n");
		return 0;
	}

	// do nothing if empty input
	if (token[0] != '\0') {
		strcpy(datatype, token);
		while (is_type_qualifier(token)) {
			gettoken(token, &tokentype);
			if (tokentype == NAME) {
				strcat(datatype, " ");
				strcat(datatype, token);
			} else {
				printf("syntax error: expected simple type (char, int) got %s\n", describe_tokentype(tokentype));
				return 0;
			}
		}
		out[0] = '\0';
		if (dcl(&tokentype, token, name, datatype, out)) {
			if (tokentype == '(') {
				printf("syntax error: unclosed (\n");
				return 0;
			} else if (tokentype == ')' || tokentype == ',' || tokentype == '\n') {
				sprintf(result, "%s:%s %s", name, out, datatype);
				*following_tokentype = tokentype;
				return 1;
			} else {
				printf("syntax error: expected end of declaration (')', ',' or '\\n'), got %s\n", describe_tokentype(tokentype));
				return 0;
			}
		} else {
			return 0;
		}
	}
	return 1;
}

int main()
{
	char result[MAXTOKEN];
	int following_tokentype;
	if (!dcl_with_type(result, &following_tokentype))
		return 1;
	if (following_tokentype == '\n' || following_tokentype == EOF) {
		if (result[0] != '\0')
			printf("%s\n", result);
	} else {
		printf("syntax error: unexpected trailing %s\n", describe_tokentype(following_tokentype));
		return 1;
	}
	return 0;
}

/*

// Qualifier test cases:

$ clang -Weverything chapter5/ex_5_20.c  && echo "const char x" | ./a.out
x: const char

$ clang -Weverything chapter5/ex_5_20.c  && echo "const x" | ./a.out
syntax error: expected name or declaration got newline

$ clang -Weverything chapter5/ex_5_20.c  && echo "const *x" | ./a.out
syntax error: expected simple type (char, int) got *

$ clang -Weverything chapter5/ex_5_20.c  && echo "unsigned int x" | ./a.out
x: unsigned int

$ clang -Weverything chapter5/ex_5_20.c  && echo "const unsigned int x" | ./a.out
x: const unsigned int

// Function argument test cases:

$ clang -Weverything chapter5/ex_5_20.c  && echo "char (*x)(int a)" | ./a.out
x: pointer to function taking (a: int) and returning char

$ clang -Weverything chapter5/ex_5_20.c  && echo "char (*x)(int a, char b)" | ./a.out
x: pointer to function taking (a: int, b: char) and returning char

$ clang -Weverything chapter5/ex_5_20.c  && echo "char (*x)(int a, char b, void ***c)" | ./a.out
x: pointer to function taking (a: int, b: char, c: pointer to pointer to pointer to void) and returning char

$ clang -Weverything chapter5/ex_5_20.c  && echo "char (*x)(int a, int (*pf)())" | ./a.out
x: pointer to function taking (a: int, pf: pointer to function returning int) and returning char

$ clang -Weverything chapter5/ex_5_20.c  && echo "char (*x)(int a, char (*(*y(unsigned int p, unsigned double q))[3])())" | ./a.out
x: pointer to function taking (a: int, y: function taking (p: unsigned int, q: unsigned double) and returning pointer to array[3] of pointer to function returning char) and returning char

// Error test cases:

$ clang -Weverything chapter5/ex_5_20.c  && echo "" | ./a.out

$ clang -Weverything chapter5/ex_5_20.c  && echo "char **(*f)(int)" | ./a.out
syntax error: expected name or declaration got )

$ clang -Weverything chapter5/ex_5_20.c  && echo "char **(*f)(int a, *char)" | ./a.out
syntax error: expected simple type (char, int) got *

*/
