#include <stdio.h>
#include <string.h>

#define SIZE 99

void check_trees(char* trees, int step, int* tree_scores)
{
    int tree_cache[10] = { 0 };
    for (int i = 0; i < SIZE; ++i, trees += step, tree_scores += step)
    {
        char tree = *trees;
        int tree_id = tree - '0';
        int closest_tree = 0;
        for (int j = tree_id; j < 10; ++j)
        {
            if (tree_cache[j] > closest_tree)
            {
                closest_tree = tree_cache[j];
            }
        }

        *tree_scores *= (i == 0) ? 1 : (i - closest_tree);
        tree_cache[tree - '0'] = i;
    }
}

int main()
{
    FILE *file = fopen("../input.txt", "r");

    char buffer[128];
    char trees[SIZE][SIZE];
    int tree_scores[SIZE][SIZE];
    for (int i = 0; i < SIZE*SIZE; ++i)
        *((int*)tree_scores + i) = 1;

    int line = 0;
    while (fgets(buffer, 128, file) != NULL)
    {
        memcpy(trees[line++], buffer, SIZE);
    }

    for (int i = 0; i < SIZE; ++i)
    {
        check_trees(&trees[i][0], 1, &tree_scores[i][0]);
        check_trees(&trees[i][SIZE - 1], -1, &tree_scores[i][SIZE - 1]);
        check_trees(&trees[0][i], SIZE, &tree_scores[0][i]);
        check_trees(&trees[SIZE - 1][i], -SIZE, &tree_scores[SIZE - 1][i]);
    }

    int highest = 0;
    for (int i = 0; i < SIZE*SIZE; ++i)
    {
        int height = *((int*)tree_scores + i);
        if (height > highest)
        {
            highest = height;
        }
    }
    printf("Answer: %d\n", highest);
}
