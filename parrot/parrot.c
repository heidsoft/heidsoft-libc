/***************************
 *
 * heidsoft@sina.com
 *
 **************************/

//use fgets() 和 fputs()
//
//
#include <stdio.h>
#define MAXLINE 20
int main(void)
{
  char line[MAXLINE];

  while(fgets(line , MAXLINE , stdin ) != NULL &&
          line[0] !='\0')
      fputs(line ,stdout);

  return 0;
}
