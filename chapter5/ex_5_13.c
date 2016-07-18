/*

Write the program tail, which prints the last n lines of its input. By default
n is 10, say, but can be changed by an optional argument -n.

The program should behave rationally no matter how unreasonable the input or
the value of n.

danlucraft: I'm going to use library getline for this rather than messing about 
with max line lengths and alloc again.

*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLINES 1000
#define DEFAULT_TAIL 10

static char *lines[MAXLINES];
static char **next = lines;

int to_i(char *s, int *val);
int to_i(char *s, int *val)
{
	int got = 0;
	int result = 0;
	for(; isdigit(s[0]); s++) {
		result *= 10;
		result += s[0] - '0';
	}
	*val = result;
	return got;
}

int main(int __unused argc, char *argv[])
{
	char *line = NULL;
	size_t len;
	int looped = 0;
	int size = DEFAULT_TAIL;

	while (*++argv) {
		if (argv[0][0] == '-')
			to_i(&argv[0][1], &size);
	}
	if (size == 0)
		exit(0);

	while ((line = NULL, getline(&line, &len, stdin) != -1)) {
		*next = line;
		if (next == lines + size - 1) {
			looped = 1;
			next = lines;
		} else {
			next++;
		}
	}

	char **p;
	if (looped) {
		p = next;
		while (p < lines + size)
			printf("%s", *p++);
	}
	p = lines;
	while (p < next)
		printf("%s", *p++);
}

/*

$ clang -g -fsanitize=address -Weverything -Wno-logical-op-parentheses chapter5/ex_5_13.c && cat chapter5/ex_5_13.c | ./a.out -10
	char **p;
	if (looped) {
		p = next;
		while (p < lines + size)
			printf("%s", *p++);
	}
	p = lines;
	while (p < next)
		printf("%s", *p++);
}

*/
