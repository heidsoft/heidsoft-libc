void init_sockaddr(struct sockaddr_in *name, const char *hostname, 
                     const char *serv)
{
   struct hostent *hp;
   char *host, myname[255];
   if (hostname == NULL){
      gethostname(myname, sizeof(myname));
      host = myname;
   }
   else
      host = hostname;
   /* 取得主電腦位址訊息 */
   if ((hp = gethostbyname(host)) == NULL){
      printf("Unknown host: %s\n", host);
      exit(-1);
   }
   /* 填入伺服器的IP位址和通訊埠 */
   bzero(name,sizeof(struct sockaddr));
   if (hp->h_addrtype == AF_INET){   
      name->sin_family = AF_INET;
      bcopy(hp->h_addr_list[0], &name->sin_addr, hp->h_length);
      if (serv == NULL)
         name->sin_port = htons(0); /* 由系統選取一個適當的通訊埠 */
      else
         name->sin_port = htons(atoi(serv));
   } else {
      printf("Unknown address type\n");
      exit(-1);
   }
}
