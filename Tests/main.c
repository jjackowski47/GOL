#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Life_generator.h"
#include "Make_png.h"
#include "Board.h"

int does_read_config_read_correctly(FILE* in)
{	
	
	Board* generation = read_config(in);
	
	int index[5][2] = {{3,3},
	    		  {3,4},
			  {3,5},
			  {4,4},
			  {5,5}};

	if(generation->numOfCol!=15 || generation->numOfRows!=15)
		return 0;

	int k=0;
	for(int i=0; i<generation->numOfRows; ++i)
		for(int j=0; j<generation->numOfCol; ++j)
		{

			if(generation->matrix[i][j]==1 && i==index[k][0] && j==index[k][1])
				++k;
			else if(generation->matrix[i][j]!=0)
			    	return 0;
		}
	return 1;
}

int does_save_config_save_correctly()
{
	FILE* in = fopen("testowy_plik_z_konfiguracja.txt","r");

	Board* generation = read_config(in);

	fclose(in);

	save_config("save_config_test.txt",generation);
	
	FILE* test = fopen("save_config_test.txt","r");
	
	int result = does_read_config_read_correctly(test);
	
	fclose(test);

	return result;
}

int does_generate_generate_correctly()
{
	FILE* in = fopen("testowy_plik_z_konfiguracja.txt","r");
	Board* generation = read_config(in);
	fclose(in);
	generate(generation);
	FILE* test = fopen("generate_test.txt","r");
	Board* generate_test = read_config(test);
	fclose(test);
	for(unsigned int i=0; i<generation->numOfRows; ++i)
		for(unsigned int j=0; j<generation->numOfCol; ++j)
			if(generation->matrix[i][j]!=generate_test->matrix[i][j]) return 0;
		
	return 1;
		
}

int does_edge_work_correctly()
{
	if(edge(-1,7)!=6) return 0;
	if(edge(3,3)!=0) return 0;

	return 1;
}

int does_count_neighbours_count_correctly()
{
	FILE* in = fopen("testowy_plik_z_konfiguracja_1.txt","r");
	Board* generation = read_config(in);
	fclose(in);
	int index[6][6];
	
	for(unsigned int i=0; i<6; ++i)
		for(unsigned int j=0; j<6; ++j)
			index[i][j] = 0;
	FILE* test = fopen("count_neighbours_test.txt","r");
	unsigned int i,j,n_count;
	while(fscanf(test,"%d %d %d\n",&i,&j,&n_count)==3){
				
				index[i][j] = n_count;
				}
	fclose(test);
	for(i=0; i<generation->numOfRows; ++i)
		for(j=0; j<generation->numOfCol; ++j)
			if(count_neighbours(generation,i,j)!=index[i][j])
				return 0;
	return 1;
}

int main()
{
	FILE* in = fopen("testowy_plik_z_konfiguracja.txt","r");
	printf("does_read_config_read_correctly = %d\n", does_read_config_read_correctly(in));
	fclose(in);
	printf("does_save_config_save_correctly = %d\n",does_save_config_save_correctly());
	printf("does_edge_work_correctly = %d\n",does_edge_work_correctly());
	printf("does_count_neighbours_count_correctly = %d\n",does_count_neighbours_count_correctly());	
	printf("does_generate_generate_correctly = %d\n",does_generate_generate_correctly());
}
