/* Should print out the length of the longest line in the input
and print it out up to a maximum of 80 characters */
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
		if (max <= MAXLINE)
			longest[max - 1] = 0; // remove newline for printing
		printf("length of longest line: %d\n", max);
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
				i++; // consume rest of line THIS WAS THE ONLY CHANGE FROM 1_16a
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

//2345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

// $ clang -Weverything chapter1/ex_1_16.c && head -n 71 chapter1/ex_1_16.c | ./a.out
// length of longest line: 90
// //234567890123456789012345678901234567890123456789012345678901234567890123456789
