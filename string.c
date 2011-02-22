#include <stdio.h>
#include <string.h>

void
str_reverse2(char* a, char* b)
{
#define SWITCH_INPLACE(a, b) do { char x = a; a = b; b = x; } while(0)
  for(; a != b && a+1 != b; ++a, --b) {
    SWITCH_INPLACE(*a, *b);
  }
  SWITCH_INPLACE(*a, *b);
}

void
str_reverse(char* s)
{
  str_reverse2(s, s+strlen(s));
}

/* Whoa, ugly */
void
str_reverse_words_inplace(char* s)
{
  char *start, *end;
  start = s;
yo:
  ;
  while(*start == ' ') ++start;
  if(*start == '\0') return;
  end = start;
  do ++end; while(*end && *end != ' ');
  str_reverse2(start, end-1);
  if(*end) *end = ' ';
  start = end+1;
  goto yo;
}

void
str_strip_whitespace_inplace(char* s)
{
  char* p2 = s;
  while(1) {
    while(*s && (*s == ' ' || *s == '\t' /* ... */)) ++s;
    if(*s == '\0') break;
    *p2++ = *s++;
  }
  *p2 = '\0';
}

char
str_findunique(char* s)
{
  char bitmap[256];
  memset(bitmap, '1', sizeof(bitmap));
  while(*s) {
    if(bitmap[*s] == '1' || bitmap[*s] == '2')
      ++bitmap[*s];
    ++s;
  }
  char* p = bitmap;
  while(*p != '2') ++p;
  return (char)(p-bitmap);
}
