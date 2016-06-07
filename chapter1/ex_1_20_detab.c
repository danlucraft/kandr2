/* Should replace tabs in the input with the proper number of spaces to reach
the next tab stop (works even with longer lines)*/

//        1         2         3         4         5         6         7         8         9
//		1		2		3		4		5		6		7		8		9		10		11		12

// to inspect result:
// clang -g -fsanitize=address -Wno-newline-eof -Weverything ex_1_20_detab.c && cat ex_1_20_detab.c | ./a.out | ruby -e 'STDIN.read.split("\n").each {|l| p l}'

#include <stdio.h>

#define TAB_WIDTH  4

int main()
{
	int column = 0;
	int tab_width = TAB_WIDTH;
	int c;
	while ((c = getchar()) && c != EOF) {
		if (c == '\t') {
			int new_column = ((column + tab_width) / tab_width) * tab_width;
			for (int spaces = new_column - column; spaces > 0; spaces--)
				putchar(' ');
			column = new_column;
		}
		else if (c == '\n') {
			column = 0;
			putchar(c);
		}
		else {
			column++;
			putchar(c);
		}
	}
}

// test line:
//	abc
//	a	b