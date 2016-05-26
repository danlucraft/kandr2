/* Write a loop equivalent to this that doens't use && or || */

#include <stdio.h>

int main()
{	
	int lim = 5;
	int c;

	printf("BEFORE:\n");
	for (int i = 0; i < lim - 1 && (c = getchar()) != '\n' && c != EOF; ++i)
		putchar(c);

	printf("\n\n\nAFTER:\n");
	int i = 0;
	int cont = 1;
	while (cont) {
		if (i == lim - 1) cont = 0;
		c = getchar();
		if (c == '\n')    cont = 0;
		if (c == EOF)     cont = 0;
		putchar(c);
		++i;
	}
}

/*

I guess the point here is that the second form is flipping tedious in
comparison. I didn't really need convincing of that though.

*/