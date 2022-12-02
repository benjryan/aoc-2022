#include <stdio.h>

typedef enum
{
    ROCK,
    PAPER,
    SCISSORS
} Move;

int get_result_score(Move opponent, Move player)
{
    if (player == opponent)
        return 3;

    if (player == ROCK && opponent == SCISSORS ||
        player == PAPER && opponent == ROCK ||
        player == SCISSORS && opponent == PAPER)
        return 6;

    return 0;
}

int get_shape_score(Move player)
{
    switch (player)
    {
        case ROCK: return 1;
        case PAPER: return 2;
        case SCISSORS: return 3;
    }
}

int get_score(Move opponent, Move player)
{
    return get_shape_score(player) + get_result_score(opponent, player);
}

int main()
{
    FILE *file = fopen("../input.txt", "r");
    char buffer[8];
    int total_score = 0;
    while (fgets(buffer, 8, file) != NULL)
    {
        Move opponent = buffer[0] - 'A';
        Move player = buffer[2] - 'X';

        total_score += get_score(opponent, player);
    }

    printf("Total score: %d\n", total_score);
}
