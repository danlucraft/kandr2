/* Should reverse each line of the input it gets: foo bar baz qux quux corge blah bloo*/

#include <stdio.h>

#define MAXLINE 80

void reverse(char in[], char out[]);
int get_line(char line[], int lim);

int main()
{
	char line[MAXLINE + 2];
	char revline[MAXLINE + 2];
	int len;
	while ((len = get_line(line, MAXLINE)) > 0) {
		reverse(line, revline);
		printf("%s", revline);
	}
}

void reverse(char in[], char out[])
{
	int i, j;
	
	for (i = 0; in[i] != 0; i++)
		;
		
	for (j = 0; j < i; j++)
		out[j] = in[i - j - 1];
	out[j] = 0;
}

// s: line including newline, terminated by a 0
// lim: max number of chars to put in s
// returns length (*including optional newline char*)
//   length 0 = no line
int get_line(char s[], int lim)
{
	int c;
	int i = 0;
	while((c = getchar()) != EOF) {
		if (c == '\n') {
			s[i] = '\n';
			s[i + 1] = 0;
			return i + 1;
		}
		s[i] = (char) c;
		i++;
		if (i == lim) {
			while ((c = getchar()) != EOF && c != '\n')
				i++; // consume rest of line
			return i;
		}
	}
	s[i + 1] = 0;
	return i;
}

// $ clang -Wno-newline-eof -Weverything ex_1_19.c && cat ex_1_19.c | ./a.out
// foo bar rab oof
// alb egroc xuuq xuq zab rab oof :steg ti tupni eht fo enil hcae esrever dluohS */
// 
// >h.oidts< edulcni#
// 
// 08 ENILXAM enifed#
// 
// ;)][tuo rahc ,][ni rahc(esrever diov
// ;)mil tni ,][enil rahc(enil_teg tni
// 
// )(niam tni
// {
// ;"rab oof" = ]2 + ENILXAM[ni_tset rahc	
// ;]2 + ENILXAM[tuo_tset rahc	
// ;)tuo_tset ,ni_tset(esrever	
// ;)tuo_tset ,ni_tset ,"n\s% s%"(ftnirp	
// 
// ;]2 + ENILXAM[enil rahc	
// ;]2 + ENILXAM[enilver rahc	
// ;nel tni	
// { )0 > ))ENILXAM ,enil(enil_teg = nel(( elihw	
// ;)enilver ,enil(esrever		
// ;)enilver ,"s%"(ftnirp		
// }	
// }
// 
// )][tuo rahc ,][ni rahc(esrever diov
// {
// ;0 = dne tni	
// ;i tni	
// )++i ;0 =! ]i[ni ;0 = i( rof	
// ;		
// ;i = dne	
// ;j tni	
// )++j ;dne < j ;0 = j( rof	
// ;]1 - j - dne[ni = ]j[tuo		
// ;0 = ]j[tuo	
// }
// 
// 0 a yb detanimret ,enilwen gnidulcni enil :s //
// s ni tup ot srahc fo rebmun xam :mil //
// )*rahc enilwen lanoitpo gnidulcni*( htgnel snruter //
// enil on = 0 htgnel   //
// )mil tni ,][s rahc(enil_teg tni
// {
// ;c tni	
// ;0 = i tni	
// { )FOE =! ))(rahcteg = c((elihw	
// { )'n\' == c( fi		
// ;'n\' = ]i[s			
// ;0 = ]1 + i[s			
// ;1 + i nruter			
// }		
// ;c )rahc( = ]i[s		
// ;++i		
// { )mil == i( fi		
// )'n\' =! c && FOE =! ))(rahcteg = c(( elihw			
// enil fo tser emusnoc // ;++i	
// ;i nruter			
// }		
// }	
// ;0 = ]1 + i[s	
// ;i nruter	
// }