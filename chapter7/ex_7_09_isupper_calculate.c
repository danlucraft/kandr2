#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int my_toupper(char x);
int my_toupper(char x)
{
	if (x >= 97 && x <= 122)
		return x - ('a' - 'A');
	return x;
}

int main()
{
	int sum = 0;
	for (int i = 0; i < INT_MAX; i++) {
		char c = (char) ((rand() % 255) - 128);
		sum += my_toupper(c) % 1000000;
		sum %= 1000000;
	}
	printf("sum: %d\n", sum);
}
