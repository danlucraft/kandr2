/* Write a squeeze function that deletes any character from a string that 
occurs in another string */

#include <stdio.h>

void squeeze(char*, char*);
void test(char*, char*);

void squeeze(char* result, char* exclude)
{
	int i, j;
	
	for(i = j = 0; result[i] != '\0'; i++) {
		int copy = 1;
		for (int n = 0; exclude[n] != '\0'; n++)
			if (result[i] == exclude[n])
				copy = 0;
		if (copy)
			result[j++] = result[i];
	}
	result[j] = '\0';
}

void test(char* result, char* exclude)
{
	printf("squeeze(\"%s\", \"%s\"): ", result, exclude);
	squeeze(result, exclude);
	printf("%s\n", result);
}

int main()
{
	char test1[] = "abc";
	test(test1, "b");
	
	char test2[] = "abcde";
	test(test2, "bd");
	
	char test3[] = "abc";
	test(test3, "");
	
	char test4[] = "";
	test(test4, "456");
}

/*
$ clang -Wno-newline-eof -Weverything ex_2_04.c && ./a.out
squeeze("abc", "b"): ac
squeeze("abcde", "bd"): ace
squeeze("abc", ""): abc
squeeze("", "456"): 
*/