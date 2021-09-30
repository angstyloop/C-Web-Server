#ifndef _FILELS_H_ // This was just _FILE_H_, but that interfered with Cygwin
#define _FILELS_H_

typedef struct file_data file_data;
struct file_data {
  char* name;
  size_t size;
  char* data;
};

file_data* file_load(char* filename);
void file_free(file_data* filedata);

#endif
