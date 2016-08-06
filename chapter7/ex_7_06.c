/*

Write a program to compare two files, printing the first line where they
differ.

---

This can't handle lines longer than 1MB. At some point I should understand good
ways of handling this kind of thing.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_CHUNK 1024*1024

int main(int __unused argc, char **argv)
{
	char *progname = argv[0];
	char *filename_a = argv[1];
	char *filename_b = argv[2];

	if (filename_a == NULL || filename_b == NULL) {
		fprintf(stderr, "Usage: diff FILE FILE\n");
		exit(1);
	}

	FILE *fa = fopen(filename_a, "r");
	FILE *fb = fopen(filename_b, "r");

	if (fa == NULL)
		fprintf(stderr, "%s: couldn't open %s\n", progname, filename_a);
	if (fb == NULL)
		fprintf(stderr, "%s: couldn't open %s\n", progname, filename_b);
	if (fa == NULL || fb == NULL)
		exit(1);

	char linea[LINE_CHUNK];
	char lineb[LINE_CHUNK];

	while (fgets(linea, LINE_CHUNK, fa) != NULL) {
		if (fgets(lineb, LINE_CHUNK, fb) == NULL || strcmp(linea, lineb) != 0) {
			printf("-- %s", linea);
			printf("++ %s", lineb);
			exit(0);
		}
	}

	exit(0);
}


/*

$ clang -Weverything chapter7/ex_7_06.c && ./a.out chapter7/sec_7_05_cat.c chapter7/sec_7_06_cat.c
-- 
++ #include <stdlib.h>

*/
