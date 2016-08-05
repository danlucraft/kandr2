/*

Write a private version of scanf analogous to minprintf from the previous
section.

---

I'm doing sscanf because it's easier to test (should have done that in the
previous exercise too...)

*/

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char *consume_value(char *s, int *found, int *value);
char *consume_value(char *s, int *found, int *value)
{
	int got    = 0;
	int result = 0;
	while (*s && isdigit(*s)) {
		got     = 1;
		result *= 10;
		result += *s - '0';
		s++;
	}
	if (got)
		*value = result;
	*found = got;
	return s;
}

int minsscanf(char *string, char *format, ...);
int minsscanf(char *string, char *format, ...)
{
	va_list ap;
	char *s = string;
	char *f = format;
	int *ival;
	float *fval;
	char *sval;
	int found;
	int matched = 0;

	va_start(ap, format);
	while (*f != '\0' && *s != '\0') {
		if (*f == '%') {
			f++;
			switch (*f) {
				case 'd':
					ival = va_arg(ap, int *);
					found = 0;
					s = consume_value(s, &found, ival);
					if (!found)
						return 0;
					matched++;
					break;
				case 's':
					sval = va_arg(ap, char *);
					while (!isspace(*s))
						*sval++ = *s++;
					matched++;
					break;
				case 'f':
					fval = va_arg(ap, float *);
					char *tmp = s;
					*fval = strtof(s, &tmp);
					matched++;
					s = tmp;
					break;
				default:
					break;
			}
			f++;
		} else {
			if (*f == ' ') {
				while (isspace(*s))
					s++;
				f++;
			} else {
				if (*f == *s) {
					f++;
					s++;
				} else {
					return matched;
				}
			}
		}
	}
	va_end(ap);

	return matched;
}

int main()
{
	int ival, ival2;
	int result;
	char *s, *f;
	char sval[] = "                 ";
	float fval, fval2;
	sval[0] = '\0';
	
	s = "x 99 x"; f = "x %d x";
	ival = 0;
	result = sscanf(s, f, &ival);
	printf("\n    scanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);
	ival = 0;
	result = minsscanf(s, f, &ival);
	printf("minsscanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);

	s = "x 99 56 x"; f = "x %d %d x";
	ival = 0;
	ival2 = 0;
	result = sscanf(s, f, &ival, &ival2);
	printf("\n    scanf(\"%s\", \"%s\", %d, %d) = %d\n", s, f, ival, ival2, result);
	ival = 0;
	ival2 = 0;
	result = minsscanf(s, f, &ival, &ival2);
	printf("minsscanf(\"%s\", \"%s\", %d, %d) = %d\n", s, f, ival, ival2, result);

	s = "x a x"; f = "x %d x";
	ival = 0;
	result = sscanf(s, f, &ival);
	printf("\n    scanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);
	ival = 0;
	result = minsscanf(s, f, &ival);
	printf("minsscanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);

	s = " x 12 x"; f = "x %d x";
	ival = 0;
	result = sscanf(s, f, &ival);
	printf("\n    scanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);
	ival = 0;
	result = minsscanf(s, f, &ival);
	printf("minsscanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);

	s = "x 12 x "; f = "x %d x";
	ival = 0;
	result = sscanf(s, f, &ival);
	printf("\n    scanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);
	ival = 0;
	result = minsscanf(s, f, &ival);
	printf("minsscanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);

	s = "  x    12 \t x   "; f = " x %d x";
	ival = 0;
	result = sscanf(s, f, &ival);
	printf("\n    scanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);
	ival = 0;
	result = minsscanf(s, f, &ival);
	printf("minsscanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);

	s = "x 12 x"; f = "x %d x     ";
	ival = 0;
	result = sscanf(s, f, &ival);
	printf("\n    scanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);
	ival = 0;
	result = minsscanf(s, f, &ival);
	printf("minsscanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);

	s = "x 12 x"; f = "x %d xxxxx";
	ival = 0;
	result = sscanf(s, f, &ival);
	printf("\n    scanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);
	ival = 0;
	result = minsscanf(s, f, &ival);
	printf("minsscanf(\"%s\", \"%s\", %d) = %d\n", s, f, ival, result);

	s = "x hello x"; f = "x %s x";
	sval[0] = '\0';
	result = sscanf(s, f, &sval);
	printf("\n    scanf(\"%s\", \"%s\", \"%s\") = %d\n", s, f, sval, result);
	sval[0] = '\0';
	result = minsscanf(s, f, &sval);
	printf("minsscanf(\"%s\", \"%s\", \"%s\") = %d\n", s, f, sval, result);

	s = "x hi there x"; f = "x %s x";
	sval[0] = '\0';
	result = sscanf(s, f, &sval);
	printf("\n    scanf(\"%s\", \"%s\", \"%s\") = %d\n", s, f, sval, result);
	sval[0] = '\0';
	result = minsscanf(s, f, &sval);
	printf("minsscanf(\"%s\", \"%s\", \"%s\") = %d\n", s, f, sval, result);

	s = "x 1.4 0.45334543 x"; f = "x %f %f x";
	fval = 0.0;
	fval2 = 0;
	result = sscanf(s, f, &fval, &fval2);
	printf("\n    scanf(\"%s\", \"%s\", %f, %f) = %d\n", s, f, (double) fval, (double) fval2, result);
	fval = 0.0;
	fval2 = 0;
	result = minsscanf(s, f, &fval, &fval2);
	printf("minsscanf(\"%s\", \"%s\", %f, %f) = %d\n", s, f, (double) fval, (double) fval2, result);
}

