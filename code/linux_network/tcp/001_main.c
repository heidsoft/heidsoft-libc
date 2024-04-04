#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

// 这是一个伪头部，用于TCP校验和计算
struct pseudo_header
{
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;
};

unsigned short csum(unsigned short *ptr,int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
    
    return(answer);
}

int main (void)
{
    // 创建一个原始套接字
    int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    
    // 一些地址结构
    char datagram[4096] , source_ip[32] , *data , *pseudogram;
    
    // 零化数据报
    memset (datagram, 0, 4096);
    
    // 你自己的IP
    strcpy(source_ip , "10.211.55.55");
    
    struct sockaddr_in sin;
    struct pseudo_header psh;
    
    // 数据部分
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    
    // IP头部
    struct iphdr *iph = (struct iphdr *) datagram;
    
    // TCP头部
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof (struct ip));
    
    // 填充IP头部
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct ip) + sizeof (struct tcphdr) + strlen(data);
    iph->id = htonl (54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;
    iph->saddr = inet_addr ( source_ip );
    iph->daddr = sin.sin_addr.s_addr;
    
    // TCP头部
    tcph->source = htons (1234);
    tcph->dest = htons (80);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->syn = 1;
    tcph->window = htons (5840);
    tcph->check = 0;
    tcph->urg_ptr = 0;
    
    // IP校验和
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
    
    // 填充伪头部
    psh.source_address = inet_addr( source_ip );
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(20);
    
    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr);
    pseudogram = malloc(psize);
    
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , tcph , sizeof(struct tcphdr));
    
    tcph->check = csum( (unsigned short*) pseudogram , psize);
    
    // 发送数据报
    if (sendto (s, datagram, iph->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
    {
        perror("sendto failed");
    }
    else
    {
        printf ("Packet Send. Length : %d \n" , iph->tot_len);
    }
    
    return 0;
}