/* Write a function htoi(s) that converts a hex string to an int.

   0x23343
   0X34AAA
   45ffA3
   
*/

#include <stdio.h>

void test(char s[]);
int htoi(char s[]);

int main()
{
	test("0");
	test("11");
	test("21");

	test("a");
	test("B");
	test("1a");
	
	test("0x11");
	test("0xFF");
	test("0x100");

	// bad stuff
	test("FxFF");
	test("1z1");
}

void test(char s[])
{
	printf("%5s = %d\n", s, htoi(s));
}

int htoi(char s[])
{
	int end = 0;
	for (; s[end] != 0; end++)
		;
	end--;

	int acc = 0;
	int exponent = 1;
	
	for (; end >= 0 && !(end == 1 && (s[end] == 'x' || s[end] == 'X')); end--) {
		int val = 0;
		
		int l = s[end];
		if (l >= 48 && l <= 57)
			val = l - 48;
		else if (l >= 65 && l <= 90)
			val = l - 65 + 10;
		else if (l >= 97 && l <= 102)
			val = l - 97 + 10;
		
		acc += exponent * val;
		exponent *= 16;
	}
	return acc;
}

/*

Fun enough. My implementation just ignores anything it doesn't understand, 
whereas it should probably barf in some way for things like FxFF or 123HIHq123.
At this point I don't know what a good way to handle such errors in C is.

*/
