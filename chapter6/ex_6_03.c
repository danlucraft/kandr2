/*

Write a cross-referencer that prints a list of all words in a document, and, for
each word, a list of the line numbers on which it occurs. Remove noise words
like "the", "and", and so on.

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

struct wnode {
	char *word;
	int line;
	struct wnode *left;
	struct wnode *right;
};

static char *stop_words[] = {
	"a",
	"about",
	"above",
	"after",
	"again",
	"against",
	"all",
	"am",
	"an",
	"and",
	"any",
	"are",
	"aren't",
	"as",
	"at",
	"be",
	"because",
	"been",
	"before",
	"being",
	"below",
	"between",
	"both",
	"but",
	"by",
	"can't",
	"cannot",
	"could",
	"couldn't",
	"did",
	"didn't",
	"do",
	"does",
	"doesn't",
	"doing",
	"don't",
	"down",
	"during",
	"each",
	"few",
	"for",
	"from",
	"further",
	"had",
	"hadn't",
	"has",
	"hasn't",
	"have",
	"haven't",
	"having",
	"he",
	"he'd",
	"he'll",
	"he's",
	"her",
	"here",
	"here's",
	"hers",
	"herself",
	"him",
	"himself",
	"his",
	"how",
	"how's",
	"i",
	"i'd",
	"i'll",
	"i'm",
	"i've",
	"if",
	"in",
	"into",
	"is",
	"isn't",
	"it",
	"it's",
	"its",
	"itself",
	"let's",
	"me",
	"more",
	"most",
	"mustn't",
	"my",
	"myself",
	"no",
	"nor",
	"not",
	"of",
	"off",
	"on",
	"once",
	"only",
	"or",
	"other",
	"ought",
	"our",
	"ours",
	"ourselves",
	"out",
	"over",
	"own",
	"same",
	"shan't",
	"she",
	"she'd",
	"she'll",
	"she's",
	"should",
	"shouldn't",
	"so",
	"some",
	"such",
	"than",
	"that",
	"that's",
	"the",
	"their",
	"theirs",
	"them",
	"themselves",
	"then",
	"there",
	"there's",
	"these",
	"they",
	"they'd",
	"they'll",
	"they're",
	"they've",
	"this",
	"those",
	"through",
	"to",
	"too",
	"under",
	"until",
	"up",
	"very",
	"was",
	"wasn't",
	"we",
	"we'd",
	"we'll",
	"we're",
	"we've",
	"were",
	"weren't",
	"what",
	"what's",
	"when",
	"when's",
	"where",
	"where's",
	"which",
	"while",
	"who",
	"who's",
	"whom",
	"why",
	"why's",
	"with",
	"won't",
	"would",
	"wouldn't",
	"you",
	"you'd",
	"you'll",
	"you're",
	"you've",
	"your",
	"yours",
	"yourself",
	"yourselves"
};

int get_word_and_line(char *word, int *line);
int get_word_and_line(char *word, int *line)
{
	int c;
	while (isspace(c = getchar()))
		if (c == '\n')
			(*line)++;

	while (!(isalpha(c) || c == '\''|| c == '_' || c == EOF)) {
		c = getchar();
		if (c == '\n')
			(*line)++;
	}

	if (c == EOF)
		return 0;

	int i = 0;
	while (isalpha(c) || c == '_' || c == '\'') {
		word[i] = (char) c;
		c = getchar();
		i++;
	}

	word[i] = '\0';

	if (word[0] == '\'' && word[i - 1] == '\'') {
		char temp[MAXWORD] = "";
		strcat(temp, word + 1);
		word[0] = '\0';
		strcat(word, temp);
		word[i - 2] = '\0';
	}

	return 1;
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

int binsearchi(char *words[], int low, int high, char *word);
int binsearchi(char *words[], int low, int high, char *word)
{
	int mid;
	if (high < low)
		return -1;
	mid = low + (high - low) / 2;
	int cond = strcmpi(word, words[mid]);
	if (cond == 0)
		return mid;
	else if (cond > 0)
		return binsearchi(words, mid + 1, high, word);
	else
		return binsearchi(words, low, mid - 1, word);
}

int is_stop_word(char *word);
int is_stop_word(char *word)
{
	if (strlen(word) == 1)
		return 1;
	if (binsearchi(stop_words, 0, sizeof(stop_words) / sizeof(stop_words[0]) - 1, word) != -1)
		return 1;
	return 0;
}

int numcmp(int a, int b);
int numcmp(int a, int b)
{
	if (a == b)
		return 0;
	else if (a > b)
		return 1;
	else
		return -1;
}

struct wnode *update_tree(struct wnode *node, char *word, int line);
struct wnode *update_tree(struct wnode *node, char *word, int line)
{
	struct wnode *np;
	if (node == NULL) {
		np = (struct wnode *) malloc(sizeof(struct wnode));
		np->word = strdup(word);
		np->line = line;
		np->left = NULL;
		np->right = NULL;
	} else {
		np = node;
		int cond = strcmpi(word, np->word);
		if (cond == 0)
			cond = numcmp(line, np->line);
		if (cond > 0)
			np->right = update_tree(node->right, word, line);
		else if (cond < 0)
			np->left = update_tree(node->left, word, line);
	}
	return np;
}

void print_tree(struct wnode *node, char *word, int *first_line, int *first_in_row);
void print_tree(struct wnode *node, char *word, int *first_line, int *first_in_row)
{

	if (node != NULL) {
		print_tree(node->left, word, first_line, first_in_row);

		if (strcmpi(word, node->word) != 0) {
			if (!*first_line)
				printf("\n");
			printf("%s: ", node->word);
			word[0] = '\0';
			strcat(word, node->word);
			*first_in_row = 1;
		}
		*first_line = 0;
		if (!*first_in_row)
			printf(", ");
		*first_in_row = 0;
		printf("%d", node->line);

		print_tree(node->right, word, first_line, first_in_row);
	}
}

int main()
{
	char word[MAXWORD];
	int line = 1;
	struct wnode *root = NULL;

	while(get_word_and_line(word, &line))
		if (!is_stop_word(word))
			root = update_tree(root, word, line);

	int first_line = 1;
	int first_in_row = 1;
	print_tree(root, word, &first_line, &first_in_row);
	printf("\n");
}

/*

$ clang -Weverything -Wno-padded chapter6/ex_6_03.c && cat chapter6/ex_6_03.test | ./a.out | head -n 20
'fee: 79
abandon: 109, 111, 119
abbeys: 97, 111
abbot: 97
abbots: 1, 33
abolished: 101
abovementioned: 149
abroad: 41, 131
absence: 41, 131
accepted: 77
accordance: 9, 33, 87, 89
according: 119
ACCORDINGLY: 147
achievement: 133
actually: 93
addition: 143, 145
administration: 43
advice: 3
afforested: 111
afterwards: 43
...

*/
