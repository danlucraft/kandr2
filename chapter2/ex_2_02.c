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
	while (1) {
		if (i == lim - 1) break;
		c = getchar();
		if (c == '\n') break;
		if (c == EOF) break;
		putchar(c);
		++i;
	}
}

/*

I guess the point here is that the second form is flipping tedious in
comparison. I didn't really need convincing of that though/

UPDATE: I did this wrong the first time and didn't short circuit the 
test in the same way as in the first loop. So not such a useless test after 
all :)

*/
