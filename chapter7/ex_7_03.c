/*

Revise minprintf to handle more of the other facilities of printf.

---

I got quite far with this but ran out of steam handling float rounding by
precision, as you can see with the tests.

There are also a couple of places where there is a max size of something that
is not well handled ("malloc(100 * sizeof(char))") and bigger values will cause
segfaults.

*/

#include <stdarg.h> 
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum width { DEFAULT = -1, SHORT = -2, LONG = -3 };

struct convspec {
	int left_align;
	int min_field_width;
	int precision;
	int width;
	char type;
};

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

void zero_convspec(struct convspec *spec);
void zero_convspec(struct convspec *spec)
{
	spec->type = '%';
	spec->left_align = 0;
	spec->width = DEFAULT;
	spec->precision = -1;
	spec->min_field_width = -1;
}

int is_base_type(char c);
int is_base_type(char c)
{
	return (c == 'i' ||
			c == 'd' ||
			c == 'o' ||
			c == 'x' ||
			c == 'X' ||
			c == 'u' ||
			c == 'c' ||
			c == 's' ||
			c == 'f' ||
			c == 'e' ||
			c == 'E' ||
			c == 'g' ||
			c == 'G' ||
			c == 'p');
}

int is_width_type(char c);
int is_width_type(char c)
{
	return (c == 'h' || c == 'l');
}

int consume_spec(char **s, struct convspec *spec);
int consume_spec(char **s, struct convspec *spec)
{
	char *p = *s;

	if (*p++ != '%')
		return 0;

	if (*p == '%') {
		spec->type = '%';
		*s = p;
		return 1;
	}

	if (*p == '-') {
		spec->left_align = 1;
		p++;
	}

	int found1 = 0;
	int found2 = 0;
	int value1, value2;

	p = consume_value(p, &found1, &value1);

	if (*p == '.') {
		if (found1)
			spec->min_field_width = value1;
		p++;
		p = consume_value(p, &found2, &value2);
		if (found2)
			spec->precision = value2;
	} else {
		if (found1)
			spec->min_field_width = value1;
	}

	if (is_width_type(*p)) {
		spec->width = *p == 'h' ? SHORT : LONG;
		p++;
	}

	if (is_base_type(*p)) {
		spec->type = *p;
		p++;
	} else {
		return 0;
	}

	*s = p;
	return 1;
}

void strreverse(char s[]);
void strreverse(char s[])
{
	int c, i, j;

	for (i = 0, j = (int) strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = (char) c;
	}
}

char *pad(char *s, char pad, int minlen, int left_align);
char *pad(char *s, char pad, int minlen, int left_align)
{
	if (minlen == -1)
		return strdup(s);

	int len = (int) strlen(s);
	if (len >= minlen)
		return strdup(s);

	char *result = malloc((unsigned long) minlen * sizeof(char) + 1);
	result[0] = '\0';
	strcat(result, s);

	if (!left_align)
		strreverse(result);

	while (minlen > len)
		result[len++] = pad;
	result[len] = '\0';

	if (!left_align)
		strreverse(result);

	return result;
}

long mylog(double x, double b);
long mylog(double x, double b)
{
	double r = floor(log(x) / log(b));
	return lround(r);
}

static char digits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

char *myitoa(int n, int precision, int base);
char *myitoa(int n, int precision, int base)
{
	int i, sign;
	long ndigits = mylog(abs(n), base) + 1;

	sign = n;
	char *s = malloc((unsigned long) ndigits * sizeof(char) + 1 + (sign < 0 ? 1 : 0));
	s[0] = '\0';

	i = 0;
	do {
		s[i++] = digits[abs(n % base)];
	} while (abs(n /= base) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	strreverse(s);

	if (precision != -1 && strlen(s) < (unsigned long) precision)
		return pad(s, '0', precision, 0);
	else
		return s;
}

char *mydtoa(double d, int precision);
char *mydtoa(double d, int precision)
{
	if (precision == -1)
		precision = 6;

	double sign = d;
	d = fabs(d);
	char *s = malloc(100 * sizeof(char));
	s[0] = '\0';

	// integer part
	int i = 0;
	long n = lround(floor(d));
	do {
		s[i++] = digits[labs(n % 10)];
	} while (labs(n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	strreverse(s);

	// decimal part
	strcat(s, ".");
	double dec = (d - floor(d)) * 10;
	long v;
	unsigned long j = strlen(s);
	do {
		v = lround(floor(dec));
		s[j++] = digits[v];
		s[j] = '\0';
		dec -= v;
		dec *= 10;
		precision--;
	} while (fabs(d) > 0 && precision > 0);

	return s;
}

void minprintf(char *fmt, ...);
void minprintf(char *fmt, ...)
{
	va_list ap;
	char *p;
	int ival;
	double dval;
	char cval;

	va_start(ap, fmt);

	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}

		struct convspec spec;
		zero_convspec(&spec);

		if (consume_spec(&p, &spec)) {
			char *work = NULL;
			char *arg;
			switch (spec.type) {
				case '%':
					printf("%%");
					break;
				case 'i':
				case 'd':
				case 'u':
					ival = va_arg(ap, int);
					work = myitoa(ival, spec.precision, 10);
					break;
				case 'o':
					ival = va_arg(ap, int);
					work = myitoa(ival, spec.precision, 8);
					break;
				case 'x':
					ival = va_arg(ap, int);
					work = myitoa(ival, spec.precision, 16);
					break;
				case 'X':
					ival = va_arg(ap, int);
					work = myitoa(ival, spec.precision, 16);
					char *c = work;
					while (*c) {
						*c = (char) toupper(*c);
						c++;
					}
					break;
				case 'f':
					dval = va_arg(ap, double);
					work = mydtoa(dval, spec.precision);
					break;
				case 's':
					arg = va_arg(ap, char *);
					work = strdup(arg);
					if ((unsigned long) spec.precision < strlen(work))
						work[spec.precision] = '\0';
					break;
				case 'c':
					cval = (char) va_arg(ap, int);
					putchar(cval);
					break;
				default:
					putchar(*p);
					break;
			}

			if (work != NULL) {
				char *result = pad(work, ' ', spec.min_field_width, spec.left_align);
				char *c = result;
				while (*c)
					putchar(*c++);
				free(result);
				free(work);
			}

			p--;
		}
	}
	va_end(ap);
}

char *print_spec(struct convspec spec);
char *print_spec(struct convspec spec)
{
	char *result = malloc(50 * sizeof(char));
	char *work;
	result[0] = '\0';
	strcat(result, "%");

	if (spec.type == '%') {
		strcat(result, "%");
		return result;
	}

	if (spec.left_align)
		strcat(result, "-");

	if (spec.min_field_width != -1) {
		work = myitoa(spec.min_field_width, -1, 10);
		strcat(result, work);
		free(work);
	}

	if (spec.precision != -1) {
		strcat(result, ".");
		work = myitoa(spec.precision, -1, 10);
		strcat(result, work);
		free(work);
	}

	if (spec.width == SHORT)
		strcat(result, "h");
	if (spec.width == LONG)
		strcat(result, "l");

	char type_s[] = " ";
	type_s[0] = spec.type;
	strcat(result, type_s);

	return result;
}
void test_consume_spec(char *s);
void test_consume_spec(char *s)
{
	char *p = s;
	struct convspec spec;
	zero_convspec(&spec);
	if (consume_spec(&p, &spec))
		printf("  consume_spec(\"%s\") = \"%s\" (\"%s\")\n", s, print_spec(spec), p);
	else
		printf("  consume_spec(\"%s\") found no spec (\"%s\")\n", s, p);
}

int main()
{
	printf("Test spec reading:\n\n");
	test_consume_spec("%%,");
	test_consume_spec("%d,");
	test_consume_spec("%-d,");
	test_consume_spec("%-hd,");
	test_consume_spec("%10ld,");
	test_consume_spec("%-10.34ld,");
	test_consume_spec("%-.34ld,");
	printf("\n");
	test_consume_spec("%s,");
	test_consume_spec("%10s,");
	test_consume_spec("%.10s,");
	test_consume_spec("%-10s,");
	test_consume_spec("%15.10s,");
	test_consume_spec("%-15.10s,");
	printf("\n");
	test_consume_spec("%y,");
	test_consume_spec("foo");

	printf("\nTest printf vs minprintf\n\n");

	printf("  Insert %%\n\n");

	printf(   "    :%%:\n");
	minprintf("    :%%:\n\n");

	printf("  Decimal Ints\n\n");

	printf(   "    :%d:\n", 13);
	minprintf("    :%d:\n\n", 13);

	printf(   "    :%i:\n", 13);
	minprintf("    :%i:\n\n", 13);

	printf(   "    :%4d:\n", 13);
	minprintf("    :%4d:\n\n", 13);

	printf(   "    :%-4d:\n", 13);
	minprintf("    :%-4d:\n\n", 13);

	printf(   "    :%.4d:\n", 132);
	minprintf("    :%.4d:\n\n", 132);

	printf(   "    :%9.6d:\n", 132);
	minprintf("    :%9.6d:\n\n", 132);

	printf(   "    :%-9.6d:\n", 132);
	minprintf("    :%-9.6d:\n\n", 132);

	printf(   "    :%.2d:\n", 13255);
	minprintf("    :%.2d:\n\n", 13255);

	printf(   "    :%d:\n", -113);
	minprintf("    :%d:\n\n", -113);

	printf("  Unsigned Octal Ints\n\n");

	printf(   "    :%o:\n", 10);
	minprintf("    :%o:\n\n", 10);

	printf(   "    :%o:\n", 16000);
	minprintf("    :%o:\n\n", 16000);

	printf(   "    :%3o:\n", 10);
	minprintf("    :%3o:\n\n", 10);

	printf(   "    :%6.4o:\n", 10);
	minprintf("    :%6.4o:\n\n", 10);

	printf(   "    undefined behaviour?\n");
	printf(   "    :%o:\n", -10);
	minprintf("    :%o:\n\n", -10);

	printf("  Unsigned Hexadecimal Ints\n\n");

	printf(   "    :%x:\n", 1239);
	minprintf("    :%x:\n\n", 1239);

	printf(   "    :%X:\n", 1239);
	minprintf("    :%X:\n\n", 1239);

	printf(   "    undefined behaviour?\n");
	printf(   "    :%x:\n", -1239);
	minprintf("    :%x:\n\n", -1239);

	printf("  Unsigned Decimal Ints\n\n");

	printf(   "    :%u:\n", 10);
	minprintf("    :%u:\n\n", 10);

	printf(   "    :%u:\n", 16000);
	minprintf("    :%u:\n\n", 16000);

	printf(   "    :%3u:\n", 10);
	minprintf("    :%3u:\n\n", 10);

	printf(   "    :%6.4u:\n", 10);
	minprintf("    :%6.4u:\n\n", 10);

	printf(   "    undefined behaviour?\n");
	printf(   "    :%u:\n", -10);
	minprintf("    :%u:\n\n", -10);

	printf("  Chars\n\n");

	printf(   "    :%c:\n", 'x');
	minprintf("    :%c:\n\n", 'x');

	printf("  Strings\n\n");

	printf(   "    :%s:\n", "foobar");
	minprintf("    :%s:\n\n", "foobar");

	printf(   "    :%10s:\n", "foobar");
	minprintf("    :%10s:\n\n", "foobar");

	printf(   "    :%-10s:\n", "foobar");
	minprintf("    :%-10s:\n\n", "foobar");

	printf(   "    :%.3s:\n", "foobar");
	minprintf("    :%.3s:\n\n", "foobar");

	printf(   "    :%.9s:\n", "foobar");
	minprintf("    :%.9s:\n\n", "foobar");

	printf(   "    :%10.3s:\n", "foobar");
	minprintf("    :%10.3s:\n\n", "foobar");

	printf(   "    :%-10.3s:\n", "foobar");
	minprintf("    :%-10.3s:\n\n", "foobar");

	printf("  Double\n\n");

	printf(   "    :%f:\n", 1.567);
	minprintf("    :%f:\n\n", 1.567);

	printf(   "    :%f:\n", -1.567);
	minprintf("    :%f:\n\n", -1.567);

	printf(   "    :%.80f:\n", 1.567);
	minprintf("    :%.80f:\n\n", 1.567);

	printf(   "    :%.2f:\n", 1.567);
	minprintf("    :%.2f:\n\n", 1.567);

	printf("  Multiple stuff\n\n");

	printf(   "    a=%d b=%f c=%s\n", 123, 99.9, "hi");
	minprintf("    a=%d b=%f c=%s\n", 123, 99.9, "hi");

}

/*

$ clang -g -fsanitize=address -Weverything -Wno-padded chapter7/ex_7_03.c && ./a.out
Test spec reading:

  consume_spec("%%,") = "%%" ("%,")
  consume_spec("%d,") = "%d" (",")
  consume_spec("%-d,") = "%-d" (",")
  consume_spec("%-hd,") = "%-hd" (",")
  consume_spec("%10ld,") = "%10ld" (",")
  consume_spec("%-10.34ld,") = "%-10.34ld" (",")
  consume_spec("%-.34ld,") = "%-.34ld" (",")

  consume_spec("%s,") = "%s" (",")
  consume_spec("%10s,") = "%10s" (",")
  consume_spec("%.10s,") = "%.10s" (",")
  consume_spec("%-10s,") = "%-10s" (",")
  consume_spec("%15.10s,") = "%15.10s" (",")
  consume_spec("%-15.10s,") = "%-15.10s" (",")

  consume_spec("%y,") found no spec ("%y,")
  consume_spec("foo") found no spec ("foo")

Test printf vs minprintf

  Insert %

    :%:
    :%:

  Decimal Ints

    :13:
    :13:

    :13:
    :13:

    :  13:
    :  13:

    :13  :
    :13  :

    :0132:
    :0132:

    :   000132:
    :   000132:

    :000132   :
    :000132   :

    :13255:
    :13255:

    :-113:
    :-113:

  Unsigned Octal Ints

    :12:
    :12:

    :37200:
    :37200:

    : 12:
    : 12:

    :  0012:
    :  0012:

    undefined behaviour?
    :37777777766:
    :-12:

  Unsigned Hexadecimal Ints

    :4d7:
    :4d7:

    :4D7:
    :4D7:

    undefined behaviour?
    :fffffb29:
    :-4d7:

  Unsigned Decimal Ints

    :10:
    :10:

    :16000:
    :16000:

    : 10:
    : 10:

    :  0010:
    :  0010:

    undefined behaviour?
    :4294967286:
    :-10:

  Chars

    :x:
    :x:

  Strings

    :foobar:
    :foobar:

    :    foobar:
    :    foobar:

    :foobar    :
    :foobar    :

    :foo:
    :foo:

    :foobar:
    :foobar:

    :       foo:
    :       foo:

    :foo       :
    :foo       :

  Double

    :1.567000:
    :1.566999:

    :-1.567000:
    :-1.566999:

    :1.56699999999999994848565165739273652434349060058593750000000000000000000000000000:
    :1.56699999999999999289457264239899814128875732421875000000000000000000000000000000:

    :1.57:
    :1.56:
 
  Multiple stuff

	a=123 b=99.900000 c=hi
	a=123 b=99.900000 c=hi
*/
