#include <stdio.h>
#include <stdlib.h>
#include "Life_generator.h"
#include "Board.h"

int edge(int x, unsigned int mod)
{
    x += mod;
    return x % mod;
}

int count_neighbours (Board* board, int x, int y) {
    int	i, j, count = 0;
    for (i = -1; i < 2; ++i) {
        int row = x+i;
        for (j = -1; j < 2; ++j)
        {
            int col = y+j;
            if(row < 0 || row >= board->numOfRows)
                row = edge(row,board->numOfRows);

            if ( col < 0 || col >= board->numOfCol)
                col = edge(col,board->numOfCol);

            count += board->matrix[row][col];
        }

    }

    return count - board->matrix[x][y];
}

void generate (Board* generation) {
    unsigned int 	i, j, n;
    char**	new_board = malloc(generation->numOfRows*sizeof (char*));
    for(i=0; i<generation->numOfRows; ++i)
        new_board[i] = malloc(generation->numOfCol* sizeof(char));


    for (i = 0; i < generation->numOfRows; ++i)
        for (j = 0; j < generation->numOfCol; ++j) {

            n = count_neighbours (generation, i, j);
            if (n == 3)
                new_board[i][j] = 1;
            else if (n != 2)
                new_board[i][j] = 0;
            else new_board[i][j] = generation->matrix[i][j];

        }


    free(generation->matrix);
    generation->matrix = new_board;
}

void print (Board* generation) {
    unsigned int	i, j;

    printf ("\n");
    for (i = 0; i < generation->numOfRows; i++)
        printf("--");
    printf ("\n");
    for (i = 0; i < generation->numOfCol; i++) {
        for (j = 0; j < generation->numOfRows; j++) {
            printf ("%s", generation->matrix[j][i] ? "<>" : "  ");
        }
        printf ("|\n");
    };
    for (i = 0; i < generation->numOfRows; i++)
        printf("--");
    printf ("\n");
}
