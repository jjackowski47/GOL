#ifndef GRA_W_ZYCIE_LIFE_GENERATOR_H
#define GRA_W_ZYCIE_LIFE_GENERATOR_H

#include "Board.h"


int edge(int x, unsigned int mod);
int count_neighbours (Board* board, int x, int y);
void generate (Board* generation);
void print (Board* generation);

#endif //GRA_W_ZYCIE_LIFE_GENERATOR_H
