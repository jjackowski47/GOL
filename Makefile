gra_w_zycie: Main.o Life_generator.o Board.o Make_png.o
	gcc -o gra_w_zycie $^ -lpng

Make_png.o: Make_png.c Make_png.h
Board.o: Board.c Board.h
Life_generator.o: Life_generator.c Life_generator.h Board.h
Main.o: Main.c Board.h Life_generator.h Make_png.h

.PHONY: clean

clean:
	-rm *.o gra_w_zycie

.PHONY: cleanfolders

cleanfolders:
	-rm Generations/*.txt Images/*.png
