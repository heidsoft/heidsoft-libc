/*************************************************
 *addaword.c --使用fprintf(),fscanf(),rewind() 函数
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#define MAX 40

int main(void)
{
  FILE *fp;
  char words[MAX];

  if((fp = fopen("words","a+"))==NULL)
  {
    fprintf(stderr,"Can't open \" words \" file.\n");
    exit(1);
  }

  puts("Enter words to add to the file press the Enter");
  puts("key at the beginning of a line to terminate.");
  while(gets(words)!=NULL && words[0]!='\0'){
      fprintf(fp,"%s ", words);
  }

  puts("File the contents: ");

  rewind(fp);//回到文件的开始处
  while(fscanf(fp, "%s", words) ==1 ){
    puts(words);
  }

  if(fclose(fp) != 0){
    fprintf(stderr,"Error closing file \n ");
  }

  return 0;
}


