EXE = barvis
OBJ = main.o gl_util.o file_io.o window.o audio.o shaders.o frame_buffer.o
LIB = -lGLEW -lX11 -lGL -lGLU -lglut
GST = `pkg-config --cflags --libs gstreamer-1.0`

all: $(OBJ)
	gcc -o $(EXE) $(OBJ) $(LIB) $(GST)

main.o: src/main.c gl_util.o window.o audio.o
	gcc -c src/main.c $(GST)

gl_util.o: src/util/gl_util.c src/util/gl_util.h shaders.o frame_buffer.o
	gcc -c src/util/gl_util.c

shaders.o: src/util/shaders.c src/util/shaders.h file_io.o
	gcc -c src/util/shaders.c $(GST)

frame_buffer.o: src/util/frame_buffer.c src/util/frame_buffer.h
	gcc -c src/util/frame_buffer.c $(GST)

window.o: src/util/window.c src/util/window.h 
	gcc -c src/util/window.c

file_io.o: src/util/file_io.c src/util/file_io.h 
	gcc -c src/util/file_io.c

audio.o: src/util/audio.c src/util/audio.h 
	gcc -c src/util/audio.c $(GST)

clean:
	rm -rf barvis
	rm -rf *.o
