#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Valve
{
    short id;
    int rate;
    short next_ids[5];
    char next_valves[5];
    int next_count;
} Valve;

typedef struct Path
{
    char opened[256];
    char minute[2];
    char valves[2][30];
    int rate;
    int pressure;
} Path;

Valve valves[256] = {};
int valve_count = 0;
int best_pressure = 0;

short get_valve_id(char* s)
{
    return s[0] << 8 | s[1];
}

char get_valve_index(short id)
{
    for (char i = 0; i < valve_count; ++i)
    {
        if (valves[i].id == id)
            return i;
    }

    return 255;
}

void simulate_path(Path path, int player)
{
    if (player == 0)
        path.pressure += path.rate;

    int minute = path.minute[player];

    // Optimisation - Safe to say if we haven't released anything in 5 minutes
    // then we're not breaking any records.
    if (minute >= 5 && path.rate == 0)
        return;

    if (minute == 25)
    {
        //printf("p = %d, minute %d, pressure %d\n", player, minute, path.pressure);
        if (path.pressure > best_pressure)
        {
            best_pressure = path.pressure;
        }
        return;
    }

    int simulated = 0;
    char other_player = (player + 1) % 2;
    char valve_index = path.valves[player][minute];
    Valve* valve = valves + valve_index;

    // open
    if (valve->rate > 0 && !path.opened[valve_index])
    {
        Path new_path = path;
        new_path.opened[valve_index] = 1;
        new_path.rate += valve->rate;
        new_path.minute[player]++;
        new_path.valves[player][new_path.minute[player]] = valve_index;
        //printf("p = %d, minute %d, open %d\n", player, minute, (int)valve_index);
        simulate_path(new_path, other_player);
        simulated = 1;
    }

    // move
    for (int i = 0; i < valve->next_count; ++i)
    {
        char next_index = valve->next_valves[i];
        if (path.opened[next_index])
            continue;

        int in_history = 0;
        for (int p = 0; p < 2; p++)
        {
            for (int j = 0; j < path.minute[p]; ++j)
            {
                if (next_index == path.valves[p][j])
                {
                    in_history = 1;
                    goto exit_loop;
                }
            }
        }

exit_loop:
        if (in_history)
            continue;

        Path new_path = path;
        new_path.minute[player]++;
        new_path.valves[player][new_path.minute[player]] = next_index;
        //printf("p = %d, minute %d, move %d\n", player, minute, (int)next_index);
        simulate_path(new_path, other_player);
        simulated = 1;
    }
    
    // the case where we have nothing left to do
    if (simulated == 0)
    {
        //printf("p = %d, minute %d, wait %d\n", player, minute, (int)valve_index);
        path.minute[player]++;
        path.valves[player][path.minute[player]] = valve_index;
        simulate_path(path, other_player);
    }
}

int main()
{
    time_t time_start = clock();
    FILE *file = fopen("../input.txt", "r");

    char buffer[256];
    while (fgets(buffer, 256, file) != NULL)
    {
        char valve_str[2];
        int rate;
        int pos;

        Valve *valve = &valves[valve_count++];
        
        sscanf(buffer, "Valve %s has flow rate=%d; tunnels lead to valve%n", &valve_str, &rate, &pos);
        if (buffer[pos] == 's')
            pos++;

        if (buffer[pos] == ' ')
            pos++;

        valve->id = get_valve_id(valve_str);
        valve->rate = rate;

        char *token = strtok(buffer + pos, ", ");
        valve->next_count = 0;
        valve->next_ids[valve->next_count++] = get_valve_id(token);

        while (token != NULL)
        {
            token = strtok(NULL, ", ");
            if (token == NULL)
                break;

            valve->next_ids[valve->next_count++] = get_valve_id(token);
        }
    }

    for (int i = 0; i < valve_count; ++i)
    {
        Valve *valve = valves + i;
        for (int j = 0; j < valve->next_count; ++j)
        {
            valve->next_valves[j] = get_valve_index(valve->next_ids[j]);
        }
    }

    Path start_path = {};
    char valve_index = get_valve_index(get_valve_id("AA"));
    start_path.valves[0][0] = valve_index;
    start_path.valves[1][0] = valve_index;
    simulate_path(start_path, 0);

    time_t time_end = clock();
    printf("Pressure released: %d, Elapsed time: %fs\n", best_pressure, (double)(time_end - time_start) / (double)CLK_TCK);
}
