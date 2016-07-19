/*

Add a field-handling capability, so sorting may be done on fields within lines,
each field sorted according to an independent set of options. (The index for 
this book was sorted with -df for the index category and -n for the page 
numbers.)

---

danlucraft: This one looks quite involved...
Don't think it needs to take multiple field sorting options, as long as the
sort is stable... which quicksort is NOT. OK then... maybe it does need to take
multiple sorting options.

Can I assume that the earlier fields have priority?
Should I allow to choose the separator? Or just default to space?

This looks unambiguous and reasonable:

   sort -df-n-rf -s ' '

OK, looking at the problem again, and looking at the index, it appears that it
means it wants to do sorting WITHIN the field (as the list of page numbers is 
sorted within the line in index).

So then in the index example the lines are sorted by the first field, and the 
second field is sorted internally? Or am I overindexing on how the index is
presented and really it's one category and one page number per line?

OK looking in The C Answer Book.... even they punt on this! They don't allow
different sort orders per field at all! AND they don't even handle "fields",
only start and end index positions!!

Given this is a completely underspecified problem, and to do it right would
require a lot of C that we haven't been taught yet, I'm skipping it. 

---

OK after some reflection I decided I should define something that makes sense
as the problem and implement it. So the plan is:

 * swap the quicksort for a different, stable, sort algorithm (insertion sort,
   because it's easy)
 * add the ability to sort by a field, with a customisable separator
 * but DON'T add the ability to specify different sort options for different
   fields

So to sort by -df for field 1 and -n for field 2 (with lines like "foo,12")
you would do (exploiting the stability):

    cat index | sort -n -s , -c 2 | sort -df -s , -c 1 

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 200000
#define MAXLEN 50
#define ALLOCSIZE 100000*12*50
#define DEFAULT_FIELD_SEPARATOR ' '

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

void extract_field(char *s, char *dest, char sep, int field);
void extract_field(char *s, char *dest, char sep, int field)
{
	char *start, *end;
	while (field > 1) {
		while (*s++ != sep)
			;
		field--;
	}
	start = s;
	while (*s != sep && *s != '\0')
		s++;
	end = s;
	strncpy(dest, (char *) start, end - start);
	dest[end - start] = '\0';
}

int cmp(char *s1, char *s2, int numeric, int case_insensitive, int directory_order, char sep, int sort_column);
int cmp(char *s1, char *s2, int numeric, int case_insensitive, int directory_order, char sep, int sort_column)
{
	char a[MAXLEN];
	char b[MAXLEN];
	if (sort_column != 0) {
		extract_field(s1, (char *) &a, sep, sort_column);
		extract_field(s2, (char *) &b, sep, sort_column);
	} else {
		strcpy(a, s1);
		strcpy(b, s2);
	}

	if (numeric)
		return numcmp(a, b);
	else
		return my_strcmp(a, b, case_insensitive, directory_order);
}

void swap(void *v[], int i, int j);
void swap(void *v[], int i, int j)
{
	char *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

void insertion_sort(void *v[], int n, int numeric, int comp_factor, int case_insensitive, int directory_order, char sep, int sort_column);
void insertion_sort(void *v[], int n, int numeric, int comp_factor, int case_insensitive, int directory_order, char sep, int sort_column)
{
	int i;

	for (i = 1; i < n; i++) {
		int j = i;
		while (j > 0 && cmp((char *) v[j-1], (char *) v[j], numeric, case_insensitive, directory_order, sep, sort_column) * comp_factor > 0) {
			swap(v, j, j-1);
			j--;
		}
	}
}

int main(int __unused argc, char *argv[])
{
	int nlines;
	int numeric = 0;
	int comp_factor = 1;
	int case_insensitive = 0;
	int directory_order = 0;
	char field_separator = DEFAULT_FIELD_SEPARATOR;
	int sort_column = 0;

	while (*++argv) {
		if (argv[0][0] == '-') {
			if (argv[0][1] == 's') {
				field_separator = argv[1][0];
			} else if (argv[0][1] == 'c') {
				sort_column = atoi(argv[1]);
			} else {
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
	}

	if ((nlines = readlines(lineptr, allocbuf, MAXLINES)) >= 0) {
		insertion_sort((void **) lineptr, nlines, numeric, comp_factor, case_insensitive, directory_order, field_separator, sort_column);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}
}

/*

$ cat chapter5/ex_5_17.test
null character,5,30
null statement,4,18
null character,3,213
object,2,195
null statement,1,1
null character,10,15
object,56,193

$ clang -Weverything chapter5/ex_5_17.c && cat chapter5/ex_5_17.test | ./a.out -s , -c 3 -n | ./a.out -s , -c 1
null character,10,15
null character,5,30
null character,3,213
null statement,1,1
null statement,4,18
object,56,193
object,2,195

*/
