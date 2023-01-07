#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Valve
{
    short id;
    int rate;
    short next[5];
    int next_count;
} Valve;

Valve valves[256];
int valve_count = 0;

short get_valve_id(char* s)
{
    return s[0] << 8 | s[1];
}

Valve* get_valve(short id)
{
    for (int i = 0; i < valve_count; ++i)
    {
        if (valves[i].id == id)
            return valves + i;
    }

    return NULL;
}

int simulate(int minute, int rate, int pressure, Valve* valve)
{
    while (minute <= 30)
    {
        pressure += rate;

        // move or open
        for (int i = 0; i < valve->next_count; ++i)
        {
            Valve* next = get_valve(valve->next[i]);
            if (next->rate > 0)
            {
                //
                simulate
            }
        }
        if (valve->rate == 0)
        {
        }
    }
}

int main()
{
    FILE *file = fopen("../input.txt", "r");

    char buffer[256];
    while (fgets(buffer, 128, file) != NULL)
    {
        char valve[2];
        int rate;
        int pos;

        Valve *valve = &valves[valve_count++];
        
        sscanf(buffer, "Valve %s has flow rate=%d; tunnels lead to valve%n", &valve, &rate, &pos);
        if (buffer[pos] == 's')
            pos++;

        if (buffer[pos] == ' ')
            pos++;

        valve->id = get_valve_id(valve);
        valve->rate = rate;

        //printf("%s %d, ", valve, rate);
        char *token = strtok(buffer + pos, ", ");
        valve->next_count = 0;
        valve->next[valve->next_count++] = get_valve_id(token);

        //printf("%s ", token);
        while (token != NULL)
        {
            token = strtok(NULL, ", ");
            valve->next[valve->next_count++] = get_valve_id(token);
            //printf("%s ", token);
        }
    }

    {
        int minute = 1;
        int rate = 0;
        int pressure = 0;
        Valve* valve = get_valve(get_valve_id("AA"));

        while (minute <= 30)
        {
            pressure += rate;

            // move or open
            if (valve->rate == 0)
            {
            }
        }
    }
}
