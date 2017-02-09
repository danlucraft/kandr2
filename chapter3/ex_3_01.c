// vim: noai:ts=4:sw=4
//
/*

Rewrite the binsearch function given to only have 1 test in the loop.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMTESTS 1000
#define NUMELEMENTS 1000
#define MAXSTEP 1000

/* binsearch: find x in v[O] <= v[1] <= ••• <= v[n-1] */
int binsearch(int x, int v[], int n);
int binsearch(int x, int v[], int n)
{
	int low, high, mid;
	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low+high) / 2;  // this can overflow?
		if (x < v[mid])
			high = mid - 1;
		else if (x > v[mid])
			low = mid + 1;
		else
			return mid;
	}
	return -1; /* no match */
}

int binsearch2(int x, int v[], int n);
int binsearch2(int x, int v[], int n)
{
	int low, high, mid;
	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low+high) / 2;
		if (x < v[mid])
			high = mid - 1;
		else
			low = mid + 1;
	}
	if (v[low] == x)
		return low;
	if (v[low - 1] == x)
		return low - 1;
	return -1;
}

int main()
{
	int a[NUMTESTS][NUMELEMENTS];
	clock_t time;

	for (int i = 0; i < NUMTESTS; i++) {
		int start = rand() % MAXSTEP;
		for (int j = 0; j < NUMELEMENTS; j++) {
			a[i][j] = start;
			start += rand() % MAXSTEP;
		}
	}

	time = clock();
	for (int i = 0; i < NUMTESTS * 100000; i++) {
		int ix = rand() % NUMELEMENTS;
		int j = i % NUMTESTS;
		binsearch(a[j][ix], a[j], NUMELEMENTS);
	}
	time = clock() - time;
	printf("binsearch() took %lu clocks (%0.2f seconds)\n", (unsigned long) time, (double) time/CLOCKS_PER_SEC);

	time = clock();
	for (int i = 0; i < NUMTESTS * 100000; i++) {
		int ix = rand() % NUMELEMENTS;
		int j = i % NUMTESTS;
		binsearch2(a[j][ix], a[j], NUMELEMENTS);
	}
	time = clock() - time;
	printf("binsearch2() took %lu clocks (%0.2f seconds)\n", (unsigned long) time, (double) time/CLOCKS_PER_SEC);
}

/*

$ clang -g -Wno-newline-eof -Weverything ex_3_01.c && time ./a.out
binsearch() took 2212479 clocks (2.21 seconds)
binsearch2() took 2292387 clocks (2.29 seconds)

i.e. the one with fewer tests in the loop is 20% slower. It doesn't terminate
when mid = value, instead it continues until the width of the interval is 0
which obviously adds extra cycles.

*/
