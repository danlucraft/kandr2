/* 

Extend detab to accept -m +n which means a tab stop every n columns starting
from column m.

*/

#include <stdio.h>
#include <ctype.h>

#define DEFAULT_TAB_WIDTH  4
#define DEFAULT_TAB_START  0

int to_i(char *s, int *val);
int to_i(char *s, int *val)
{
    int result = 0;
    int got = 0;
    for (; isdigit(s[0]); s++) {
        got = 1;
        result *= 10;
        result += s[0] - '0';
    }
	if (got)
		*val = result;
    return got;
}

static int tab_width = DEFAULT_TAB_WIDTH;
static int tab_start = DEFAULT_TAB_START;

int next_tab_stop(int col);
int next_tab_stop(int col)
{
    if (col < tab_start)
		return tab_start;

	return (((col + tab_width) - tab_start) / tab_width ) * tab_width + tab_start;
}

int main(int argc, char *argv[])
{
	int column = 0;
	int c;

    if (argc > 1) {
        argv++;
        while (argv[0]) {
            if (argv[0][0] == '-')
                to_i(&argv[0][1], &tab_start);
            if (argv[0][0] == '+')
                to_i(&argv[0][1], &tab_width);
            argv++;
        }
    }

	while ((c = getchar()) && c != EOF) {
		if (c == '\t') {
			int new_column = next_tab_stop(column);
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

/*

$ clang -Weverything chapter5/ex_5_12_detab.c && tail -n 76 chapter5/ex_5_12_detab.c | ./a.out -2 +8 | ruby -e 'STDIN.read.split("\n").each {|l| p l}'
""
""
"#include <stdio.h>"
"#include <ctype.h>"
""
"#define DEFAULT_TAB_WIDTH  4"
"#define DEFAULT_TAB_START  0"
""
"int to_i(char *s, int *val);"
"int to_i(char *s, int *val)"
"{"
"    int result = 0;"
"    int got = 0;"
"    for (; isdigit(s[0]); s++) {"
"        got = 1;"
"        result *= 10;"
"        result += s[0] - '0';"
"    }"
"  if (got)"
"          *val = result;"
"    return got;"
"}"
""
"static int tab_width = DEFAULT_TAB_WIDTH;"
"static int tab_start = DEFAULT_TAB_START;"
""
"int next_tab_stop(int col);"
"int next_tab_stop(int col)"
"{"
"    if (col < tab_start)"
"          return tab_start;"
""
"  return (((col + tab_width) - tab_start) / tab_width ) * tab_width + tab_start;"
"}"
""
"int main(int argc, char *argv[])"
"{"
"  int column = 0;"
"  int c;"
""
"    if (argc > 1) {"
"        argv++;"
"        while (argv[0]) {"
"            if (argv[0][0] == '-')"
"                to_i(&argv[0][1], &tab_start);"
"            if (argv[0][0] == '+')"
"                to_i(&argv[0][1], &tab_width);"
"            argv++;"
"        }"
"    }"
""
"  while ((c = getchar()) && c != EOF) {"
"          if (c == '\\t') {"
"                  int new_column = next_tab_stop(column);"
"                  for (int spaces = new_column - column; spaces > 0; spaces--)"
"                          putchar(' ');"
"                  column = new_column;"
"          }"
"          else if (c == '\\n') {"
"                  column = 0;"
"                  putchar(c);"
"          }"
"          else {"
"                  column++;"
"                  putchar(c);"
"          }"
"  }"
"}"

*/
