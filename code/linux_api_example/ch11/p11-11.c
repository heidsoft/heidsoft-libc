/* P動作--企圖進入一關鍵區 */
void semaphore_P(int sem_id)
{
   struct sembuf sb;
   sb.sem_num = 0;
   sb.sem_op = -1;          /* P動作－訊號量值減少 */
   sb.sem_flg = SEM_UNDO;   /* 若果死亡，解除訊號量請求*/
   if (semop(sem_id, &sb, 1) == -1){
       fprintf(stderr,”semaphore_P failed\n”);
       return(0);
   }
   return(1);
}
/* V動作--離開關鍵區 */
void semaphore_V(int sem_id)
{
   struct sembuf sb;
   sb.sem_num = 0;
   sb.sem_op = 1;           /* V動作－訊號量值增加 */
   sb.sem_flg = SEM_UNDO;   /* 若果死亡，解除訊號量請求*/
   if (semop(sem_id, &sb, 1) == -1){
       fprintf(stderr,”semaphore_V failed\n”);
       return(0);
   }
   return(1);
}
