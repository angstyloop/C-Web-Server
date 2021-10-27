#ifndef STR_H
#define STR_H

typedef struct Str Str;
struct Str{
  size_t size;
  size_t len;
  char* data;
}

Str* Str_create();
Str* Str_resize();
Str* Str_growUntil(Str* this, size_t len);
Str* Str_init();
Str* Str_new();
Str* Str_print();
Str* Str_zero();
Str* Str_free();
Str* Str_concat();
Str* Str_append();
Str* Str_interp();
Str* Str_copy();
Str* Str_move();
Str* Str_growByFactor();
Str* Str_shrinkByFactor();
Str* Str_growToExceed();
Str* Str_shrink();
Str* Str_trim();
Str* Str_getDelim();
Str* Str_read();
Str* Str_write();

#endif
