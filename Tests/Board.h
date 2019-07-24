#ifndef GRA_W_ZYCIE_BOARD_H
#define GRA_W_ZYCIE_BOARD_H

#include <stdio.h>

typedef struct {
    char **matrix;
    int numOfCol;
    int numOfRows;
} Board;

Board *read_config(FILE *in);

void save_config(char *file_name, Board *generation);

#endif //GRA_W_ZYCIE_BOARD_H
