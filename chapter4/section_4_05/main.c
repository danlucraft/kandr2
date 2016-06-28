#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "calc.h"

#define MAXOP   100 /* max size of operand or operator */

static double vars[26];
static double last;

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

/*

*/
