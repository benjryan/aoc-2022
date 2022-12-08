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


unsigned long long find_dirs(struct Dir* dir, int max_size, int depth)
{
    unsigned long long total_size = 0;
    if (dir->size <= max_size)
        total_size += dir->size;

    char buffer[1024];
    for (int i = 0; i < depth; ++i)
        buffer[i] = ' ';

    sprintf(buffer + depth, "- dir: %s, size: %d\n", dir->name, dir->size);
    printf(buffer);

    for (int i = 0; i < dir->file_count; ++i)
    {
        memset(buffer + depth, 0, 1024 - depth);
        sprintf(buffer + depth, " - file: %s, size %d\n", dir->files[i]->name, dir->files[i]->size);
        printf(buffer);
    }
        
    for (int i = 0; i < dir->dir_count; ++i)
    {
        total_size += find_dirs(dir->dirs[i], max_size, depth + 1);
    }

    return total_size;
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

    unsigned long long total_size = find_dirs(&root, 100000, 0);
    printf("Answer: %llu\n", total_size);
}
