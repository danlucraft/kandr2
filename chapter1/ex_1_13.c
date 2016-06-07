/* Print a histogram of lengths of words in the input */

#include <stdio.h>
#define MAX_LEN 	12

int main()
{
	int lens[MAX_LEN];
	for (int i = 0; i < MAX_LEN; i++)
		lens[i] = 0;

	int inword = 0;
	int len = 0;
	int c;
	while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t' || c == '\n') {
			if (inword) {
				if (len > MAX_LEN)
					len = MAX_LEN;
				lens[len - 1]++;
			}
			inword = 0;
			len = 0;
		} else {
			inword = 1;
			len++;
		}
	}

	printf("\n");
	for (int i = 0; i < MAX_LEN; i++) {
		printf("%3d ", i + 1);
		for (int j = 0; j < lens[i]; j++)
			printf("#");
		printf("\n");
	}

	printf("\n");
	int highest = 0;
	for (int i = 0; i < MAX_LEN; i++)
		if (lens[i] > highest)
			highest = lens[i];

	int divisor = 1;
	while (highest >= 30) {
		highest /= 2;
		divisor *= 2;
	}

	for (int row = highest; row >= 0; row--) {
		printf(" ");
		for (int j = 0; j < MAX_LEN; j++)
			if ((lens[j] / divisor) >= row)
				printf("###");
			else
				printf("   ");

		printf("\n");
	}

	for (int i = 0; i < MAX_LEN; i++)
		printf("%3d", i + 1);
	printf("\n");
}

// $ clang ex_1_13.c && cat ex_1_13.c | ./a.out
//
//   1 #################################################################
//   2 ##########################################
//   3 ##########################
//   4 ###################
//   5 #######
//   6 ######
//   7 #########
//   8 ###################
//   9 ###
//  10 #
//  11 #
//  12 #########
//
//  ###
//  ###
//  ###
//  ###
//  ###
//  ###
//  ######
//  ######
//  ######
//  ######
//  #########
//  #########
//  ############         ###
//  ############         ###
//  ############      ######         ###
//  ########################         ###
//  ####################################
//   1  2  3  4  5  6  7  8  9 10 11 12
