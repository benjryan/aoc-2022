#include <stdio.h>

int count_line(char *buffer)
{
    char *p = buffer;
    while (1)
    {
        if (*p < 'A' || *p > 'z')
            break;

        p++;
    }

    return p - buffer;
}

int get_priority(char c)
{
    if (c >= 'a')
        return c - 'a' + 1;

    return c - 'A' + 27;
}

int main()
{
    FILE *file = fopen("../input.txt", "r");
    char buffer[128];
    int total_priority = 0;
    while (fgets(buffer, 128, file) != NULL)
    {
        int size = count_line(buffer);
        int half_size = size / 2;
        for (char *front = buffer; front < buffer + half_size; ++front)
        {
            for (char *back = buffer + size - 1; back >= buffer + half_size; --back)
            {
                if (*front == *back)
                {
                    total_priority += get_priority(*front);
                    goto next_line;
                }
            }
        }
next_line:
    }

    printf("Priority sum: %d\n", total_priority);
}
