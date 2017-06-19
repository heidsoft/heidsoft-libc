int make_socket(int type, unsigned short int port)
{
   int sock;
   struct sockaddr_in name;
   /* 建立套接字 */
   if ((sock = socket(AF_INET, type, 0)) < 0)
      err_exit("socket");
   /* 命名該套接字 */
   name.sin_family = AF_INET;
   name.sin_port = htons (port);
   name.sin_addr.s_addr = htonl (INADDR_ANY);
   if (bind(sock, (struct sockaddr *)&name, (socklen_t)sizeof(name)) < 0)
      err_exit ("bind");
   return sock;
}
