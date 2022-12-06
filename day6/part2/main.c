#include <stdio.h>

int main()
{
    FILE *file = fopen("../input.txt", "r");
    
    char buffer[4096];
    int counter = 0;
    while ((buffer[counter++] = fgetc(file)) != EOF)
    {
        char c = buffer[counter - 1];

        // ensure this new char is different to the previous ones
        for (int i = 0; i < counter - 1; ++i)
        {
            if (buffer[i] == c)
            {
                // reset
                buffer[0] = c;
                counter = 1;
                break;
            }
        }

        if (counter == 14)
        {
            printf("Answer: %lld\n", ftell(file));
            return 0;
        }
    }
}
