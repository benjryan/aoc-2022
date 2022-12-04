#include <stdio.h>

int get_priority(char c)
{
    if (c >= 'a')
        return c - 'a' + 1;

    return c - 'A' + 27;
}

int search_string(char *buffer, char c)
{
    char *p = buffer;
    for (; *p != 0; ++p)
    {
        if (*p == c)
            return p - buffer;
    }

    return -1;
}

int main()
{
    FILE *file = fopen("../input.txt", "r");
    char buffer[3][128];
    int line_id = 0;
    int total_priority = 0;
    while (fgets(buffer[line_id++], 128, file) != NULL)
    {
        if (line_id < 3)
            continue;

        line_id = 0;

        char *p = buffer[0];
        for (; *p != '\n' && *p != 0; ++p) 
        {
            if (search_string(buffer[1], *p) >= 0 && 
                search_string(buffer[2], *p) >= 0)
            {
                total_priority += get_priority(*p);
                break;
            }
        }
    }

    printf("Priority sum: %d\n", total_priority);
}
