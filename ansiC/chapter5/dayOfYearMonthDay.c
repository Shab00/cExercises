#include <stdio.h>

static char daytab[2][13] = {
    { 0,31,28,31,30,31,30,31,31,30,31,30,31 },
    { 0,31,29,31,30,31,30,31,31,30,31,30,31 }
};

int day_of_year(int year, int month, int day)
{
    int i, leap;

    if (year < 1)
        return -1;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    if (month < 1 || month > 12)
        return -1;
    if (day < 1 || day > daytab[leap][month])
        return -1;

    for (i = 1; i < month; i++)
        day += daytab[leap][i];
    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
    int i, leap;

    if (year < 1) {
        *pmonth = -1;
        *pday = -1;
        return;
    }
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    if (yearday < 1 || yearday > 365 + leap) {
        *pmonth = -1;
        *pday = -1;
        return;
    }
    for (i = 1; yearday > daytab[leap][i]; i++)
        yearday -= daytab[leap][i];
    if (i > 12) {
        *pmonth = -1;
        *pday = -1;
        return;
    }
    *pmonth = i;
    *pday = yearday;
}

int main(void)
{
    int d, m, y;
    int yearday;

    y = 2024; m = 3; d = 1;
    yearday = day_of_year(y, m, d);
    printf("%d-%02d-%02d is day %d\n", y, m, d, yearday);

    y = 2020; m = 2; d = 29;
    yearday = day_of_year(y, m, d);
    printf("%d-%02d-%02d is day %d\n", y, m, d, yearday);

    y = 2021; yearday = 59;
    month_day(y, yearday, &m, &d);
    printf("Day %d of %d is %d-%02d-%02d\n", yearday, y, y, m, d);

    y = 2020; yearday = 60;
    month_day(y, yearday, &m, &d);
    printf("Day %d of %d is %d-%02d-%02d\n", yearday, y, y, m, d);

    y = 2021; m = 12; d = 31;
    yearday = day_of_year(y, m, d);
    printf("%d-%02d-%02d is day %d\n", y, m, d, yearday);

    y = 2024; yearday = 366;
    month_day(y, yearday, &m, &d);
    printf("Day %d of %d is %d-%02d-%02d\n", yearday, y, y, m, d);

    return 0;
}
