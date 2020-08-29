#include <stdio.h>

#include "configuration.h"

extern FILE* file_input[4];

void open_files()
{
    file_input[0] = fopen(FILE_INPUT_A, "r");
    file_input[1] = fopen(FILE_INPUT_B, "r");
    file_input[2] = fopen(FILE_INPUT_C, "r");
    file_input[3] = fopen(FILE_INPUT_D, "r");
}

void close_files()
{
    for (int i = 0; i < 4; i++)
    {
        fclose(file_input[i]);
    }
}