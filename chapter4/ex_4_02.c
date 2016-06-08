/*

Write double atof(char s[]) that can handle notation like:

  123.45e-6

*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

double myatof(char s[]);
double myatof(char s[])
{
	double val = 0.0;
	double power = 1.0;
	double sign = 1.0;
	
	int i = 0;

	while (isspace(s[i]))
		i++;
	if (s[i] == '-')
		sign = -1.0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i] != '\0' && s[i] != '.' && s[i] != 'e') {
		val = val*10.0 + s[i] - '0';
		i++;
	}
	if (s[i] == '.') {
		i++;
		while (s[i] != '\0' && s[i] != 'e') {
			val = val*10.0 + s[i] - '0';
			power = power * 10.0;
			i++;
		}
	}
	int exp = 0;
	if (s[i] == 'e') {
		i++;
		int negative_exp = 0;
		if (s[i] == '-')
			negative_exp = 1;
		if (s[i] == '+' || s[i] == '-')
			i++;
		while (s[i] != '\0') {
			exp = exp*10 + s[i] - '0';
			i++;
		}
		if (negative_exp)
			exp = -exp;
	}

	double result = sign * val / power;
	while (exp > 0) {
		result = result * 10.0;
		exp--;
	}
	while (exp < 0) {
		result = result / 10.0;
		exp++;
	}

	return result;
}

void test_atof(char s[]);
void test_atof(char s[])
{
	printf("myatof(\"%s\") = %f (atof:%f)\n", s, myatof(s), atof(s));
}

int main()
{
	test_atof("0");
	test_atof("2");
	test_atof("0.1");
	test_atof("2.0");
	test_atof("4.5");
	test_atof("123.456");

	test_atof("123e+5");
	test_atof("123e5");
	test_atof("123e-5");

	test_atof("123.456e+5");
	test_atof("123.456e5");
	test_atof("123.456e-5");

}

/*

$ clang -g -Weverything chapter4/ex_4_02.c && ./a.out
myatof("0") = 0.000000 (atof:0.000000)
myatof("2") = 2.000000 (atof:2.000000)
myatof("0.1") = 0.100000 (atof:0.100000)
myatof("2.0") = 2.000000 (atof:2.000000)
myatof("4.5") = 4.500000 (atof:4.500000)
myatof("123.456") = 123.456000 (atof:123.456000)
myatof("123e+5") = 12300000.000000 (atof:12300000.000000)
myatof("123e5") = 12300000.000000 (atof:12300000.000000)
myatof("123e-5") = 0.001230 (atof:0.001230)
myatof("123.456e+5") = 12345600.000000 (atof:12345600.000000)
myatof("123.456e5") = 12345600.000000 (atof:12345600.000000)
myatof("123.456e-5") = 0.001235 (atof:0.001235)

*/
