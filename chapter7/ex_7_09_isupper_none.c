#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
	int sum = 0;
	for (int i = 0; i < INT_MAX; i++) {
		char c = (char) ((rand() % 255) - 128);
		sum += c;
		sum %= 1000000;
	}
	printf("sum: %d\n", sum);
}
