/* Should print out all input lines that are longer than 80 characters (in full) */
/* Should print out all input lines that are longer than 80 characters (in fu */
/* Should print out all input lines that are longer than 80 characters (in fu*/
/* Should print out all input lines that are longer than 80 characters (in f*/
/* Should print out all input lines that are longer than 80 characters (in full) Should print out all input lines that are longer than 80 characters (in full) */

#include <stdio.h>

#define MAXLINE  80

int get_line(char s[], int lim);
int get_line_old(char s[], int lim);
void copy(char from[], char to[]);

int main()
{
	int len = 0;
	// need extra 1 to detect if line is 81 chars (or include the newline) and extra 1 to hold \0
	char line[MAXLINE + 2];

	// is this initialization unecessary?
	for (int i = 0; i < MAXLINE + 2; i++)
		line[i] = 0;

	while ((len = get_line(line, MAXLINE + 1)) > 0) {
		int len_exc_newline = line[len - 1] == '\n' ? len - 1 : len;
		if (len_exc_newline == MAXLINE + 1) {
			printf("%s", line);
			int c;
			while ((c = getchar()) != EOF && c != '\n') {
				putchar(c);
			}
			putchar('\n');
		}
	}

	return 0;
}

// lim is max length string can be (not including terminator) tttttttttttttttttttttttttt
// s is lim + 1 long (includes terminator)
// return value is length of string (not including terminator)
// return value of 0 implies no more data
int get_line(char s[], int lim)
{
	int i = 0;
	int c;
	while ((c = getchar()) != EOF) {
		s[i] = (char) c;
		i++;
		if (c == '\n' || i == lim) {
			s[i] = 0;
			return i;
		}
	}
	return i;
}

// PREVIOUS NASTIER IMPLEMENTATION (with slightly different semantics)
// // s: line excluding newline, terminated by a 0
// // lim: max number of chars to put in s
// // returns length (*excluding newline*) ------------------------------------------
// //   length -1 = end of file
int get_line_old(char s[], int lim)
{
	int c;
	int i = 0;
	while((c = getchar()) != EOF) {
		if (c == '\n') {
			s[i] = 0;
			return i;
		}
		s[i] = (char) c;
		i++;
		if (i == lim) {
			return i;
		}
	}
	if (i == 0) {
		return -1;
	}
	s[i + 1] = 0;
	return i;
}
