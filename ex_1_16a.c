/* Should print out the longest line it
receives in it's input (max 80 chars) */

#include <stdio.h>

#define MAXLINE  80

int get_line(char s[], int lim);
void copy(char from[], char to[]);

int main()
{
	int len = 0;
	int max = 0;
	char longest[MAXLINE + 1]; // plus one to allow for terminator
	char line[MAXLINE + 1];

	// is this initialization unecessary?
	for (int i = 0; i < MAXLINE + 1; i++)
		line[i] = longest[i] = 0;

	while ((len = get_line(line, MAXLINE)) > 0) {
		if (len > max) {
			copy(line, longest);
			max = len;
		}
	}

	if (max > 0) {
		longest[max - 1] = 0; // remove newline for printing
		printf("%s\n", longest);
	}

	return 0;
}

// s: line including newline, terminated by a 0
// lim: max number of chars to put in s
// returns length (*including optional newline char*)
//   length 0 = no line
int get_line(char s[], int lim)
{
	int c;
	int i = 0;
	while((c = getchar()) != EOF) {
		if (c == '\n') {
			s[i] = '\n';
			s[i + 1] = 0;
			return i + 1;
		}
		s[i] = (char) c;
		i++;
		if (i == lim) {
			while ((c = getchar()) != EOF && c != '\n')
				; // consume rest of line
			return i;
		}
	}
	s[i + 1] = 0;
	return i;
}

void copy(char from[], char to[])
{
	int i = 0;
	while ((to[i] = from[i]) != 0)
		++i;
}

// my attempt, a bit less obvious:
//
// void copy(char from[], char to[])
// {
// 	char c;
// 	for (int i = 0; (c = from[i]) && c != 0; i++)
// 		to[i] = c;
// }

// $ clang -Weverything ex_1_16a.c && cat ex_1_16a.c | ./a.out
//         char longest[MAXLINE + 1]; // plus one to allow for terminator
