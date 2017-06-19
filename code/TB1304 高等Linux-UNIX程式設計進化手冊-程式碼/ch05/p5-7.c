#include "ch05.h"
typedef enum{
  ST_PROGRAM, ST_FUNCTION, ST_SUBROUTINE, ST_END, ST_NONE // 其它敘述型態...
} statement;
jmp_buf eof_buf;
int parse_file(void)
{
   statement st;
   if (setjmp(eof_buf)){  /* 設定出錯傳回點 */
      error("Unexpected end of file in '%s'", source_file);
      return FAILUR;	 /* 遇到非標準的EOF  */
   }
   while((st = next_statement())!= ST_NONE)  /* 標準的EOF，結束掃描  */
      switch (st) {
      case ST_PROGRAM:     /* 段首敘述  */
      case ST_SUBROUTINE:
      case ST_FUNCTION:
          accept_statement(st);
          parse_progunit(); 
          break;
          /* 其它段首敘述處理  */
      }
   /* 後繼處理 ...  */  
   return SUCCESS;
}
void parse_progunit()
{ 
   statement st;
   st = parse_spec();        /* 掃描說明敘述  */
   if (st == ST_NONE)
	  longjmp(eof_buf, 1);  /* 遇到非標準的EOF，直接傳回到主控過程  */
   else if (st == ST_END) {   /* 段尾敘述  */
      accept_statement(st);
      goto done;
   }
   parse_executable(st);     /* 掃描執行敘述  */
done: ; /* 後繼處理  */
}
parse_executable(statement st)  /* 掃描執行敘述，內含段尾敘述  */
{
   for (;; st = next_statement())
      switch (st) {
	  case ST_NONE:  
	     longjmp (eof_buf, 1); /* 遇到非標準的EOF，直接傳回到主控過程  */
	  case ST_END:      /* 段尾敘述  */
	     accept_statement(st);
	     return;
	  case ST_IF_BLOCK:
	     parse_if_block();
	     continue;
	  case ST_DO:
	     parse_do_block();
	     continue;
	  /* 其它可執行敘述處理...  */
	 }
}
