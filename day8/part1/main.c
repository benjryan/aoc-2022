#include <windows.h>
#include <stdio.h>
#include <string.h>

#define SIZE 99
#define VISIBLE_BIT (1 << 7)

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);

    PCFreq = (double)(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (double)(li.QuadPart-CounterStart)/PCFreq;
}

void check_trees(char* trees, int step, int *total)
{
    int highest_tree = -1; 
    for (int i = 0; i < SIZE; ++i, trees += step)
    {
        char tree = *trees;
        char real_tree = tree & ~VISIBLE_BIT;
        if (real_tree <= highest_tree)
            continue;

        highest_tree = real_tree;

        if ((tree & VISIBLE_BIT) != 0)
            continue;

        *total += 1;
        highest_tree = real_tree;
        *trees |= VISIBLE_BIT;

        if (real_tree == '9')
            break;
    }
}

int main()
{
    StartCounter();
    FILE *file = fopen("../input.txt", "r");

    char buffer[128];
    char trees[SIZE][SIZE];
    int line = 0;
    while (fgets(buffer, 128, file) != NULL)
    {
        memcpy(trees[line++], buffer, SIZE);
    }

    int visible_trees = 0;
    for (int i = 0; i < SIZE; ++i)
    {
        check_trees(&trees[i][0], 1, &visible_trees);
        check_trees(&trees[i][SIZE - 1], -1, &visible_trees);
        check_trees(&trees[0][i], SIZE, &visible_trees);
        check_trees(&trees[SIZE - 1][i], -SIZE, &visible_trees);
    }

    printf("Answer: %d\n", visible_trees);
    printf("%f\n", GetCounter());
}
