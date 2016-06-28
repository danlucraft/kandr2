/*

Write getfloat, the floating-point analog of getint. What type does getfloat 
return as its function value?

danlucraft: I've made getfloat return the same thing as getint did, viz: 
1 if it's a valid int or float and 0 otherwise.

*/

#include <stdio.h>
#include <ctype.h>

int getch(void);
void ungetch(int);
void debugbuf(void);
int getint(int *);

int getfloat(double *pd);
int getfloat(double *pd)
{
	int sign;
	int c;

	// consume (optional) whitespace
	while (isspace(c = getch()))
		;
	if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetch(c);
		return 0;
	}

	// consume (optional) sign character
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-')
		c = getch();

	// consume (required) digits
	while (isdigit(c)) {
		*pd = 10 * *pd + (c - '0');
		c = getch();
	}
	int den = 1;
	if (c == '.') {
		c = getch();
		while (isdigit(c)) {
			*pd = 10 * *pd + (c - '0');
			den *= 10;
			c = getch();
		}
	}
	*pd /= den;
	*pd *= sign;

	if (c != EOF)
		ungetch(c);

	return c;
}

int main()
{
	double d;
	printf(">> ");
	if (getfloat(&d)) {
		printf("got: %g\n", d);
	} else {
		printf("not a number\n");
	}
	debugbuf();
}


#define BUFSIZE 100

static char buf[BUFSIZE]; /* buffer for ungetch */
static int bufp = 0;      /* next free position in buf */

int getch(void) /* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = (char) c;
}

void debugbuf()
{
	printf("buf: [");
	for (int i = 0; i < bufp; i++)
		printf("%d, ", buf[i]);
	printf("]\n");
}

/*

$ clang -Weverything chapter5/ex_5_02.c && ./a.out
>> 123
got: 123
buf: []
~/Dropbox/Code/kandr2 (master)
$ clang -Weverything chapter5/ex_5_02.c && ./a.out
>> -99
got: -99
buf: []
~/Dropbox/Code/kandr2 (master)
$ clang -Weverything chapter5/ex_5_02.c && ./a.out
>> 123.4
got: 123.4
buf: []
~/Dropbox/Code/kandr2 (master)
$ clang -Weverything chapter5/ex_5_02.c && ./a.out
>> -123.45
got: -123.45
buf: []
~/Dropbox/Code/kandr2 (master)
$ clang -Weverything chapter5/ex_5_02.c && ./a.out
>> 0.3
got: 0.3
buf: []
~/Dropbox/Code/kandr2 (master)
$ clang -Weverything chapter5/ex_5_02.c && ./a.out
>> .3
not a number
buf: [46, ]

*/
