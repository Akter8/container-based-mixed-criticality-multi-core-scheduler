#include <stdio.h>

#include "configuration.h"

extern FILE* file_input_level_A;

void open_files()
{
    file_input_level_A = fopen(FILE_INPUT_A, "r");
}

void close_files()
{
    fclose(file_input_level_A);
}