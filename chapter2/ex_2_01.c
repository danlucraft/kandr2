/* Print ranges for some types */
// Range of floats is discussed here https://en.wikipedia.org/wiki/IEEE_754-1985#Range_and_precision
#include <stdio.h>
#include <limits.h>
#include <float.h>

unsigned char uchar_max(void);
char char_max(void);
char char_min(void);

unsigned short ushort_max(void);
short short_max(void);
short short_min(void);

unsigned int uint_max(void);
int int_max(void);
int int_min(void);

unsigned long ulong_max(void);
long long_max(void);
long long_min(void);

unsigned long ullong_max(void);
long llong_max(void);
long llong_min(void);

int main()
{
	printf("INTEGER TYPES (computed)\n");
	printf("                                signed                                 (unsigned max)\n");
	printf("char      %22d to %22d      (%22d)\n",       char_min(),  char_max(),  uchar_max());
	printf("short     %22d to %22d      (%22d) \n",      short_min(),  short_max(),  ushort_max());
	printf("int       %22d to %22d      (%22u) \n",      int_min(),   int_max(),   uint_max());
	printf("long      %22li to %22li      (%22lu) \n",   long_min(),  long_max(),  ulong_max());
	printf("long long %22li to %22li      (%22lu) \n",   llong_min(), llong_max(), ullong_max());
	
	printf("\nINTEGER TYPES (from headers)\n");
	printf("                                signed                                (unsigned max)\n");
	printf("char      %22d to %22d      (%22d)\n",       CHAR_MIN,  CHAR_MAX,  UCHAR_MAX);
	printf("short     %22d to %22d      (%22d) \n",      SHRT_MIN,  SHRT_MAX,  USHRT_MAX);
	printf("int       %22d to %22d      (%22u) \n",      INT_MIN,   INT_MAX,   UINT_MAX);
	printf("long      %22li to %22li      (%22lu) \n",   LONG_MIN,  LONG_MAX,  ULONG_MAX);
	printf("long long %22lli to %22lli      (%22llu)\n", LLONG_MIN, LLONG_MAX, ULLONG_MAX);

	printf("\nFLOATING POINT TYPES (headers)\n");
	printf("float       %22f to %22f\n",       FLT_MIN,  FLT_MAX);
	// printf for double is %f too
	printf("double      %22f to %22f\n",       DBL_MIN,  DBL_MAX);
	printf("long double %22Lf to %22Lf\n",       LDBL_MIN,  LDBL_MAX);
	
}

unsigned char uchar_max()
{
	unsigned char i = 0;
	while ((unsigned char) (i + 1) > (unsigned char) i)   i += 1;
	return i;
}

char char_max()
{
	char i = 0;
	while ((char) (i + 1) > (char) i)   i += 1;
	return i;
}

char char_min()
{
	char i = 0;
	while ((char) (i - 1) < (char) i)   i -= 1;
	return i;
}

// if you do i - 1000 where i is a short it will convert to ints
unsigned short ushort_max()
{
	unsigned short i = 0;
	while ((unsigned short) (i + 100) > (unsigned short) i) i += 100;	
	while ((unsigned short) (i + 1) > (unsigned short) i)   i += 1;
	return i;
}

short short_max()
{
	short i = 0;
	while ((short) (i + 100) > (short) i) i += 100;	
	while ((short) (i + 1) > (short) i)   i += 1;
	return i;
}

short short_min()
{
	short i = 0;
	while ((short) (i - 100) < (short) i) i -= 100;	
	while ((short) (i - 1) < (short) i)   i -= 1;
	return i;
}

unsigned int uint_max()
{
	unsigned int i = 0;
	while (i + 10000 > i) i += 10000;
	while (i + 1 > i) i++;
	return i;
}

int int_max()
{
	int i = 0;
	while (i + 10000 > i) i += 10000;
	while (i + 1 > i) i++;
	return i;
}

int int_min()
{
	int i = 0;
	while (i - 10000 < i) i -= 10000;
	while (i - 1 < i) i--;
	return i;
}

unsigned long ulong_max()
{
	unsigned long i = 0;
	while (i + 1000000000000 > i) i += 1000000000000;
	while (i + 1000000 > i) i += 1000000;
	while (i + 1 > i) i += 1;
	return i;
}

long long_max()
{
	long i = 0;
	while (i + 1000000000000 > i) i += 1000000000000;
	while (i + 1000000 > i) i += 1000000;
	while (i + 1 > i) i += 1;
	return i;
}

long long_min()
{
	long i = 0;
	while (i - 1000000000000 < i) i -= 1000000000000;
	while (i - 1000000 < i) i -= 1000000;
	while (i - 1 < i) i -= 1;
	return i;
}

unsigned long ullong_max()
{
	unsigned long long i = 0;
	while (i + 1000000000000 > i) i += 1000000000000;
	while (i + 1000000 > i) i += 1000000;
	while (i + 1 > i) i += 1;
	return i;
}

long llong_max()
{
	long long i = 0;
	while (i + 1000000000000 > i) i += 1000000000000;
	while (i + 1000000 > i) i += 1000000;
	while (i + 1 > i) i += 1;
	return i;
}

long llong_min()
{
	long long i = 0;
	while (i - 1000000000000 < i) i -= 1000000000000;
	while (i - 1000000 < i) i -= 1000000;
	while (i - 1 < i) i -= 1;
	return i;
}