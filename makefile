EXE = barvis
OBJ = main.o gl_util.o file_io.o window.o
LIB = -lGLEW -lX11 -lGL -lGLU -lglut

all: $(OBJ)
	gcc -o $(EXE) $(OBJ) $(LIB)

main.o: src/main.c gl_util.o window.o
	gcc -c src/main.c

gl_util.o: src/util/gl_util.c src/util/gl_util.h file_io.o
	gcc -c src/util/gl_util.c

window.o: src/util/window.c src/util/window.h 
	gcc -c src/util/window.c

file_io.o: src/util/file_io.c src/util/file_io.h 
	gcc -c src/util/file_io.c

clean:
	rm -rf barvis
	rm -rf *.o
