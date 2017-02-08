// Should remove trailing spaces and tabs, and remove whitespace only lines 
//     (up to 80 chars) 
// NB some editors mess this up by auto removing trailing whitespace
// View with invisibles on to see the effect

#include <stdio.h>
#define MAX_SPACE_RUN  80

int main()
{
	char whitespace[MAX_SPACE_RUN + 1];
	int ix = 0;
	int blank_line = 1;

	int c;
	while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t') {
			if (ix == MAX_SPACE_RUN - 2) {
				printf("%s", whitespace);
				for (int i = 0; i < MAX_SPACE_RUN + 1; i++)       
					whitespace[i] = 0;
				ix = 0;
			}
			whitespace[ix++] = (char) c;	
		} else if (c == '\n') {
			for (int i = 0; i < MAX_SPACE_RUN + 1; i++)       
				whitespace[i] = 0;
			ix = 0;
			if (!blank_line) {
				putchar(c);
			}
			blank_line = 1;
		} else {
			blank_line = 0;
			printf("%s", whitespace);
			for (int i = 0; i < MAX_SPACE_RUN + 1; i++)       
				whitespace[i] = 0;
			ix = 0;
			putchar(c);
		}
	}
	printf("\n");
}

// test lines: ----
//     
//  			
//  		
//                                                                                             

/*
$ clang -Wno-newline-eof -Weverything chapter1/ex_1_18.c && head -n 50 chapter1/ex_1_18.c | ./a.out
// Should remove trailing spaces and tabs, and remove whitespace only lines
//     (up to 80 chars)
#include <stdio.h>
#define MAX_SPACE_RUN  80
int main()
{
	char whitespace[MAX_SPACE_RUN + 1];
	int ix = 0;
	int blank_line = 1;
	int c;
	while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t') {
			if (ix == MAX_SPACE_RUN - 2) {
				printf("%s", whitespace);
				for (int i = 0; i < MAX_SPACE_RUN + 1; i++)
					whitespace[i] = 0;
				ix = 0;
			}
			whitespace[ix++] = (char) c;
		} else if (c == '\n') {
			for (int i = 0; i < MAX_SPACE_RUN + 1; i++)
				whitespace[i] = 0;
			ix = 0;
			if (!blank_line) {
				putchar(c);
			}
			blank_line = 1;
		} else {
			blank_line = 0;
			printf("%s", whitespace);
			for (int i = 0; i < MAX_SPACE_RUN + 1; i++)
				whitespace[i] = 0;
			ix = 0;
			putchar(c);
		}
	}
	printf("\n");
}
// test lines: (NB some editors mess this up by auto removing trailing whitespace)----
//
//
//
//                                                                              

*/
