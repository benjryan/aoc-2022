#include <stdio.h>

int main()
{
    FILE *file = fopen("../input.txt", "r");

    int total_calories = 0;
    int calorie_counter = 0;
    int calories;
    char buffer[128];

    while (fgets(buffer, 128, file) != NULL)
    {
        if (sscanf(buffer, "%d", &calories) > 0)
        {
            // add calories to current elf
            calorie_counter += calories;
        }
        else
        {
            // end of elf
            if (calorie_counter > total_calories)
            {
                total_calories = calorie_counter;
            }

            // reset for next elf
            calorie_counter = 0;
        }
    }

    printf("Total calories: %d\n", total_calories);
}
