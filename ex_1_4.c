#include <stdio.h>

int main()
{
	float fahr, celsius;
	int lower, upper, step;

	lower = -20;
	upper = 150;
	step = 10;

	celsius = lower;
	while (celsius <= upper) {
		// celsius = (5.0/9.0) * (fahr-32.0);
		fahr = celsius * (9.0/5.0) + 32.0;
		printf(" %3.0f %5.0f\n", celsius, fahr);
		celsius = celsius + step;
	}
}
