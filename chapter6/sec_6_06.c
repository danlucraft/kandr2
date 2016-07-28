/*

Example code from Section 6.6, a simple hashtable

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

#define HASHSIZE 101

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
		if (np == NULL || (np->name = strdup(name)) == NULL)
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
	test_lookup("hi");
	test_lookup("hello");
	test_lookup("nuhuh");
}




