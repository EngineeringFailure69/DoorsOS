#ifndef STRING_H
#define STRING_H

int strlength(char* string);
char* strcopy(char* destination, const char* source);
int strcompare(const char* first, const char* second);
char* strncopy(char* dest, const char* source, int n);

#endif
