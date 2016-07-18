/*

This is to test the behaviour of getline as it seems as though it overwrites
previous lines in between invocations.

Answer: yes it does IF you provide it with an initialized char *, which is
easy to do by accident in a loop.

*/

#include <stdio.h>

int main()
{
	char *line1 = NULL;
	char *line2 = NULL;
	char *line = NULL;
	size_t len;
	getline(&line, &len, stdin);
	line1 = line;
	// this is the key line to make it work:
	line = NULL;
	getline(&line, &len, stdin);
	line2 = line;
	printf("1: %s", line1);
	printf("2: %s", line2);
}
