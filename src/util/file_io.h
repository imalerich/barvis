#ifndef FILE_IO_H
#define FILE_IO_H

/**
 * Utility method to read the given file name into a single
 * character buffer. The caller will be responsible for
 * freeing the returned memeory.
 * 
 * \param filename Name of the file to read in as a buffer.
 * \return Buffer representation of the input file.
 */
char * read_file(const char * filename);

/**
 * Determines the length (in bytes) of the input file.
 * \param filename Name of the file to read.
 * \return The length (in bytes) of the file.
 */
size_t file_length(const char * filename);

#endif
