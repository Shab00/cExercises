#include <stdio.h>
#include "hfcal.h"

double calories(double weight, double distance) {
    return weight * distance * 0.75;
}

void display_calories(float weight, float distance, float coeff)
{
    printf("Weight: %3.3f lbs\n", weight);
    printf("Distance: %3.2f miles\n", distance);
    printf("Calories burned: %4.2f cal\n", coeff * weight * distance);
}
