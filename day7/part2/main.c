#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct File
{
    char name[64];
    int size;
    struct Dir* parent;
} File;

typedef struct Dir
{
    char name[64];
    int size;
    int file_count;
    int dir_count;
    struct File* files[64];
    struct Dir* dirs[64];
    struct Dir* parent;
} Dir;

void find_dir_to_delete(struct Dir* dir, int min_size, int *smallest)
{
    if (dir->size >= min_size && dir->size < *smallest)
        *smallest = dir->size;
        
    for (int i = 0; i < dir->dir_count; ++i)
    {
        find_dir_to_delete(dir->dirs[i], min_size, smallest);
    }
}

int main()
{
    FILE *file = fopen("../input.txt", "r");

    int a0, a1, b0, b1, counter = 0;
    char line[128];
    char command[64];
    char arg[64];
    
    Dir root = {};
    root.name[0] = '/';
    Dir *cd = &root;
    int line_counter = 0;

    while (fgets(line, 128, file) != NULL)
    {
        if (line[0] == '$')
        {
            // command
            int inputs = sscanf(line, "$ %s %s\n", command, arg);
            if (strncmp(command, "cd", 2) == 0)
            {
                if (arg[0] == '/')
                {
                    cd = &root;
                }
                else if (strncmp(arg, "..", 2) == 0)
                {
                    if (cd->parent != NULL)
                        cd = cd->parent;
                    else
                        printf("failed on line %d: 'cd ..'\n", line_counter);
                }
                else
                {
                    int found = 0;
                    for (int i = 0; i < cd->dir_count; ++i)
                    {
                        if (strncmp(cd->dirs[i]->name, arg, strlen(arg)) == 0)
                        {
                            cd = cd->dirs[i];
                            found = 1;
                            break;
                        }
                    }

                    if (!found)
                        printf("failed on line %d: 'cd %s', strlen(arg) = %d\n", line_counter, arg, strlen(arg));
                }
            }
            else if (strncmp(command, "ls", 2) == 0)
            {

            }
        }
        else
        {
            if (strncmp(line, "dir", 3) == 0)
            {
                sscanf(line, "dir %s\n", arg);
                Dir* new_dir = malloc(sizeof(Dir));
                memset(new_dir, 0, sizeof(Dir));
                strcpy(new_dir->name, arg);
                new_dir->parent = cd;
                cd->dirs[cd->dir_count++] = new_dir;
                //printf("adding dir id %d: %s\n", cd->dir_count - 1, arg);
            }
            else
            {
                // assume file
                int size;
                sscanf(line, "%d %s\n", &size, arg);
                File* new_file = malloc(sizeof(File));
                memset(new_file, 0, sizeof(File));
                new_file->size = size;
                strcpy(new_file->name, arg);
                new_file->parent = cd;
                cd->files[cd->file_count++] = new_file;

                // add size
                Dir* parent = cd;
                while (parent != NULL)
                {
                    parent->size += size;
                    parent = parent->parent;
                }
            }
        }

        line_counter++;
    }

    int capacity = 70000000;
    int required = 30000000;
    int used = root.size;
    int unused = capacity - root.size;
    int shortfall = required - unused;

    int smallest = used;
    find_dir_to_delete(&root, shortfall, &smallest);
    printf("Answer: %d\n", smallest);
}
