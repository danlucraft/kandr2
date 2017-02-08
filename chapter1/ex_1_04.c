#include <stdio.h>

int main()
{
	/* K&R2 uses float here, but nowadays literals are doubles */
	double fahr, celsius;
	int lower, upper, step;

	lower = -20;
	upper = 150;
	step = 10;

	celsius = lower;
	printf("  c      f\n----------\n");
	while (celsius <= upper) {
		fahr = celsius * (9.0/5.0) + 32.0;
		printf(" %3.0f %5.0f\n", celsius, fahr);
		celsius = celsius + step;
	}
}
