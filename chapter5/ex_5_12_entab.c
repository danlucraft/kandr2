/*

Extend entab to accept the shorthand `entab -m +n` to mean tab stops every _n_
columns, starting at column _m_. Choose convenient (for the user) default
behaviour.

NB this file is indented with spaces to make it a test case for itself.

*/

#include <stdio.h>
#include <ctype.h>

#define DEFAULT_TAB_WIDTH  4
#define DEFAULT_TAB_START  0

// have fixed this so it won't set val unless it gets a valid integer
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
    int space_run = 0;
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

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            ++space_run;
        }
        else if (c == '\n') {
            column = -1;
            space_run = 0;
            putchar(c);
        } else {
            if (space_run == 1) {
                putchar(' ');
                space_run = 0;
            }
            while (space_run > 0) {
                int start_of_space_run = column - space_run;
                int next_stop = next_tab_stop(start_of_space_run);
                if (next_stop <= column) {
                    putchar('\t');
                    space_run -= next_stop - start_of_space_run;
                } else {
                    for (; space_run > 0; space_run--)
                        putchar(' ');
                }
            }

            putchar(c);
        }
        ++column;
    }
}

// test line
//  b   c           d e f   g                                                                                           h
//                      b

/*

clang -Weverything chapter5/ex_5_12_entab.c && tail -n 76 chapter5/ex_5_12_entab.c | ./a.out -4 +8 | ruby -e 'STDIN.read.split("\n").each {|l| p l}'
""
"static int tab_width = DEFAULT_TAB_WIDTH;"
"static int tab_start = DEFAULT_TAB_START;"
""
"int next_tab_stop(int col);"
"int next_tab_stop(int col)"
"{"
"\tif (col < tab_start)"
"\t\treturn tab_start;"
""
"\treturn (((col + tab_width) - tab_start) / tab_width ) * tab_width + tab_start;"
"}"
""
"int main(int argc, char *argv[])"
"{"
"\tint column = 0;"
"\tint space_run = 0;"
"\tint c;"
"\tif (argc > 1) {"
"\t    argv++;"
"\t    while (argv[0]) {"
"\t\tif (argv[0][0] == '-')"
"\t\t    to_i(&argv[0][1], &tab_start);"
"\t\tif (argv[0][0] == '+')"
"\t\t    to_i(&argv[0][1], &tab_width);"
"\t\targv++;"
"\t    }"
"\t}"
""
"\twhile ((c = getchar()) != EOF) {"
"\t    if (c == ' ') {"
"\t\t++space_run;"
"\t    }"
"\t    else if (c == '\\n') {"
"\t\tcolumn = -1;"
"\t\tspace_run = 0;"
"\t\tputchar(c);"
"\t    } else {"
"\t\tif (space_run == 1) {"
"\t\t    putchar(' ');"
"\t\t    space_run = 0;"
"\t\t}"
"\t\twhile (space_run > 0) {"
"\t\t    int start_of_space_run = column - space_run;"
"\t\t    int next_stop = next_tab_stop(start_of_space_run);"
"\t\t    if (next_stop <= column) {"
"\t\t\tputchar('\\t');"
"\t\t\tspace_run -= next_stop - start_of_space_run;"
"\t\t    } else {"
"\t\t\tfor (; space_run > 0; space_run--)"
"\t\t\t    putchar(' ');"
"\t\t    }"
"\t\t}"
""
"\t\tputchar(c);"
"\t    }"
"\t    ++column;"
"\t}"
"}"
""
"// test line"
"//\tb   c\t\td e f\tg\t\t\t\t\t\t\t\t\t\t\t    h"
"//\t\t\t    b"
""

*/
