/* Folds long lines at last blank character before 80 char line limit */
// Also: FUCK tabs (decided to treat them like any other character)

// to test:

// clang -Wno-newline-eof -Weverything ex_1_22_fold.c && cat ex_1_22_fold.c | ./a.out

#include <stdio.h>

#define MAX_LINE  80

int update_column(int col, char c);

int main()
{
	char buffer[MAX_LINE + 2];
	int i = 0;
	int column = 0;
	
	int c;
	while ((c = getchar()) != EOF) {
		if (column == MAX_LINE && c != '\n') {
			if (i < MAX_LINE)
				putchar('\n');
			column = 0;
			for (int j = 0; j < i; j++) {
				int ch = buffer[j];
				column = update_column(column, (char) ch);
				putchar(ch);
			}
			if (i == MAX_LINE)
				putchar('\n');
			if (column == MAX_LINE)
				column = 0;
			i = 0;
		}

		if (c == ' ' || c == '\n') {
			for (int j = 0; j < i; j++)
				putchar(buffer[j]);
			i = 0;
			putchar(c);
		} else {
			buffer[i++] = (char) c;
		}

		column = update_column(column, (char) c);
	}
	for (int j = 0; j < i; j++)
		putchar(buffer[j]);
}

int update_column(int col, char c)
{
	if (c == '\n')
		return 0;
	else
		return col+1;
}

/*
The ladies of Longbourn soon waited on those of Netherfield. The visit was soon returned in due form. Miss Bennet's pleasing manners grew on the goodwill of Mrs Hurst and Miss Bingley; and though the mother was found to be intolerable, and the younger sisters

returned in due form. Miss Bennet's pleasing manners grew on the goodwill of Mrs Hurst and Miss Bingley; and though the mother was found to be intolerable, and the younger sisters

Mrs Hurst and Miss Bingley; and though the mother was found to be intolerable, and

Mrs Hurst and Miss Bingley; and though the mother was found to be intolerableand

Mrs. Hurst and Miss Bingley; and though themotherwasfoundtobeintolerablebiooonfs,sdf and the younger sisters and the younger sisters and the younger sisters and the younger sisters and the younger sisters and the younger sisters

Mrs. Hurst and                   though the mother was found to be intolerable, and the younger sisters

TheladiesofLongbournsoonwaitedonthoseofNetherfield.Thevisitwassoonreturnedindueform.MissBennet'spleasingmannersgrewonthegoodwillofMrsHurstandMissBingley;andthoughthemotherwasfoundtobeintolerable,,andtheyoungersistersTheladiesofLongbournsoonwaitedonthoseofNetherfield.Thevisitwassoonreturnedindueform.MissBennet'spleasingmannersgrewonthegoodwillofMrsHurstandMissBingley.MissBennet'spleasingmannersgrewonthegoodwillofMrsHurstandMissBingleyMissBennet'spleasingmannersgrewonthegoodwillofMrsHurstandMissBingleyMissBennet'spleasingmannersgrewonthegoodwillofMrsHurstandMissBingley

TheladiesofLongbournsoonwaitedonthoseofNetherfield.Thevisitwassoonreturnedindueform.*/