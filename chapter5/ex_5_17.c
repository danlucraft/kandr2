/*

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 200000
#define MAXLEN 10
#define ALLOCSIZE 100000*12*10

static char allocbuf[ALLOCSIZE];
static char *lineptr[MAXLINES];

int get_line(char *s, int lim);
int get_line(char *s, int lim)
{
	int c = 0;
	int i;
	for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
		*s++ = (char) c;
	if (c == '\n') {
		*s++ = (char) c;
		++i;
	}
	*s = '\0';
	return i;
}

int readlines(char *linep[], char allocbuf[], int nlines);
int readlines(char *lines[], char mem[], int maxlines)
{
	int len, nlines;
	char *p = mem;
	nlines = 0;
	while ((len = get_line(p, MAXLEN)) > 0) {
		if (nlines >= maxlines) {
			return -1;
		} else {
			p[len-1] = '\0'; // delete newline
			lines[nlines++] = p;
			p += len;
		}
	}
	return nlines;
}

void writelines(char *linep[], int nlines);
void writelines(char *linep[], int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *linep++);
}

int numcmp(const char *s1, const char *s2);
int numcmp(const char *s1, const char *s2)
{
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}

int charcmp(char a, char b);
int charcmp(char a, char b)
{
	if (a < b)
		return -1;
	else if (a > b)
		return 1;
	else
		return 0;
}

int charcmpi(char a, char b);
int charcmpi(char a, char b)
{
	if (a >= 'A' && a <= 'Z')
		a += 'a' - 'A';
	if (b >= 'A' && b <= 'Z')
		b += 'a' - 'A';
	return charcmp(a, b);
}

int nondchar(char c);
int nondchar(char c)
{
	return !(c >= '0' && c <= '9') &&
		   !(c == ' ') &&
		   !(c >= 'A' && c <= 'Z') &&
		   !(c >= 'a' && c <= 'z');
}

int my_strcmp(const char *cs, const char *ct, int case_insensitive, int directory_order);
int my_strcmp(const char *cs, const char *ct, int case_insensitive, int directory_order)
{
	int (*char_comparer)(char, char) = case_insensitive ? charcmpi : charcmp;

	// consume equal chars
	for (; char_comparer(*cs, *ct) == 0 ; cs++, ct++)
		if (*cs == '\0')
			return 0;

	// if directory ordering, also consume nondchars and any subsequent equal chars
	if (directory_order) {
		while (char_comparer(*cs, *ct) == 0 || nondchar(*cs) || nondchar(*ct)) {
			if (char_comparer(*cs, *ct) == 0) {
				if (*cs == '\0')
					return 0;
				cs++;
				ct++;
			} else if (nondchar(*cs)) {
				cs++;
			} else if (nondchar(*ct)) {
				ct++;
			}
		}
	}

	// compare first non-equal, nondchar (if appropriate)
	return char_comparer(*cs, *ct);
}

int cmp(const char *s1, const char *s2, int numeric, int case_insensitive, int directory_order);
int cmp(const char *s1, const char *s2, int numeric, int case_insensitive, int directory_order)
{
	if (numeric)
		return numcmp(s1, s2);
	else
		return my_strcmp(s1, s2, case_insensitive, directory_order);
}

void swap(void *v[], int i, int j);
void swap(void *v[], int i, int j)
{
	char *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

void my_qsort(void *v[], int left, int right, int numeric, int comp_factor, int case_insensitive, int directory_order);
void my_qsort(void *v[], int left, int right, int numeric, int comp_factor, int case_insensitive, int directory_order)
{
	int i, last;
	if (left >= right)
		return;

	swap(v, left, (left+right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if (cmp(v[i], v[left], numeric, case_insensitive, directory_order) * comp_factor < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	my_qsort(v, left, last-1, numeric, comp_factor, case_insensitive, directory_order);
	my_qsort(v, last+1, right, numeric, comp_factor, case_insensitive, directory_order);
}

int main(int __unused argc, char *argv[])
{
	int nlines;
	int numeric = 0;
	int comp_factor = 1;
	int case_insensitive = 0;
	int directory_order = 0;

	while (*++argv) {
		if (argv[0][0] == '-') {
			char *arg = argv[0];
			while (*++arg) {
				if (arg[0] == 'n')
					numeric = 1;
				if (arg[0] == 'r')
					comp_factor = -1;
				if (arg[0] == 'f')
					case_insensitive = -1;
				if (arg[0] == 'd')
					directory_order = 1;
			}
		}
	}

	if ((nlines = readlines(lineptr, allocbuf, MAXLINES)) >= 0) {
		my_qsort((void **) lineptr, 0, nlines - 1, numeric, comp_factor, case_insensitive, directory_order);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}
}

/*

*/
