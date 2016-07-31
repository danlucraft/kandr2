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

int is_word_char(char c);
int is_word_char(char c)
{
	return isalnum(c) || c == '_';
}

int is_non_word_char(char c);
int is_non_word_char(char c)
{
	return !is_word_char(c) && c != '\n' && c != EOF && c != '\0';
}

// returns new place in line
// 0 for end, 1 for word and 2 for nonword
// result is placed in s, NULL means end of input, 
// '\n' always appears as it's own word
char *get_word_or_nonword(char *line, char **s);
char *get_word_or_nonword(char *line, char **s)
{
	char *p = line;
	char *work = malloc((MAX_DEFINE_NAME + 1) * sizeof(char));
	char *workp = work;
	int (*char_discriminator)(char);
	int return_val;
	*s = NULL;

	if (*p == '\n') {
	   *workp++ = '\n';
	   *workp = '\0';
	   *s = strdup(work);
	   free(work);
	   return p + 1;
	}

	if (*p == EOF || *p == '\0') {
		*s = NULL;
		return p;
	}

	if (is_word_char(*p)) {
		char_discriminator = is_word_char;
		return_val = 1;
	} else {
		char_discriminator = is_non_word_char;
		return_val = 2;
	}

	while (char_discriminator(*p) && workp < work + MAX_DEFINE_NAME)
		*workp++ = *p++;

	*workp = '\0';
	*s = strdup(work);
	free(work);

	return p;
}

void test_strprefix(char *line, char *prefix);
void test_strprefix(char *line, char *prefix)
{
	printf("  strprefix(\"%s\", \"%s\") = %d\n", line, prefix, strprefix(line, prefix));
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
		char *current = line;
		char *word = NULL;

		while ((current = get_word_or_nonword(current, &word), word) != NULL) {
			struct nlist *np = lookup(word);
			printf("%s", (np != NULL ? np->defn : word));
		}

		char *name = NULL;
		char *defn = NULL;
		if (extract_define(line, (char **) &name, (char **) &defn)) {
			install(name, defn);
			free(name);
			free(defn);
		}
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

$ clang -Wno-comment  -Weverything -Wno-padded -Wno-cast-align chapter6/ex_6_06.c && cat chapter6/ex_6_06.c | ./a.out | head -7
#define DOUBLE_HASHSIZE 202
#define HASHSIZE (202/2)
/*

Implement a simple version of the #define processor (i.e., no arguments)
suitable for use with C programs, based on the routines of this section. You
may also find getch and ungetch helpful.

*/
