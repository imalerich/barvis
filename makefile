EXE = barvis
OBJ = main.o gl_util.o file_io.o
LIB = -lGLEW -lX11 -lGL -lGLU -lglut

all: $(OBJ)
	gcc -o $(EXE) $(OBJ) $(LIB)

main.o: src/main.c gl_util.o
	gcc -c src/main.c

gl_util.o: src/gl_util.c src/gl_util.h file_io.o
	gcc -c src/gl_util.c

file_io.o: src/file_io.c src/file_io.h 
	gcc -c src/file_io.c

clean:
	rm -rf barvis
	rm -rf *.o
