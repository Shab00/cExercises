#include <stdio.h>

float convert(float temp);

int main(void)
{
    float fahr, celsius;
    int lower, upper, step;

    lower = 0;
    upper = 300;
    step = 20;

    fahr = lower;
    while (fahr <= upper) {
        celsius = convert(fahr);
        printf("%3.0f %6.1f\n", fahr, celsius);
        fahr = fahr + step;
    }

    return 0;
}

float convert(float temp)
{
    float tempC;

    return tempC = (5.0/9.0) * (temp-32.0);
}
