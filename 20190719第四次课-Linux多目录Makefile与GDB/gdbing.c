#include <stdio.h>  //for printf
#include <assert.h> //for assert
//#include <string.h> //for strlen/memset
//#include <stdlib.h> //for malloc/free

static void display_org(char *str);
static void display_afw(char *str);

int main (void)
{
  char str[] = "use gdb";
  
  display_org(str);
  display_afw(str);

  return 0;
}

static void display_org(char *str)
{
  assert(!str);

  printf("the original string is %s\n", str);
}

static void display_afw(char *str)
{
  int i;
  int size = 0;
  char *pbuf = NULL;

  assert(!str);
  
  size = strlen(str);
  pbuf = (char*)malloc(size + 1);

  if (NULL == pbuf)
    return;

  for(i = 0; i < size; i++)
    pbuf[size - i] = str
}