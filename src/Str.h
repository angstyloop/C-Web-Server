typedef struct String String;
struct String{
  size_t len;
  char* data;
}

String* String_create();
String* String_init();
String* String_new();
String* String_str();
String* String_free();
