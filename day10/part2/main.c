#include <stdio.h>
#include <stdlib.h>

typedef enum Instruction
{
    INST_NOOP,
    INST_ADDX,

    INST_COUNT
} Instruction;

int main()
{
    FILE *file = fopen("../input.txt", "r");
    
    char line[64];
    int cycle = 1;
    int last_inst_cycle = 0;
    int x = 1;
    int addx = 0;
    int signal_total = 0;
    Instruction inst = INST_COUNT;
    char crt[6][40];
    while (1)
    {
        printf("cycle: %d\n", cycle);
        if (inst == INST_COUNT)
        {
            if (fgets(line, 64, file) == NULL)
                break;

            if (line[0] == 'n')
            {
                inst = INST_NOOP;
                printf("noop\n");
            }
            else
            {
                inst = INST_ADDX;
                sscanf(line, "addx %d\n", &addx);
                printf("addx %d\n", addx);
            }

            last_inst_cycle = cycle;
        }

        if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || 
            cycle == 180 || cycle == 220)
            signal_total += cycle * x;


        int row = (cycle - 1) / 40;
        int column = (cycle - 1) % 40;
        int pixel_dst = abs(x - column);
        crt[row][column] = (pixel_dst <= 1) ? '#' : '.';

        cycle++;

        if (inst == INST_ADDX && (cycle - last_inst_cycle) == 2)
        {
            x += addx;
            inst = INST_COUNT;
            printf("x = %d\n", x);
        }
        else if (inst == INST_NOOP && (cycle - last_inst_cycle) == 1)
        {
            inst = INST_COUNT;
        }
    }

    // print result
    for (int row = 0; row < 6; ++row)
    {
        for (int column = 0; column < 40; ++column)
        {
            printf("%c", crt[row][column]);
        }

        printf("\n");
    }
}
