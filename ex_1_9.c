#include   <stdio.h>

int main()
{
	int  c;

	int prevspace = 0;
	while ((c =        getchar()) != EOF) {
		if (c != ' ' || !prevspace) {
			putchar(c);
			prevspace = 0;
		}
		if (c == ' ')
			prevspace = 1;
	}
}

// $ clang 1_9.c && cat 1_9.c | ./a.out
// #include <stdio.h>
//
// int main()
// {
// 	int c;
//
// 	int prevspace = 0;
// 	while ((c = getchar()) != EOF) {
// 		if (c != ' ' || !prevspace) {
// 			putchar(c);
// 			prevspace = 0;
// 		}
// 		if (c == ' ')
// 			prevspace = 1;
// 	}
// }
