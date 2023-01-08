#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MINUTES 26
#define MAX_VALVES 128

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
    char opened[MAX_VALVES];
    char visited[MAX_VALVES];
    char minute[2];
    char valves[2];
    char finished[2];
    int rate;
    int pressure;
} Path;

Valve valves[MAX_VALVES] = {};
int valve_count = 0;
int best_pressure = 0;

#define get_valve_id(s) (s[0] << 8 | s[1])

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
    int minute = path.minute[player];

    // Optimisation - Safe to say if we haven't released anything in 5 minutes
    // then we're not breaking any records.
    if (minute >= 5 && path.pressure <= 20)
        return;
    
    // Optimisation - Cheating. You could only know these numbers if you already
    // knew the answer to the question.
    //if (minute >= 10 && path.pressure < 150)
    //    return;

    if (player == 0)
        path.pressure += path.rate;

    if (minute == MAX_MINUTES - 1)
    {
        //printf("p = %d, minute %d, pressure %d\n", player, minute, path.pressure);
        if (path.pressure > best_pressure)
            best_pressure = path.pressure;

        return;
    }

    char other_player = (player + 1) % 2;
    char valve_index = path.valves[player];
    if (path.finished[player])
    {
        //printf("p = %d, minute %d, wait %d\n", player, minute, (int)valve_index);
        path.minute[player]++;
        simulate_path(path, other_player);
        return;
    }

    int simulated = 0;
    Valve* valve = valves + valve_index;

    // open
    if (valve->rate > 0 && !path.opened[valve_index])
    {
        Path new_path = path;
        new_path.opened[valve_index] = 1;
        new_path.rate += valve->rate;
        new_path.minute[player] = minute + 1;
        new_path.valves[player] = valve_index;
        //new_path.valves[player][new_path.minute[player]] = valve_index;
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

        if (path.visited[next_index])
            continue;

        Path new_path = path;
        new_path.minute[player] = minute + 1;
        new_path.valves[player] = next_index;
        new_path.visited[next_index] = 1;
        //printf("p = %d, minute %d, move %d\n", player, minute, (int)next_index);
        simulate_path(new_path, other_player);
        simulated = 1;
    }
    
    // the case where we have nothing left to do
    if (simulated == 0)
    {
        //printf("p = %d, minute %d, wait %d\n", player, minute, (int)valve_index);
        if (path.finished[other_player])
        {
            int minutes_remaining = MAX_MINUTES - (minute + 1);
            int final_pressure = path.pressure + path.rate * minutes_remaining;
            if (final_pressure > best_pressure)
                best_pressure = final_pressure;
        }
        else 
        {
            path.finished[player] = 1;
            path.minute[player] = minute + 1;
            path.valves[player] = valve_index;
            simulate_path(path, other_player);
        }
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
    start_path.valves[0] = valve_index;
    start_path.valves[1] = valve_index;
    start_path.visited[valve_index] = 1;
    simulate_path(start_path, 0);

    time_t time_end = clock();
    printf("Pressure released: %d, Elapsed time: %fs\n", best_pressure, (double)(time_end - time_start) / (double)CLK_TCK);
}
