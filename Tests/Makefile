gra_w_zycie: main.o Life_generator.o Board.o Make_png.o
	gcc -o gra_w_zycie $^ -lpng

Make_png.o: Make_png.c Make_png.h -lpng
Board.o: Board.c Board.h
Life_generator.o: Life_generator.c Life_generator.h Board.h
main.o: main.c Board.h Life_generator.h Make_png.h

.PHONY: clean

clean:
	-rm *.o gra_w_zycie
