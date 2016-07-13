/*

Write the program expr, which evaluates a reverse Polish expression from the 
command line, where each operator or operand is a separate argument. For 
example, 234+* evaluates 2 x (3+4).

*/

#include <stdio.h>
#include <ctype.h>

#define STACK_SIZE 100

int collect_value(char *arg, int *value);
int collect_value(char *arg, int *value)
{
	*value = 0;
	int got = 0;
	for (; arg && isdigit(*arg) ; arg++) {
		got = 1;
		*value *= 10;
		*value += *arg - '0';
	}
	return got;
}

static int stack[STACK_SIZE];
static int *pstack = stack;

int push(int value);
int push(int value)
{
	if (pstack < stack + STACK_SIZE) {
		*pstack = value;
		pstack++;
		return 1;
	} else {
		return 0;
	}
}

int pop(int *value);
int pop(int *value)
{
	if (pstack > stack) {
		pstack--;
		*value = *pstack;
		return 1;
	} else {
		return 0;
	}
}

int main(int argc, char *argv[])
{
	(void)(argc); // silences unused parameter warning. Not a fan.

	int value;
	int value2;

	while (*++argv) {
		if (collect_value(*argv, &value)) {
			if (!push(value)) {
				printf("error: stack full\n");
			}
		} else {
			switch(*argv[0]) {
				case '+':
					if (pop(&value) && pop(&value2)) {
						push(value + value2);
					} else {
						printf("error: stack empty\n");
					}
					break;
				case '-':
					if (pop(&value) && pop(&value2)) {
						push(value2 - value);
					} else {
						printf("error: stack empty\n");
					}
					break;
				case '*':
					if (pop(&value) && pop(&value2)) {
						push(value * value2);
					} else {
						printf("error: stack empty\n");
					}
					break;
				case '/':
					if (pop(&value) && pop(&value2)) {
						push(value2 / value);
					} else {
						printf("error: stack empty\n");
					}
					break;
				case '.':
					if (pop(&value)) {
						push(value);
						printf("%d\n", value);
					} else {
						printf("error: stack empty\n");
					}
					break;
				default: 
					printf("unknown op '%c'\n", *argv[0]);
			}
		}
	}
	if (pstack > stack) {
		pop(&value);
		printf("%d\n", value);
	}
}

/*

$ clang -Weverything chapter5/ex_5_10.c && ./a.out 10 2 - 10 \* . 2 / 1 +
80
41

*/
