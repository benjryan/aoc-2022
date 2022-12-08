#include <stdio.h>

int main()
{
    FILE *file = fopen("../input.txt", "r");

    int a0, a1, b0, b1, counter = 0;
    char buffer[128];
    while (fgets(buffer, 128, file) != NULL)
    {
        sscanf(buffer, "%d-%d, %d-%d", &a0, &a1, &b0, &b1);
        if ((a1 >= b0 && a0 <= b1) || (b1 >= a0 && b0 <= a1))
            counter++;
    }

    printf("Answer: %d\n", counter);
}
