/*

Write a routine ungets(s) that will push back an entire string onto the input. 

Q. Should ungets know about buf and bufp, or should it just use ungetch? 

A. It should know about buf and bufp, because I want the behaviour that it
returns immediately with nothing pushed back if it can't push everything.
If I was happy to have it push part of a string then I could use getch and 
ungetch (although I'd have to modify ungetch to return an error code when 
it can't push).

*/

#include <stdio.h>

#define BUFSIZE 5

void debug_buf(void);
int getch(void);
void ungetch(int);
void ungets(char*);

/* reverse polish calculator */
int main()
{
	printf("enter 3 characters: ");
	// turns 123 into 1123
	//
	char s[] = "              ";
	s[0] = (char) getch();
	s[1] = (char) getch();
	s[2] = (char) getch();
	s[3] = '\0';
	ungets(s);
	ungetch(s[0]);
	debug_buf();
	for (int i = 0; i < 4; i++)
		printf("%c", getch());
	printf("\n");
	debug_buf();

	// accepts length BUFSIZE on empty
	ungets("abcde");
	printf("\n");
	for (int i = 0; i < 5; i++)
		printf("%c", getch());
	printf("\n");
	debug_buf();

	printf("\n");
	// doesn't accept BUFSIZE + 1
	ungets("      ");

	// ungetch should allow full BUFSIZE characters
	debug_buf();

	ungetch('1');
	ungetch('2');
	ungetch('3');
	ungetch('4');
	ungetch('5');
	ungetch('6');
	debug_buf();
}

static char buf[BUFSIZE]; /* buffer for ungetch */
static int bufp = 0;      /* next free position in buf */

void debug_buf()
{
	printf("bufp: %d, \"", bufp);
	for (int i = 0; i < bufp; i++)
		printf("%c", buf[i]);
	printf("\"\n");
}

int getch(void) /* get a (possibly pushed back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = (char) c;
}

void ungets(char* s)
{
	int len = 0;
	while (s[len] != '\0')
		len++;
	printf("ungets len: %d (bufp: %d)\n", len, bufp);

	// This doesn't use >= even though ungetch does. This is 
	// because ungetch is asking "are we *currently* full" whereas
	// this is asking "will we be full after this op".
	if (bufp + len > BUFSIZE)
		printf("ungets: too many characters\n");
	else
		for (int j = len - 1; j >= 0; j--)
			buf[bufp++] = s[j];
}

/*

enter 3 characters: 123
ungets len: 3 (bufp: 0)
bufp: 4, "3211"
1123
bufp: 0, ""
ungets len: 5 (bufp: 0)

abcde
bufp: 0, ""

ungets len: 6 (bufp: 0)
ungets: too many characters
bufp: 0, ""
ungetch: too many characters
bufp: 5, "12345"

*/
