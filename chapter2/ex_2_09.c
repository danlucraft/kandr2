// vim: noai:ts=4:sw=4
//
/*

In a two's complement number system, x &= ( x - 1 ) deletes the rightmost 
1-bit in x. Explain why. Use this observation to write a faster version of
bitcount.

*/


/*

I don't really get this. If it's unsigned then two's complement is identical
to ones complement and signed magnitude representations. So why bother 
mentioning it??

Anyway, why does x &= (x - 1) delete the rightmost bit. Answer:

Assume that the rightmost non-zero bit is at column n:

            n
            |
            v
   .........1000000

If you subtract 1 from x, you get this (where all the dotted portions are not
changed):

   .........0111111

Now if you AND that with x, all the dotted portion will have the same bit
values so won't change from x, and all the other bits are opposite so will 
become 0.

What you're left with is the dotted portion is unchanged, but the bit in
column n (the rightmost non-zero bit) is switched from 1 to 0.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_bits(unsigned int x);
void print_bits(unsigned int x)
{
	char s[] = "00000000000000000000000000000000";
	for (int i = 0; i < 32; i++) {
		if ((x & 1) != 0)
			s[31 - i] = '1';
		else
			s[31 - i] = '0';
	    x = x >> 1;
	}
	printf("%s", s);
}

int bitcount(unsigned int x);
int bitcount(unsigned int x)
{
    int b;

    for (b = 0; x != 0; x >>= 1)
        if (x & 1)
            b++;

    return b;
}
// loop runs sizeof(x)*8 times

int bitcount2(unsigned int x);
int bitcount2(unsigned int x)
{
    int b = 0;

    for (b = 0; x; b++, x &= (x - 1))
		;

    return b;
}
// loop runs sizeof(x)*8 * 0.5 times on average (once for each
// bit set to 1)

void test_bitcount(unsigned int x);
void test_bitcount(unsigned int x)
{
    printf("bitcount(");
    print_bits(x);
    printf(") = %d = ", bitcount(x));
    printf("%d\n", bitcount2(x));
}

int main(int argc, char** argv)
{
	test_bitcount(3);
	test_bitcount(15);
	int use_fast_version = 
		(argc > 1 && 
		 strcmp("--fast", *(argv + 1)) == 0);

	if (use_fast_version) {
		printf("Fast version\n");
		for (int i = 0; i < 100000000; i++)
			bitcount2((unsigned) rand());
	}
	else {
		printf("Slow version\n");
		for (int i = 0; i < 100000000; i++)
				bitcount((unsigned) rand());
	}
}

/*

// $ clang -Weverything chapter2/ex_2_09.c && time ./a.out
// Slow version
// 
// real	0m21.780s
// user	0m21.702s
// sys	0m0.029s
//
// $ clang -Weverything chapter2/ex_2_09.c && time ./a.out --fast
// Fast version
// 
// real	0m5.560s
// user	0m5.530s
// sys	0m0.010s

*/

/*

So this runs actually about 71% faster, better than the 50% improvement
in number of times it runs the loop. I guess this is because there are
fewer or faster operations performed in the loop, but that's hard to judge
without a table of times for operations.

*/


