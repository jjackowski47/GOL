#include <stdio.h>
#include <stdlib.h>
#include "Board.h"

Board* read_config(FILE* in)
{
    Board* board = malloc(sizeof(Board));
    fscanf(in,"%d %d\n", &board->numOfRows, &board->numOfCol);

    board->matrix = malloc(board->numOfRows*sizeof (char*));
    for(unsigned int i = 0; i < board->numOfRows; ++i)
    {
        board->matrix[i] = malloc(board->numOfCol* sizeof(char));
    }

    for(unsigned int i = 0; i < board->numOfRows; ++i)
    {
        for(int j = 0; j < board->numOfCol; ++j)
        {
            board->matrix[i][j] = 0;
        }
    }

    int i,j;
    while((fscanf(in,"%d %d\n",&i,&j))==2)
    {
        board->matrix[i][j] = 1;
    }

    return board;
}

void save_config(char* file_name, Board* generation)
{
    FILE* out = fopen(file_name,"w");
    fprintf(out,"%d %d\n",generation->numOfRows,generation->numOfCol);
    for(unsigned int i = 0; i < generation->numOfRows; ++i)
    {
        for(int j = 0; j < generation->numOfCol; ++j)
        {
            if(generation->matrix[i][j]==1) fprintf(out,"%d %d\n",i,j);
        }
    }
    fclose(out);

}
