#define DOUBLE_HASHSIZE 202
#define HASHSIZE (DOUBLE_HASHSIZE/2)
/*

Implement a simple version of the #define processor (i.e., no arguments)
suitable for use with C programs, based on the routines of this section. You 
may also find getch and ungetch helpful.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

#define PI (355.0/113.0)

static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s);
unsigned hash(char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = (unsigned int) *s + 31 * hashval;
	return hashval % HASHSIZE;
}

struct nlist *lookup(char *s);
struct nlist *lookup(char *s)
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;
	return NULL;
}

struct nlist *install(char *name, char *defn);
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) {
		np = malloc(sizeof *np);
		if (np == NULL)
			return NULL;
		np->next = NULL;
		np->name = NULL;
		np->defn = NULL;
		if ((np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	} else {
		free((void *) np->defn);
	}
	if ((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

void undef(char *name);
void undef(char *name)
{
	struct nlist *found = NULL, *np = NULL, *prev = NULL;

	unsigned hashval = hash(name);

	if (hashtab[hashval] == NULL)
		return;

	if (strcmp(name, hashtab[hashval]->name) == 0) {
		found = hashtab[hashval];
		hashtab[hashval] = hashtab[hashval]->next;
	} else {
		for (prev = hashtab[hash(name)], np = prev->next; np != NULL; prev = np, np = np->next)
			if (strcmp(name, np->name) == 0) {
				found = np;
				prev->next = np->next;
			}
	}

	if (found != NULL) {
		free((void *) found->name);
		free((void *) found->defn);
		free((void *) found);
	}
}

void test_lookup(char *name);
void test_lookup(char *name)
{
	struct nlist *np = lookup(name);
	printf("%s -> %s\n", name, (np == NULL ? "<not found>" : np->defn));
}

int strprefix(char *line, char *prefix);
int strprefix(char *line, char *prefix)
{
	char *a, *b;
	for (a=line, b=prefix; *a != '\0' && *a == *b; a++, b++)
		;
	return *b == '\0';
}

#define MAX_DEFINE_NAME 63  // apparently the max identifier length in C standard
#define MAX_DEFINE_DEFN 500 // made up by me

char *consume_space(char *p);
char *consume_space(char *p)
{
	while (isspace(*p))
		p++;
	return p;
}

int extract_define(char *line, char **name, char **defn);
int extract_define(char *line, char **name, char **defn)
{
	if (strprefix(line, "#define ")) {
		char *p = line + 7;
		p = consume_space(p);

		char *name_work = malloc((MAX_DEFINE_NAME + 1) * sizeof(char));
		char *name_workp = name_work;

		while ((isalpha(*p) || *p == '_') && name_workp < name_work + MAX_DEFINE_NAME)
			*name_workp++ = *p++;
		*name_workp = '\0';
		*name = strdup(name_work);

		p = consume_space(p);

		char *defn_work = malloc((MAX_DEFINE_DEFN + 1) * sizeof(char));
		char *defn_workp = defn_work;

		while (!isspace(*p) && *p != '\n' && *p != EOF && defn_workp < defn_work + MAX_DEFINE_DEFN)
			*defn_workp++ = *p++;
		*defn_workp = '\0';
		*defn = strdup(defn_work);

		free(name_work);
		free(defn_work);

		return 1;
	} else {
		return 0;
	}
}

char *replace_range_in(char *line, unsigned long start, unsigned long end, char *replacement);
char *replace_range_in(char *line, unsigned long start, unsigned long end, char *replacement)
{
	unsigned long len = strlen(line);
	unsigned long replacement_len = strlen(replacement);
	unsigned long newlen = len - (end - start) + replacement_len;

	if (start > len || end > len)
		return NULL;

	char *newline = malloc(newlen * sizeof(char));
	newline[0] = '\0';

	strncat(newline, line, start);
	strncat(newline, replacement, replacement_len);
	strncat(newline, (line + end), len - end);

	return newline;
}

int get_word(char *line, char **word, char **start, char**end);
int get_word(char *line, char **word, char **start, char**end)
{
	char *p = line;
	char *name_work;

	while (!isalnum(*p) && *p != '_') {
		if (*p == '\n' || *p == EOF || *p == '\0')
			return 0;
		p++;
	}

	name_work = malloc((MAX_DEFINE_NAME + 1) * sizeof(char));
	char *name_workp = name_work;
	char *start_of_word = p;

	while (isalnum(*p) || *p == '_')
		*name_workp++ = *p++;
	*name_workp = '\0';

	*word = strdup(name_work);
	*start = start_of_word;
	*end = p;

	free(name_work);

	return 1;
}

// NB line may be freed in this call, only use the return value
char *replace_defines(char *line);
char *replace_defines(char *line)
{
	char *word;
	char *start = NULL;
	char *end = NULL;
	char *current = line;
	char *newline = line;
	char *old_newline;
	while (get_word(current, &word, &start, &end)) {
		struct nlist *np = lookup(word);
		if (np != NULL) {
			unsigned long start_ix = (unsigned long) (current - line) + (unsigned long) (start - current);
			unsigned long end_ix   = start_ix + strlen(word);
			old_newline = newline;
			newline = replace_range_in(newline, start_ix, end_ix, np->defn);
			free(old_newline);
		}
		current = end;
	}
	return newline;
}

void test_strprefix(char *line, char *prefix);
void test_strprefix(char *line, char *prefix)
{
	printf("  strprefix(\"%s\", \"%s\") = %d\n", line, prefix, strprefix(line, prefix));
}

void test_replace_range_in(char *line, unsigned long start, unsigned long end, char *replacement);
void test_replace_range_in(char *line, unsigned long start, unsigned long end, char *replacement)
{
	printf("  replace_range_in(\"%s\", %lu, %lu, \"%s\") = ", line, start, end, replacement);
	char *result = replace_range_in(line, start, end, replacement);
			
	if (result == NULL)
		printf("NULL\n");
	else
		printf("\n                   \"%s\"\n", result);
}

int args_include(char **argv, char *arg);
int args_include(char **argv, char *arg)
{
	while (*++argv)
		if (strcmp(*argv, arg) == 0)
			return 1;
	return 0;
}


void test_and_exit(void) __attribute__((noreturn));
void test_and_exit()
{
	printf("** Test(strprefix): \n\n");
	test_strprefix("hello, world", "hello");
	test_strprefix("hello, world", "hi");
	test_strprefix("hello, world", "");
	test_strprefix("", "");
	test_strprefix("", "foo");

	printf("\n** Test(replace_range_in):\n\n");

	char test1[] = "hello, world";
	test_replace_range_in(test1, 5, 6, ":");

	char test2[] = "hello, world";
	test_replace_range_in(test2, 0, 5, "hi");

	char test3[] = "hello, world";
	test_replace_range_in(test3, 0, 12, "total");

	char test4[] = "he";
	test_replace_range_in(test4, 0, 15, "too big index");

	exit(0);
}

int main(int __unused argc, char** argv)
{
	double __unused pi = PI;

	if (args_include(argv, "--test"))
		test_and_exit();

	char *line;
	size_t size;

	while ((line = NULL, size = 0, getline(&line, &size, stdin)) > 0) {
		char *name = NULL;
		char *defn = NULL;

		line = replace_defines(line);

		if (extract_define(line, (char **) &name, (char **) &defn)) {
			install(name, defn);
			free(name);
			free(defn);
		}
		printf("%s", line);
		free(line);
	}
}

/*

$ clang -Wno-comment  -Weverything -Wno-padded -Wno-cast-align chapter6/ex_6_06.c && cat chapter6/ex_6_06.c | ./a.out --test
** Test(strprefix):

strprefix("hello, world", "hello") = 1
strprefix("hello, world", "hi") = 0
strprefix("hello, world", "") = 1
strprefix("", "") = 1
strprefix("", "foo") = 0

** Test(replace_range_in):

replace_range_in("hello, world", 5, 6, ":") =
                 "hello: world"
replace_range_in("hello, world", 0, 5, "hi") =
                 "hi, world"
replace_range_in("hello, world", 0, 12, "total") =
                 "total"
replace_range_in("he", 0, 15, "too big index") = NULL

*/

/*

$ clang -Wno-comment -Weverything -Wno-padded -Wno-cast-align chapter6/ex_6_06.c && cat chapter6/ex_6_06.c | ./a.out | less | head -n 7
#define DOUBLE_HASHSIZE 202
#define HASHSIZE (202/2)

/*

Implement a simple version of the #define processor (i.e., no arguments)
suitable for use with C programs, based on the routines of this section. You
may also find getch and ungetch helpful.

*/
