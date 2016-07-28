/*

Write a function undef that will remove a name and definition from the table
maintained by lookup and install.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

#define HASHSIZE 2

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

int main()
{
	install("hello", "world");
	install("hi", "there");

	install("hello1", "world1");
	install("hi1", "there1");
	install("hello2", "world2");
	install("hi2", "there2");
	install("hello3", "world3");
	install("hi3", "there3");
	install("hello4", "world4");
	install("hi4", "there4");
	install("hello5", "world5");
	install("hi5", "there5");
	install("hello6", "world6");
	install("hi6", "there6");

	test_lookup("hi");

	// exercises the code that finds it at the start of the list
	test_lookup("hello4");
	printf("undefining \"hello4\"\n");
	undef("hello4");
	test_lookup("hello4");

	// exercises the code that finds it in the middle / end of the list
	test_lookup("hello");
	printf("undefining \"hello\"\n");
	undef("hello");
	test_lookup("hello");

	test_lookup("nuhuh");
}

/*

$ clang -Weverything -Wno-padded chapter6/ex_6_05.c && ./a.out
hi -> there
hello6 -> world6
undefining "hello6"
hello6 -> <not found>
hello -> world
undefining "hello"
hello -> <not found>
nuhuh -> <not found>

*/
