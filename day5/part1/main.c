#include <stdio.h>

#define STACK_HEIGHT 128

int is_crate_valid(char crate)
{
    return (crate >= 'A' && crate <= 'Z');
}

int get_next_free_id(char* stack)
{
    for (int i = STACK_HEIGHT - 1; i >= 0; i--)
    {
        char crate = stack[i];
        if (is_crate_valid(crate))
        {
            return i + 1;
        }
    }

    return 0;
}

void print_stacks(char stacks[][STACK_HEIGHT])
{
    for (int j = STACK_HEIGHT - 1; j >= 0; --j)
    {
        for (int i = 0; i < 9; ++i)
        {
            char c = stacks[i][j];
            if (is_crate_valid(c))
            {
                printf("[%c] ", c);
            }
            else
            {
                printf("    ");
            }
        }
        printf("\n\n");
    }
}

int main()
{
    FILE *file = fopen("../input.txt", "r");
    char buffer[128];
    char stacks[9][STACK_HEIGHT] = {};
    int line_count = 0;
    while (fgets(buffer, 128, file) != NULL)
    {
        if (line_count < 8)
        {
            // read stacks
            for (int i = 0; i < 9; ++i)
            {
                char crate = buffer[1 + i * 4];
                if (is_crate_valid(crate))
                {
                    // valid
                    stacks[i][7 - line_count] = crate;
                }
            }
        }
        else if (line_count >= 10)
        {
            // moves
            int crate_count;
            int from_col;
            int to_col;
            sscanf(buffer, "move %d from %d to %d\n", &crate_count, &from_col, &to_col);

            int move_counter = 0;
            for (int i = STACK_HEIGHT - 1; i >= 0 && move_counter < crate_count; i--)
            {
                char from_crate = stacks[from_col - 1][i];
                if (is_crate_valid(from_crate))
                {
                    int free_id = get_next_free_id(stacks[to_col - 1]);
                    stacks[to_col - 1][free_id] = from_crate;
                    stacks[from_col - 1][i] = 0;
                    move_counter++;
                }
            }
        }

        line_count++;
    }

    print_stacks(stacks);

    printf("Answer: ");
    for (int i = 0; i < 9; ++i)
    {
        int last_id = get_next_free_id(stacks[i]) - 1;
        char c = stacks[i][last_id];
        printf("%c", c);
    }
    printf("\n");
}
