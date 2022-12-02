#include <stdio.h>

int main()
{
    FILE *file = fopen("../input.txt", "r");

    int best_three[3] = { 0, 0, 0 };
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
            for (int i = 0; i < 3; ++i)
            {
                if (calorie_counter > best_three[i])
                {
                    // shuffle next bests along
                    for (int j = 2; j > i; --j)
                    {
                        int prev_best = best_three[j - 1];
                        best_three[j] = prev_best;
                    }

                    // save new best
                    best_three[i] = calorie_counter;
                    break;
                }
            }

            // reset for next elf
            calorie_counter = 0;
        }
    }

    int total_calories = best_three[0] + best_three[1] + best_three[2];
    printf("Total calories (top 3): %d\n", total_calories);
}
