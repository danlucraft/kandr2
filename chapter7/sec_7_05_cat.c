/*

The implementation of cat from Section 7.5

*/

#include <stdio.h>

void filecopy(FILE *ifp, FILE *ofp);
void filecopy(FILE *ifp, FILE *ofp)
{
	int c;
	while ((c = getc(ifp)) != EOF)
		putc(c, ofp);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	
	if (argc == 1)
		filecopy(stdin, stdout);
	else
		while (--argc > 0)
			if ((fp = fopen(*++argv, "r")) == NULL) {
				printf("cat: can't open %s\n", *argv);
				return 1;
			} else {
				filecopy(fp, stdout);
				fclose(fp);
			}
	return 0;
}
