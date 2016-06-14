/*

Add commands:

p - print the top element of the stack without popping
d - duplicate the top element
s - swap the top two elements.
c - clear the stack

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100 /* max size of operand or operator */
#define NUMBER  '0' /* signal that a number was found */

int getop(char []);
void push(double);
double pop(void);
void clear(void);

/* reverse polish calculator */
int main()
{
	int type;
	double op1, op2;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);;
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: zero divisor\n");
			break;
		case '%':
			op2 = pop();
			push(fmod(pop(),op2));
			break;
		case 'p':
			op2 = pop();
			printf("\t%.8g\n", op2);
			push(op2);
			break;
		case 'd':
			op2 = pop();
			push(op2);
			push(op2);
			break;
		case 's':
			op1 = pop();
			op2 = pop();
			push(op1);
			push(op2);
			break;
		case 'c':
			clear();
			break;
		case '\n':
			printf("\t%.8g\n", pop());
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	return 0;
}

#define MAXVAL  100 /* maximum depth of val stack */

static int sp = 0;         /* next free stack position */
static double val[MAXVAL]; /* value stack */

/* push: push f onto the value stack */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0) {
		return val[--sp];
	}
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

/* clear: empty the stack */
void clear(void)
{
	sp = 0;
}

static int getch(void);
static void ungetch(int);

/* getop:   get next operator or numeric operand */
int getop(char s[])
{
	int i, c;

	while ((c = getch(), s[0] = (char) c) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;    /* not a number */

	if (c == '-') {
		/* get then ungetch the next character to see if this
		 * is a negative number or not */
		c = getch();
		ungetch(c);
		if (!isdigit(c) && c != '.') {
			return '-';
		}
	}

	i = 0;
	if (isdigit(c))  /* collect integer part */
		while (c = getch(), s[++i] = (char) c, isdigit(s[i]))
			;
	if (c == '.')    /* collect fraction part */
		while (c = getch(), s[++i] = (char) c, isdigit(s[i]))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
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

/*

$ clang -g -Weverything chapter4/ex_4_04.c && ./a.out
1 p 2 p +
    1
	2
	3
1 d +
	2
2 1 -
	1
2 1 s -
	-1
1 2 c +
error: stack empty
error: stack empty
    0

*/
