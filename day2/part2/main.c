#include <stdio.h>

typedef enum
{
    ROCK,
    PAPER,
    SCISSORS
} Move;

typedef enum
{
    LOSE,
    DRAW,
    WIN
} Result;

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

Move get_winning_move(Move opponent)
{
    switch (opponent)
    {
        case ROCK: return PAPER;
        case PAPER: return SCISSORS;
        case SCISSORS: return ROCK;
    }
}

Move get_losing_move(Move opponent)
{
    switch (opponent)
    {
        case ROCK: return SCISSORS;
        case PAPER: return ROCK;
        case SCISSORS: return PAPER;
    }
}

Move get_move(Move opponent, Result result)
{
    if (result == DRAW)
        return opponent;

    if (result == WIN)
        return get_winning_move(opponent);

    return get_losing_move(opponent);
}

int main()
{
    FILE *file = fopen("../input.txt", "r");
    char buffer[8];
    int total_score = 0;
    while (fgets(buffer, 8, file) != NULL)
    {
        Move opponent = buffer[0] - 'A';
        Result result = buffer[2] - 'X';

        Move player = get_move(opponent, result);
        total_score += get_score(opponent, player);
    }

    printf("Total score: %d\n", total_score);
}
