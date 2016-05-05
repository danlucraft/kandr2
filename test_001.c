/* I want to see exactly when for loops do their incrementing */

#include <stdio.h>

int main()
{
	int i;
	for (i = 0; i < 10; i++)
		printf("in loop, i=%d\n", i);
	printf("after loop, i=%d\n", i);
	
	// equivalent while loop
	i = 0;
	while (i < 10) {
		printf("in loop, i=%d\n", i);
		i++;
	}
	printf("after loop, i=%d\n", i);
}
