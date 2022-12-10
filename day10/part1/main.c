#include <stdio.h>

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

    printf("Answer: %d\n", signal_total);
}
