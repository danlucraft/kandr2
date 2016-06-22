/*

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100 /* max size of operand or operator */
#define NUMBER  '0' /* signal that a number was found */
#define OP      '1' /* signal that it's an operation / command */

void debug_buf(void);
int getop(char []);
void push(double);
double pop(void);
void clear(void);
int streq(char* a, char* b);

static double vars[26];
static double last;

/* reverse polish calculator */
int main()
{
	int type;
	double op1, op2;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		if (type == NUMBER) {
			push(atof(s));
		} else if (type == OP) {
			// arithmetic
			if (streq("+", s)) {
				push(pop() + pop());
			} else if (streq("*", s)) {
				push(pop() * pop());
			} else if (streq("-", s)) {
				op2 = pop();
				push(pop() - op2);;
			} else if (streq("/", s)) {
				op2 = pop();
				if (op2 != 0.0)
					push(pop() / op2);
				else
					printf("error: zero divisor\n");
			} else if (streq("%", s)) {
				op2 = pop();
				push(fmod(pop(),op2));

			// stack manipulation 
			} else if (streq("p", s) || streq("peek", s)) {
				op2 = pop();
				printf("\t%.8g\n", op2);
				push(op2);
			} else if (streq("d", s) || streq("dup", s)) {
				op2 = pop();
				push(op2);
				push(op2);
			} else if (streq("s", s) || streq("swap", s)) {
				op1 = pop();
				op2 = pop();
				push(op1);
				push(op2);
			} else if (streq("c", s) || streq("clear", s)) {
				clear();

			// maths
			} else if (streq("sin", s)) {
				push(sin(pop()));
			} else if (streq("cos", s)) {
				push(cos(pop()));
			} else if (streq("tan", s)) {
				push(tan(pop()));
			} else if (streq("exp", s)) {
				push(exp(pop()));
			} else if (streq("pow", s)) {
				op2 = pop();
				op1 = pop();
				push(pow(op1, op2));

			// pop and print
			} else if (streq("\n", s)) {
				last = pop();
				printf("\t%.8g\n", last);

			// variables
			} else if (s[0] == '>') {
				vars[s[1] - 'a'] = pop();
			} else if (s[1] == '>') {
				push(vars[s[0] - 'a']);
			} else if (streq("_", s)) {
				push(last);
			} else {
				printf("error: unknown command %s\n", s);
			}
		} else {
			printf("unknown token type: '%c'\n", type);
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

int streq(char* a, char* b)
{
	int i = 0;
	while (a[i] != '\0') {
		if (a[i] != b[i])
			return 0;
		i++;
	}
	if (b[i] != '\0')
		return 0;
	return 1;
}

static int getch(void);
static void ungetch(int);

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

#define BUFSIZE 100

static char buf[BUFSIZE]; /* buffer for ungetch */
static int bufp = 0;      /* next free position in buf */

void debug_buf()
{
	printf("bufp: %d, [", bufp);
	for (int i = 0; i < bufp; i++)
		printf("%d, ", buf[i]);
	printf("]\n");
}

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

*/
