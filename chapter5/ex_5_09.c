/*

Rewrite the routines day_of_year and month_day with pointers instead of indexing.

*/

#include <stdio.h>

#define GREGORIAN_START_YEAR 1583

static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

int day_of_year(int year, int month, int day);
int day_of_year(int year, int month, int day)
{
	int i, leap;

	// &&'s precedence is tighter than ||, ballsy
	leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;

	char *month_days = daytab[leap];

	if (year < GREGORIAN_START_YEAR)
		return -1;
	if (month < 1 || month > 12)
		return -1;
	if (day < 1 || day > *(month_days + month))
		return -1;

	month_days++;
	for (i = 1; i < month; month_days++, i++)
		day += *month_days;
	return day;
}

int month_day(int year, int yearday, int *pmonth, int *pday);
int month_day(int year, int yearday, int *pmonth, int *pday)
{
	int i, leap;

	leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;

	if (year < GREGORIAN_START_YEAR)
		return -1;
	if (yearday < 1 || (!leap && yearday > 365) || (leap && yearday > 366))
		return -1;

	char *month_days = daytab[leap];

	for (i = 1; yearday > *(month_days + i); i++)
		yearday -= *(month_days + i);

	*pmonth = i;
	*pday = yearday;

	return 1;
}

void test_month_day(int year, int yearday);
void test_month_day(int year, int yearday)
{
	printf("generating month and day %5d %5d ... ", year, yearday);
	int month, day;
	int status = month_day(year, yearday, &month, &day);
	if (status == -1) {
		printf("error\n");
	} else {
		printf("(month=%2d, day=%2d)\n", month, day);
	}
}

void test_day_of_year(int year, int month, int day);
void test_day_of_year(int year, int month, int day)
{
	printf("generating yearday %5d %5d %5d ... ", year, month, day);
	int yearday = day_of_year(year, month, day);
	if (yearday == -1) {
		printf("error\n");
	} else {
		printf("%d\n", yearday);
	}
}

int main()
{
	test_month_day(2015, 1);
	test_month_day(2015, 5);
	test_month_day(2015, 100);
	test_month_day(2015, 31);
	test_month_day(2015, 365);

	// leap year
	test_month_day(2016, 5);
	test_month_day(2016, 60);
	test_month_day(2016, 100);
	test_month_day(2016, 366);

	// error checking
	test_month_day(2015, 0);
	test_month_day(2015, -1);
	test_month_day(1000, 5);
	test_month_day(2015, 366);

	test_day_of_year(2015, 1, 1);
	test_day_of_year(2015, 2, 1);
	test_day_of_year(2015, 3, 1);
	test_day_of_year(2015, 12, 31);

	// leap year
	test_day_of_year(2016, 2, 29);
	test_day_of_year(2016, 12, 31);

	// error checking
	test_day_of_year(1000, 1, 1);
	test_day_of_year(2015, 0, 1);
	test_day_of_year(2015, 13, 1);
	test_day_of_year(2015, -1, 1);
	test_day_of_year(2015, 1, -1);
	test_day_of_year(2015, 13, 1);
	test_day_of_year(2015, 1, 32);

	// ... correct number of days in particular month
	test_day_of_year(2015, 2, 30);
	test_day_of_year(2015, 2, 29);
	test_day_of_year(2016, 2, 30);
}
