#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "file.h"

/** @brief Loads a file into memory and returns a pointer to the data.
 *
 *  @remark file_data::buffer on the returned file_data is null-terminated.
 *
 *  @param filename Name of the file to load. Must be a regular file in static root.
 *
 *  @return Pointer to newly created file_data.
 */
file_data* file_load(char* filename){

  // Get the file size. Assumes 
  //
  // TODO: Use DLL to store larger files in memory.
  struct stat st;
  if(stat(filename, &st) || SIZE_MAX <= st.st_size){return 0}

  // Make sure it's a regular file
  if(!S_ISREG(st.st_mode)){
    fprintf(stderr, "Not a regular file: %s.\n", filename);
    return 0;
  }

  // Open the file for reading
  FILE* fp=0;
  if(!(fp = fopen(filename, "rb"))){return 0}

  // Allocate as many bytes as the size of the file, plus one null byte.
  char* buffer=0;
  if(!(buffer = calloc(st.st_size + 1, 1))){return 0}

  // Read the entire file into the buffer, leaving the last byte '\0'.
  fread(buffer, 1, st.st_size, fp);

  // Allocate the file data struct
  file_data* filedata=0;
  if(!(filedata = calloc(sizeof(filedata), 1))){
    free(buffer);
    return 0;
  }

  // Initialize struct with name, data, and size.
  size_t szname = strlen(filename);
  filedata->name = calloc(szname + 1, 1);
  memcpy(filedata->name, filename, szname);

  filedata->data = buffer;
  filedata->size = st_size + 1;

  return filedata;
}

/**
 * Frees memory allocated by file_load().
 */
void file_free(file_data *filedata){
  free(filedata->data);
  free(filedata->name);
  free(filedata);
}
