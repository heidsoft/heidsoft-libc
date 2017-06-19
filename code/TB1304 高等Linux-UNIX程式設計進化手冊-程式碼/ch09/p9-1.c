#include "ch09.h"
/*指令選單*/
char *menu[]={"a-add new record", "d-delete record", "q-quit", NULL}; 
int getchoice(char *greet, char *choices[])
{
   int selected, chosen = 0;
   char **option;
   do{
      printf( "Choice: %s\n", greet);
      option = choices;
	  while(*option) {     /* 列印選單 */
          printf ("%s\n", *option);
          option++;
      }
      selected = getchar();  /* 讀入指令字元 */
      option = choices;
      while(*option){         /* 檢查指令字元，正確則置chosen=1*/
         if(selected == *option[0]) {
            chosen = 1;
            break;
         }
         option++;
	  }
      if(!chosen)   /* 若指令不正確，提示重新輸入指令，直到得到正確的指令*/
      printf("Incorrect choice,please select agian\n");
   } while(!chosen); 
   return selected;
}
int main(void)
{
   int choice = 0;
   do {
      choice = getchoice("Please select a action", menu);
      printf ("Your choice is: %c\n", choice);
   } while (choice != 'q') ;
   exit (0);
}
