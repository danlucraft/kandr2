/*


*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 200000
#define MAXLEN 10
#define ALLOCSIZE 100000*12*10

int readlines(char *linep[], char allocbuf[], int nlines);
void writelines(char *linep[], int nlines);

void my_qsort(void *linep[], int left, int right,
			   int (*comp)(void *, void *),
			   int comp_factor);
int numcmp(const char *, const char *);
int strcmpi(const char *, const char *);
void swap(void *v[], int i, int j);

static char allocbuf[ALLOCSIZE];
static char *lineptr[MAXLINES];

int main(int __unused argc, char *argv[])
{
	int nlines;
	int numeric = 0;
	int comp_factor = 1;
	int case_insensitive = 0;

	while (*++argv) {
		if (strcmp(argv[0], "-n") == 0)
			numeric = 1;
		if (strcmp(argv[0], "-r") == 0)
			comp_factor = -1;
		if (strcmp(argv[0], "-f") == 0)
			case_insensitive = -1;
	}

	if ((nlines = readlines(lineptr, allocbuf, MAXLINES)) >= 0) {
		int (*func)(void*,void*) = (int (*)(void*,void*))
			(numeric ? numcmp :
					   (case_insensitive ? strcmpi :
										   strcmp)) ;
		my_qsort((void **) lineptr, 0, nlines - 1, func, comp_factor);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}
}

int get_line(char *s, int lim);
char *alloc(int);

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

void writelines(char *linep[], int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *linep++);
}

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

void my_qsort(void *v[], int left, int right,
		      int (*comp)(void *, void *),
			  int comp_factor)
{
	int i, last;
	if (left >= right)
		return;

	swap(v, left, (left+right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((*comp)(v[i], v[left]) * comp_factor < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	my_qsort(v, left, last-1, comp, comp_factor);
	my_qsort(v, last+1, right, comp, comp_factor);
}

void swap(void *v[], int i, int j)
{
	char *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

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

int charcmpi(char a, char b);
int charcmpi(char a, char b)
{
	if (a >= 'A' && a <= 'Z')
		a += 'a' - 'A';
	if (b >= 'A' && b <= 'Z')
		b += 'a' - 'A';
	if (a < b)
		return -1;
	else if (a > b)
		return 1;
	else
		return 0;
}

// return <0 if cs < ct, 0 if cs = ct, >0 if cs > ct
int strcmpi(const char *cs, const char *ct)
{
	for (; charcmpi(*cs, *ct) == 0 ; cs++, ct++)
		if (*cs == '\0')
			return 0;
	return charcmpi(*cs, *ct);
}

/*

*/
