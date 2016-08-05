/*

Rewite the postfix calculator of Chapter 4 to use scanf/sscanf to do the input
and number conversion.

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP   100
#define MAXVAL  100 /* maximum depth of val stack */

static int sp = 0;         /* next free stack position */
static double val[MAXVAL]; /* value stack */

/* push: push f onto the value stack */
void push(double);
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void);
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
void clear(void);
void clear(void)
{
	sp = 0;
}

static double vars[26];
static double last;

void perform_op(char *s);
void perform_op(char *s)
{
	double op1, op2;
	// arithmetic
	if (0 == strcmp("+", s)) {
		push(pop() + pop());
	} else if (0 == strcmp("*", s)) {
		push(pop() * pop());
	} else if (0 == strcmp("-", s)) {
		op2 = pop();
		push(pop() - op2);;
	} else if (0 == strcmp("/", s)) {
		op2 = pop();
		if (op2 != 0.0)
			push(pop() / op2);
		else
			printf("error: zero divisor\n");
	} else if (0 == strcmp("%", s)) {
		op2 = pop();
		push(fmod(pop(),op2));

		// stack manipulation 
	} else if (0 == strcmp("p", s) || 0 == strcmp("peek", s)) {
		op2 = pop();
		printf("\t%.8g\n", op2);
		push(op2);
	} else if (0 == strcmp("d", s) || 0 == strcmp("dup", s)) {
		op2 = pop();
		push(op2);
		push(op2);
	} else if (0 == strcmp("s", s) || 0 == strcmp("swap", s)) {
		op1 = pop();
		op2 = pop();
		push(op1);
		push(op2);
	} else if (0 == strcmp("c", s) || 0 == strcmp("clear", s)) {
		clear();

		// maths
	} else if (0 == strcmp("sin", s)) {
		push(sin(pop()));
	} else if (0 == strcmp("cos", s)) {
		push(cos(pop()));
	} else if (0 == strcmp("tan", s)) {
		push(tan(pop()));
	} else if (0 == strcmp("exp", s)) {
		push(exp(pop()));
	} else if (0 == strcmp("pow", s)) {
		op2 = pop();
		op1 = pop();
		push(pow(op1, op2));

		// pop and print
	} else if (0 == strcmp("\n", s)) {
		// variables
	} else if (s[0] == '>') {
		vars[s[1] - 'a'] = pop();
	} else if (s[1] == '>') {
		push(vars[s[0] - 'a']);
	} else if (0 == strcmp("_", s)) {
		push(last);
	} else {
		printf("error: unknown command %s\n", s);
	}
}

int main()
{
	char *s = malloc(MAXOP * sizeof(char));
	char *line = NULL;
	char *p = NULL;
	size_t cap;
	ssize_t line_len;
	double fval;
	int consumed = 0;

	while ((free(line), line = NULL, line_len = getline(&line, &cap, stdin)) != -1) {
		p = line;
		while (strcmp(p, "\n") != 0 && strcmp(p, "") != 0) {
			while (isspace(*p))
				p++;
			consumed = 0;
			if (sscanf(p, "%lf%n", &fval, &consumed) && consumed > 0)
				push(fval);
			else if (sscanf(p, "%s%n", s, &consumed) && consumed > 0)
				perform_op(s);
			p += consumed;
		}
		last = pop();
		printf("\t%.8g\n", last);
	}
	return 0;
}

/*

$ clang -Weverything chapter7/ex_7_05.c && ./a.out 
1 2 +
	3
_ sin p 10 *
	0.14112001
	1.4112001

*/
