/*

Our version of getword does not properly handle underscores, string constants,
comments, or preprocessor control lines. Write a better version.

---

This is where it gets stupid, why spend ages on this char counting as in 
previous chapters when we're supposed to be learning about structures now?

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "getch.h"

#define MAXWORD 100
//#define NKEYS (sizeof keytab / sizeof(struct key))
#define NKEYS ((int) (sizeof keytab / sizeof keytab[0]))  
  #define TESTTHING goto

struct key {
	char *word;
	int count;
};

// goto goto goto goto goto goto goto goto goto goto
/* auto auto auto auto auto auto auto auto auto auto */




static struct key keytab[] = {
	{ "auto"     , 0 },
	{ "break"    , 0 },
	{ "case"     , 0 },
	{ "char"     , 0 },
	{ "const"    , 0 },
	{ "continue" , 0 },
	{ "default"  , 0 },
	{ "do"       , 0 },
	{ "double"   , 0 },
	{ "else"     , 0 },
	{ "enum"     , 0 },
	{ "extern"   , 0 },
	{ "float"    , 0 },
	{ "for"      , 0 },
	{ "goto"     , 0 },
	{ "if"       , 0 },
	{ "int"      , 0 },
	{ "long"     , 0 },
	{ "register" , 0 },
	{ "return"   , 0 },
	{ "short"    , 0 },
	{ "signed"   , 0 },
	{ "sizeof"   , 0 },
	{ "static"   , 0 },
	{ "struct"   , 0 },
	{ "switch"   , 0 },
	{ "typedef"  , 0 },
	{ "union"    , 0 },
	{ "unsigned" , 0 },
	{ "void"     , 0 },
	{ "volatile" , 0 },
	{ "while"    , 0 }
};

int getword(char *word, int lim);
int getword(char *word, int lim)
{
	int c, c1;
	char *w = word;

	c = getch();
	while (isspace(c)) {
		ungetch(c);
		while (isspace(c = getch()) && c != '\n')
			;
		if (c == '\n') {
			while (isspace(c = getch()) && c != '\n')
				;
			if (c == '#')
				while ((c = getch()) != '\n')
					;
		}
	}

	if (c != EOF)
		*w++ = (char) c;

	if (c == '/') {
		c = getch();
		if (c == '/') {
			while ((c = getch()) != '\n' && c != EOF)
				;
			ungetch(c);
		} else if (c == '*') {
			c1 = getch();
			while (!(c == '*' && c1 == '/')) {
				c = c1;
				c1 = getch();
			}
		}
	}

	// this assumes 'X', '\\', '\X' are the only possibilities
	// and will go badly wrong otherwise
	if (c == '\'') {
		c1 = getch();
		if (c1 == '\\') {
			c1 = getch();
			c1 = getch();
		} else {
			c1 = getch();
		}
	}

	char __unused *testthing1 = "  '  ";
	char __unused *testthing2 = "  \"  ";

	if (c == '"')
		while ((c = getch()) != '"')
			if (c == '\\')
				c = getch();

	if (!isalpha(c)) {
		*w = '\0';
		return c;
	}

	for ( ; --lim > 0; w++)
		if (!isalnum(*w = (char) getch())) {
			ungetch(*w);
			break;
		}

	*w = '\0';
	return word[0];

}

int binsearch(char *word, struct key tab[], int n);
int binsearch(char *word, struct key tab[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}


int main()
{
	int n;
	char word[MAXWORD];

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			if ((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;

	for (n = 0; n < NKEYS; n++)
		if (keytab[n].count > 0)
			printf("%4d %s\n",
					keytab[n].count, keytab[n].word);
	return 0;
}

/*

$ clang -Weverything -Wno-padded -Wno-unused-macros chapter6/getch.c chapter6/ex_6_01.c && cat chapter6/ex_6_01.c | ./a.out
1 break
11 char
4 else
2 for
17 if
14 int
5 return
1 static
4 struct
9 while

*/
