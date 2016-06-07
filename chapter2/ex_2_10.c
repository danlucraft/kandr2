// vim: noai:ts=4:sw=4

/*

Rewrite the function lower, which converts upper case letters to lower case, 
with a conditional expression instead of if-else.

*/

#include <stdio.h>

int lower(int c);
int lower(int c)
{
    if (c >= 'A' && c <= 'Z') 
        return c + 'a' - 'A';
    else
        return c;
}

int mylower(int c);
int mylower(int c)
{
    return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

int main()
{
    printf("lower(A)   = %c\n", lower('A'));
    printf("mylower(A) = %c\n", mylower('A'));
}

/*

$ clang -g -Wno-newline-eof -Weverything ex_2_10.c && ./a.out
lower(A)   = a
mylower(A) = a

*/
