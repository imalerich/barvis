#include <stdio.h>
#include <stdlib.h>

#include "file_io.h"

char * read_file(const char * filename) {
    FILE * f = fopen(filename, "rb");
    char * buffer;
    size_t file_length;

    if (!f) {
        printf("failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // this will tell us how big of a buffer we need
    fseek(f, 0, SEEK_END);
    file_length = ftell(f);
    fseek(f, 0, SEEK_SET);

    // allocate our buffer and read the data
    buffer = (char *)malloc(file_length + 1); // +1 for NULL terminator
    fread(buffer, file_length, 1, f);
    buffer[file_length] = '\0';
    fclose(f);

    return buffer;
}

size_t file_length(const char * filename) {
    FILE * f = fopen(filename, "rb");
    size_t file_length;

    if (!f) {
        printf("failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    file_length = ftell(f);
    fclose(f);

    return file_length;
}
