#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MINUTES 30
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
    char minute;
    char valve;
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

void simulate_path(Path path)
{
    path.pressure += path.rate;

    // Optimisation - Safe to say if we haven't released anything in 5 minutes
    // then we're not breaking any records.
    if (path.minute >= 5 && path.pressure <= 20)
        return;

    if (path.minute == MAX_MINUTES - 1)
    {
        //printf("minute %d, pressure %d\n", path.minute, path.pressure);
        if (path.pressure > best_pressure)
            best_pressure = path.pressure;

        return;
    }

    int simulated = 0;
    char valve_index = path.valve;
    Valve* valve = valves + valve_index;
    
    // open
    if (valve->rate > 0 && !path.opened[valve_index])
    {
        Path new_path = path;
        new_path.opened[valve_index] = 1;
        new_path.rate += valve->rate;
        new_path.minute++;
        new_path.valve = valve_index;
        //printf("minute %d, open %d\n", path.minute, (int)valve_index);
        simulate_path(new_path);
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
        new_path.visited[next_index] = 1;
        new_path.minute++;
        new_path.valve = next_index;
        //printf("minute %d, move %d\n", path.minute, (int)next_index);
        simulate_path(new_path);
        simulated = 1;
    }
    
    // the case where we have nothing left to do
    if (simulated == 0)
    {
        //printf("minute %d, wait %d\n", path.minute, (int)valve_index);
        //path.minute++;
        //path.valves[path.minute] = valve_index;
        //simulate_path(path);
        int minutes_remaining = MAX_MINUTES - (path.minute + 1);
        int final_pressure = path.pressure + path.rate * minutes_remaining;
        if (final_pressure > best_pressure)
            best_pressure = final_pressure;
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
    start_path.visited[valve_index] = 1;
    start_path.valve = valve_index;
    simulate_path(start_path);

    time_t time_end = clock(); 
    printf("Pressure released: %d, Elapsed time: %fs\n", best_pressure, (double)(time_end - time_start) / (double)CLK_TCK);
}
