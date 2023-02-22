void sig_urg(int signo)
{
   int n,skiped=0;
   char buf[100];
   fprintf(stderr, "SIGURG received\n");

   while(1) {
     n = sockatmark(connfd);
     if(n==1) {
        fprintf(stderr, "at OOB mark: ");
        n = recv(connfd,buf,sizeof(buf-1),MSG_OOB);
        if (n<0)
          err_exit("recv error");
        else
          fprintf(stderr, "OOB=%c,",buf[0]);
        fprintf(stderr, "skiped %d bytes before OOB\n",skiped);
        return;
     } else if (n<0)
        err_exit("sockatmark error");
     else {
        n=read(connfd,buf,sizeof(buf)-1);
        skiped=skiped+n;
     }
   }
}

