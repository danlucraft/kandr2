/*

Write a program that reads a C program and prints in alphabetical order each
group of variable names that are identical in the first 6 characters, but
different somewhere thereafter. Don't count words within strings and comments.
Make 6 a parameter that can be set from the command line.

testcomment1

testcomment1

*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// testcomment2 testcomment2

#define MAXWORD 100

static __unused char* test1 = "teststring";
static __unused char* test2 = "teststring";
static __unused char test3  = 'z';

struct wnode {
	char *word;
	int count;
	struct wnode *left;
	struct wnode *right;
};

int getchar_c(void);
int getchar_c(void)
{
	int c1, c2;

	c1 = getchar();
	if (c1 == '/') {
		// possibly in comment
		c2 = getchar();
		if (c2 == '/') {
			// skip cpp comment
			while ((c1 = getchar()) != '\n')
				;
		} else if (c2 == '*') {
			// skip c comment
			c1 = getchar();
			c2 = getchar();
			while (!(c1 == '*' && c2 == '/')) {
				c1 = c2;
				c2 = getchar();
			}
			c1 = getchar();
		}
	} else if (c1 == '"') {
		// skip string
		while ((c1 = getchar()) != '"')
			if (c1 == '\\')
				c1 = getchar();
		c1 = getchar();
	} else if (c1 == '\'') {
		// skip char
		while ((c1 = getchar()) != '\'')
			if (c1 == '\\')
				c1 = getchar();
		c1 = getchar();
	}
	return c1;
}

int getword(char *word);
int getword(char *word)
{
	int c;
	while (isspace(c = getchar_c()))
		;

	while (!(isalpha(c) || c == '_' || c == EOF))
		c = getchar_c();

	if (c == EOF)
		return 0;

	int i = 0;
	while (isalpha(c) || c == '_') {
		word[i] = (char) c;
		c = getchar_c();
		i++;
	}

	word[i] = '\0';
	return 1;
}

struct wnode *update_tree(struct wnode *node, char *word);
struct wnode *update_tree(struct wnode *node, char *word)
{
	struct wnode *np;
	if (node == NULL) {
		np = (struct wnode *) malloc(sizeof(struct wnode));
		np->word = strdup(word);
		np->count = 1;
		np->left = NULL;
		np->right = NULL;
	} else {
		np = node;
		int cond = strcmp(word, node->word);
		if (cond == 0)
			node->count++;
		else if (cond > 0)
			node->right = update_tree(node->right, word);
		else
			node->left = update_tree(node->left, word);
	}
	return np;
}

void print_words(struct wnode *node, int prefix_length, char *previous_prefix, int *count);
void print_words(struct wnode *node, int prefix_length, char *previous_prefix, int *count)
{
	if (node != NULL) {
		print_words(node->left, prefix_length, previous_prefix, count);
		if (strncmp(previous_prefix, node->word, (unsigned long) prefix_length) != 0) {
			int prev_was_null = (previous_prefix[0] == '\0');
			previous_prefix[0] = '\0';
			strncat(previous_prefix, node->word, (unsigned long) prefix_length);
			if (!prev_was_null)
				printf("\n");
			if (*count == 1) {
				printf("\033[F");
				printf("\033[K");
			}
			*count = 0;
		}
		printf("%s ", node->word);
		(*count)++;
		print_words(node->right, prefix_length, previous_prefix, count);
	}
}

int collect_value(char *s, int *value);
int collect_value(char *s, int *value)
{
	int got    = 0;
	int result = 0;
	while (*s && isdigit(*s)) {
		got     = 1;
		result *= 10;
		result += *s - '0';
		s++;
	}
	if (got)
		*value = result;
	return got;
}

#define DEFAULT_PREFIX_LENGTH 6

int main(int __unused argc, char *argv[])
{
	struct wnode *root = NULL;
	char word[MAXWORD];
	int val;

	int __unused testvariable_foo;
	int __unused testvariable_bar;

	while (getword(word)) {
		root = update_tree(root, word);
		word[0] = '\0';
	}

	int prefix_length = DEFAULT_PREFIX_LENGTH;
	while (*++argv)
		if (strcmp(*argv, "-p") == 0)
			if (*++argv)
				if (collect_value(argv[0], &val))
					prefix_length = val;

	char previous_prefix[MAXWORD] = "";
	int count = 0;

	print_words(root, prefix_length, previous_prefix, &count);
	if (count == 1)
		printf("\033[F");
	printf("\n");

}

/*

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 0
DEFAULT_PREFIX_LENGTH EOF MAXWORD NULL __unused argc argv c char collect_value cond count ctype define else getchar getchar_c getword got h i if include int isalpha isdigit isspace left long main malloc node np prefix_length prev_was_null previous_prefix print_words printf result return right root s sizeof static stdio stdlib strcmp strdup string strncat strncmp struct test testvariable_bar testvariable_foo unsigned update_tree val value void while wnode word

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 1
argc argv
c char collect_value cond count ctype
getchar getchar_c getword got
i if include int isalpha isdigit isspace
left long
main malloc
node np
prefix_length prev_was_null previous_prefix print_words printf
result return right root
s sizeof static stdio stdlib strcmp strdup string strncat strncmp struct
test testvariable_bar testvariable_foo
unsigned update_tree
val value void
while wnode word

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 2
argc argv
collect_value cond count
getchar getchar_c getword
include int
isalpha isdigit isspace
main malloc
prefix_length prev_was_null previous_prefix print_words printf
result return
static stdio stdlib strcmp strdup string strncat strncmp struct
test testvariable_bar testvariable_foo
val value

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 3
argc argv
getchar getchar_c getword
prefix_length prev_was_null previous_prefix
print_words printf
stdio stdlib
strcmp strdup string strncat strncmp struct
test testvariable_bar testvariable_foo
val value

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 4
getchar getchar_c
prev_was_null previous_prefix
print_words printf
strncat strncmp
test testvariable_bar testvariable_foo

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 5
getchar getchar_c
print_words printf
strncat strncmp
testvariable_bar testvariable_foo

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 6
getchar getchar_c
testvariable_bar testvariable_foo

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 9
testvariable_bar testvariable_foo

$ clang -Weverything -Wno-padded chapter6/ex_6_02.c && cat chapter6/ex_6_02.c | ./a.out -p 15

*/
