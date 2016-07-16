/*

*/

#include <stdio.h>
#include <ctype.h>

#define DEFAULT_TAB_WIDTH  4

int to_i(char *s, int *val);
int to_i(char *s, int *val)
{
    *val= 0;
    int got = 0;
    for (; isdigit(s[0]); s++) {
        got = 1;
        *val *= 10;
        *val += s[0] - '0';
    }
    return got;
}

int next_tab_stop(int col, int argc, char *argv[]);
int next_tab_stop(int col, int argc, char *argv[])
{
    if (argc > 1) {
        char **tab_stops = ++argv;
        while (*tab_stops) {
            int stop;
            if (to_i(*tab_stops, &stop) && stop > col) {
                return stop;
            }
            tab_stops++;
        }
    }
    return ((col + DEFAULT_TAB_WIDTH) / DEFAULT_TAB_WIDTH) * DEFAULT_TAB_WIDTH;
}


int main(int argc, char *argv[])
{
    int column = 0;
    int space_run = 0;
    int c;

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
                int next_stop = next_tab_stop(start_of_space_run, argc, argv);
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

$ clang -Weverything chapter5/ex_5_11_entab.c && tail -n 76 chapter5/ex_5_11_entab.c | ./a.out 2 4 8 16 100 | ruby -e 'STDIN.read.split("\n").each {|l| p l}'
"#include <ctype.h>"
""
"#define DEFAULT_TAB_WIDTH  4"
""
"int to_i(char *s, int *val);"
"int to_i(char *s, int *val)"
"{"
"\t\t*val= 0;"
"\t\tint got = 0;"
"\t\tfor (; isdigit(s[0]); s++) {"
"\t\t\tgot = 1;"
"\t\t\t*val *= 10;"
"\t\t\t*val += s[0] - '0';"
"\t\t}"
"\t\treturn got;"
"}"
""
"int next_tab_stop(int col, int argc, char *argv[]);"
"int next_tab_stop(int col, int argc, char *argv[])"
"{"
"\t\tif (argc > 1) {"
"\t\t\tchar **tab_stops = ++argv;"
"\t\t\twhile (*tab_stops) {"
"\t\t\t    int stop;"
"\t\t\t    if (to_i(*tab_stops, &stop) && stop > col) {"
"\t\t\t\treturn stop;"
"\t\t\t    }"
"\t\t\t    tab_stops++;"
"\t\t\t}"
"\t\t}"
"\t\treturn ((col + DEFAULT_TAB_WIDTH) / DEFAULT_TAB_WIDTH) * DEFAULT_TAB_WIDTH;"
"}"
""
""
"int main(int argc, char *argv[])"
"{"
"\t\tint column = 0;"
"\t\tint space_run = 0;"
"\t\tint c;"
""
"\t\twhile ((c = getchar()) != EOF) {"
"\t\t\tif (c == ' ') {"
"\t\t\t    ++space_run;"
"\t\t\t}"
"\t\t\telse if (c == '\\n') {"
"\t\t\t    column = -1;"
"\t\t\t    space_run = 0;"
"\t\t\t    putchar(c);"
"\t\t\t} else {"
"\t\t\t    if (space_run == 1) {"
"\t\t\t\tputchar(' ');"
"\t\t\t\tspace_run = 0;"
"\t\t\t    }"
"\t\t\t    while (space_run > 0) {"
"\t\t\t\tint start_of_space_run = column - space_run;"
"\t\t\t\tint next_stop = next_tab_stop(start_of_space_run, argc, argv);"
"\t\t\t\tif (next_stop <= column) {"
"\t\t\t\t    putchar('\\t');"
"\t\t\t\t    space_run -= next_stop - start_of_space_run;"
"\t\t\t\t} else {"
"\t\t\t\t    for (; space_run > 0; space_run--)"
"\t\t\t\t        putchar(' ');"
"\t\t\t\t}"
"\t\t\t    }"
""
"\t\t\t    putchar(c);"
"\t\t\t}"
"\t\t\t++column;"
"\t\t}"
"}"
""
"// test line"
"//\tb\tc\t    d e f   g\t\t\t\t\t\th"
"//\t\t\t        b"

*/
