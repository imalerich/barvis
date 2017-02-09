EXE = barvis
LIB = -lX11 -lGL -lGLU

all: 
	gcc -o $(EXE) src/main.c $(LIB)

clean:
	rm -rf barvis
	rm -rf *.o
