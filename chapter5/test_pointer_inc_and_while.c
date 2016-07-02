/*

I've got some serious confusion here as to whether this:

  while (*s++)
    do_stuff;

takes s to the last character in the string, the string terminator, or to the 
position AFTER the terminator. And which of those positions the loop contents 
get run for.

There is some test code below that tests this.

Findings:

the code above is equivalent to (for a string s):

  while (*s) {
    s++;
	do_stuff;
  }
  s++;

Because *s++ is equivalent to { r = *s, s++, r }.

Therefore:

 * after the loop, s will be left pointing at the position AFTER the string
   terminator, which is quite likely to be memory you don't own.
 * the loop contents will be run for each position in the string, INCLUDING
   the terminator.

*/

#include <stdio.h>

void pointer_tests(void);
void pointer_tests()
{
	printf("** Test while loop with ++ behaviour:\n");

	char t[] = "abc";

	// Question: does this go to c or to the \0 after c?
	char *z = t;
	while (*z++)
		printf("\"%s\"\n", z);
	printf("ptr diff: %d\n", (int) (z - t));
	// Answer: it goes to the \0.

	// So it runs the loop contents with z pointing at
	// \0. So it MUST have run ++ AFTER the while (*z) took place.
	
	// So 
	//
	// while (*z++) 
	//   do_stuff; 
	//
	// is equivalent to 
	//
	// while (*z) {
	//   z++;
	//   do_stuff;
	// }
	// z++;
	//
	// ?
	// 
	// proof:
	
	z = t;
	while (*z) {
		z++;
		printf("\"%s\"\n", z);
	}
	z++;
	printf("ptr diff: %d\n", (int) (z - t));

	// Is this because *z++ is equivalent to:
	//   { r = *z; z++; r }
	// ?
	
	z = t;
	char y = *z++;
	printf("y = %c\n", y);

	// Yes.

	// So my real question was what does while (*z++); do with
	// an empty string?
	
	char *e = "";
	char *s = e;
	while (*s++)
		;
	printf("ptr diff: %d\n", (int) (s - e));

	// Diff is 1. So yes, this moves s on to one past the \0, or in other words
	// it runs ++ whether or not the while condition is true. So while (*s++) 
	// will always go to the position AFTER the \0 in a string.

	printf("** DONE\n\n");
}

int main()
{
	pointer_tests();
}

/*

$ clang -Weverything chapter5/test_pointer_inc_and_while.c && ./a.out
** Test while loop with ++ behaviour:
"bc"
"c"
""
ptr diff: 4
"bc"
"c"
""
ptr diff: 4
y = a
ptr diff: 1
** DONE

*/
