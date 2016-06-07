/*

Write a function escape(s, t) that copies s to t and converts \n to "\\n" etc
Also write unescape that does the opposite.

*/

#include <stdio.h>

void escape(char* s, char* t);
void escape(char* s, char* t)
{
	int i = 0;
	int j = 0;
	while (s[i] != '\0') {
		switch (s[i]) {
		case '\n':
			t[j] = '\\';
			j++;
			t[j] = 'n';
			break;
		case '\t':
			t[j] = '\\';
			j++;
			t[j] = 't';
			break;
		default:
			t[j] = s[i];
			break;
		}
		i++;
		j++;
	}
	t[j] = '\0';
}

void unescape(char* s, char* t);
void unescape(char* s, char* t)
{
	int i = 0;
	int j = 0;
	while (s[i] != '\0') {
		switch (s[i]) {
		case '\\':
			switch(s[i + 1]) {
			case 'n':
				t[j] = '\n';
				i += 2;
				j++;
				break;
			case 't':
				t[j] = '\t';
				i += 2;
				j++;
				break;
			default:
				t[j] = '\\';
				t[j + 1] = s[i + 1];
				i += 2;
				j += 2;
				break;
			}
			break;
		default:
			t[j] = s[i];
			i++;
			j++;
			break;
		}
	}
	t[j] = '\0';
}

int main()
{
	char s[] = "foo\n\tbar\n";
	printf("\"%s\"\n", s);
	char t[] = "                                                       ";
	escape(s, t);
	printf("\"%s\"\n\n", t);

	char s1[] = "foo\\n\\tbar\\n\\";
	printf("unescape:\"%s\"\n", s1);
	char t1[] = "                                                       ";
	unescape(s1, t1);
	printf("\"%s\"\n", t1);
}

/*

Straightforward enough, the main difficulty was in keeping track of my
string indices. (Although I did manage to make the standard bug of
leaving out "breaks" the FIRST time I wrote a switch here.)

*/
