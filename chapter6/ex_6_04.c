/*

Write a program that prints the distinct words in its input sorted into 
decreasing order of frequency of occurrence. Precede each word by its count.

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "getch.h"

#define MAXWORD 100

struct tnode {
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

int getword(char *word, int lim);
int getword(char *word, int lim)
{
	int c;
	char *w = word;

	while (isspace(c = getch()))
		;
	if (c != EOF)
		*w++ = (char) c;

	if (!isalpha(c)) {
		*w = '\0';
		return c;
	}
	for ( ; --lim > 0; w++)
		if ( !isalnum(*w = (char) getch())) {
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}

struct tnode *talloc(void);
struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
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

int strcmpi(const char *cs, const char *ct);
int strcmpi(const char *cs, const char *ct)
{
	for (; charcmpi(*cs, *ct) == 0 ; cs++, ct++)
		if (*cs == '\0')
			return 0;
	return charcmpi(*cs, *ct);
}

struct tnode *addtree(struct tnode *p, char *w);
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if (p == NULL) {
		p = talloc();
		p->word = strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	} else if ((cond = strcmpi(w, p->word)) == 0) {
		p->count++;
	} else if (cond < 0) {
		p->left = addtree(p->left, w);
	} else {
		p->right = addtree(p->right, w);
	}
	return p;
}

void treeprint(struct tnode *p);
void treeprint(struct tnode *p)
{
	if (p != NULL) {
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
	}
}

int tree_size(struct tnode *p);
int tree_size(struct tnode *p)
{
	if (p != NULL) {
		return 1 + tree_size(p->left) + tree_size(p->right);
	} else {
		return 0;
	}
}

int cmp_node(struct tnode *a, struct tnode *b);
int cmp_node(struct tnode *a, struct tnode *b)
{
	if (a->count == b->count)
		return strcmpi(a->word, b->word);
	else if (a->count > b->count)
		return -1; // reversed
	else
		return 1; // reversed
}

void insertion_sort_nodes(struct tnode **nodes, int n);
void insertion_sort_nodes(struct tnode **nodes, int n)
{
	int i, j;
	struct tnode *tmp;

	for (i = 0; i < n - 1; i++) {
		for (j = i; j > 0; j--) {
			if (cmp_node(*(nodes + j - 1), *(nodes + j)) > 0) {
				tmp = *(nodes + j);
				*(nodes + j) = *(nodes + j - 1);
				*(nodes + j - 1) = tmp;
			}
		}
	}
}

void collect_nodes(struct tnode ***collect, struct tnode *node);
void collect_nodes(struct tnode ***collect, struct tnode *node)
{
	if (node != NULL) {
		**collect = node;
		(*collect)++;
		collect_nodes(collect, node->left);
		collect_nodes(collect, node->right);
	}
}

int main()
{
	struct tnode *root;
	char word[MAXWORD];
	int i;

	root = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			root = addtree(root, word);

	int count = tree_size(root);

	struct tnode **nodes = (struct tnode **) malloc((unsigned long) count * sizeof(struct tnode *));
	struct tnode **p = nodes;
	collect_nodes(&p, root);
	insertion_sort_nodes(nodes, count);

	for (i = 0; i < count; i++) {
		printf("%4d %s\n", (*(nodes + i))->count, (*(nodes + i))->word);
	}

	return 0;
}

/*

$ clang -Weverything -Wno-padded chapter6/getch.c chapter6/ex_6_04.c && cat chapter6/ex_6_03.test | ./a.out  | head -n 20
290 the
226 of
148 to
147 and
98 in
96 or
92 shall
87 be
75 a
71 our
64 for
63 WE
55 his
47 HAVE
41 by
41 THAT
35 will
34 all
33 any
33 If

*/
