#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <netdb.h>
#include <math.h>

#define PortNum_SerA  21526
#define PortNum_SerB  22526
#define PortNum_SerC  23526
#define PortNum_SerD  24526
#define PortNum_Client  25526
 
#define QUEUE   20
#define BUFFER_SIZE 1024
/*#define MAXLINE 4096
char *trim(const char *str)
{
    static char line[MAXLINE];
    const char *pbegin;
    char *p,*pend;
    int i;
    
    pbegin=str;
    while (*pbegin==' ')
        pbegin++;
    
    p=line;
    while (*p=*pbegin){
        if ((*p==' ')&&(*(p-1)!=' '))
            pend=p;
        p++;pbegin++;
    }
    if (*(p-1)!=' ') pend=p;
    *pend=0;
    return line;
}   */
struct neigh_link 
  {
      char serv_neigh[20];
      int link_cost; 
  };
       struct neigh_link neig_lnk[4];
       struct neigh_link edge_lnk[6];
int main()
{

    //TCP client
      char sendbuf[BUFFER_SIZE];
      char recvbuf[BUFFER_SIZE];
      
      //const char str_title3[100]="The server B finishing sending its neighbor infomation to the Client with TCP port number ";

      memset(sendbuf,0,sizeof sendbuf);
      memset(neig_lnk,0,sizeof neig_lnk);
      memset(edge_lnk,0,sizeof edge_lnk);
      
    /* server is running  */ 
      printf("The server B is up and running\n");
      //puts(str_start);
    /*reading the file containing its neighbor information */
    FILE *fp=NULL;
    char str[30];
    int i=0;                           //use to remember the number of line in file 
 
    fp= fopen("serverB.txt", "r");   //opening file for reading 
    if(fp == NULL) 
     {
        perror("Error opening file");
     }

    
    printf("The server B has following neighbor information:\n");
    printf("Neighbor --- Cost\n");
    while(fgets(str,100,fp)!= NULL)
     {
        
        const char s[] = " ";
        char *token;
        char *token_trim;
        const char s_2[] ="-";
        int flag=0;
       
        token = strtok(str, s);      //get the first token  
        while( token != NULL )       //walk through other tokens
         {  
           if(flag==0)                              //store the context of file in structure  neigh_link
           { 
             
             strcat(sendbuf,token);
             strcat(sendbuf,s_2);
             strcpy(neig_lnk[i].serv_neigh,token);
             //printf( " %s\n", token );
             token = strtok(NULL, s);
             flag=1 ;
           }
           else 
           { 
             
             int a;
             a = atoi(token);
             sprintf(token_trim,"%d",a);
             strcat(sendbuf,token_trim);
             strcat(sendbuf,s_2);
 	           neig_lnk[i].link_cost = a;
             //printf( " %s\n", token );
             token = strtok(NULL, s);            
             flag =0 ;
           }     
         }
          i++;
      }   
    strcat(sendbuf,"B");
     fclose(fp);                 //finish reading the file 

     int j=0;
     const char *pt_space="      ";
     while(j<i)                                   //print the neighbor information
     {
      printf("%s",neig_lnk[j].serv_neigh);
      printf("%s",pt_space);
      printf("%d\n",neig_lnk[j].link_cost); 
      j++;
     }

     /* j=0;     //transfer neighbor link to edge link 
     while(j<i)
     {
       if(strcmp(neig_lnk[j].serv_neigh,"serverB")==0)
         {
           strcpy(edge_lnk[j].serv_neigh,"AB");
           edge_lnk[j].link_cost= neig_lnk[j].link_cost;
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverC")==0)
         {
           //edge_lnk[j].serv_neigh="AC";
           strcpy(edge_lnk[j].serv_neigh,"AC");
           edge_lnk[j].link_cost= neig_lnk[j].link_cost;
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverD")==0)
         {
           //edge_lnk[j].serv_neigh="AD";
           strcpy(edge_lnk[j].serv_neigh,"AD");
           edge_lnk[j].link_cost= neig_lnk[j].link_cost;    
         }
       j++;
     } 
                


   j=0;

   
     while(j<i)                                   //print the neighbor information
     {
      printf("%s",edge_lnk[j].serv_neigh);
      printf("%s",pt_space);
      printf("%d\n",edge_lnk[j].link_cost); 
      j++;
     }                   */
    /* send the information to the clinet via TCP */
     

    //printf("%s\n",sendbuf);
    printf("%s\n",sendbuf);
    //strcat(sendbuf,"exit\n");
    //printf("%s","The server B finishing sending its neighbor infomation to the Client with TCP port number ");
    //printf("%d\n",PortNum_Client);

    char host[]="localhost";
    struct hostent *he;
    struct sockaddr_in sockaddr_serv_tcp;                     // define sockaddr_in
    struct sockaddr_in sockaddr_cli_tcp;
    unsigned int len_cli;
    int tcpsockfd_cli = socket(AF_INET,SOCK_STREAM, 0); 

    memset((char *)&sockaddr_cli_tcp, 0, sizeof(sockaddr_cli_tcp));
    sockaddr_cli_tcp.sin_family = AF_INET;
    sockaddr_cli_tcp.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr_cli_tcp.sin_port = htons(0);

    if (bind(tcpsockfd_cli, (struct sockaddr *)&sockaddr_cli_tcp, sizeof(sockaddr_cli_tcp)) < 0)
     {
    perror("bind failed");
    return 0;
     }
     len_cli=sizeof(sockaddr_cli_tcp);
    if (getsockname(tcpsockfd_cli, (struct sockaddr *)&sockaddr_cli_tcp, &len_cli) < 0) 
    {
                perror("getsockname failed");
                return 0;
      }
    // printf("local port number = %d\n", ntohs(sockaddr_cli_tcp.sin_port));

    memset(&sockaddr_serv_tcp, 0, sizeof(sockaddr_serv_tcp));
    sockaddr_serv_tcp.sin_family = AF_INET;
    sockaddr_serv_tcp.sin_port = htons(PortNum_Client);      //port number of TCP server
    if ((he=gethostbyname(host)) == NULL)
       {
         fprintf(stderr, "could not obtain address of %s\n", host);
          return 0;
        }
    
    memcpy(&sockaddr_serv_tcp.sin_addr, he->h_addr_list[0], he->h_length);
    
    //printf("%s\n",inet_ntoa(sockaddr_serv_tcp.sin_addr));

    //sockaddr_serv_tcp.sin_addr.s_addr = inet_addr("127.0.0.1");  // old version, use hardcode IP address

     
    if (connect(tcpsockfd_cli, (struct sockaddr *)&sockaddr_serv_tcp, sizeof(sockaddr_serv_tcp)) < 0)
    {
        perror("connect");
        exit(1);
    }
        
        send(tcpsockfd_cli, sendbuf, strlen(sendbuf),0);             //sending 
        
        recv(tcpsockfd_cli, recvbuf, sizeof(recvbuf),0);             //receiving
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
  

    close(tcpsockfd_cli);

/*    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        send(tcpsockfd_cli, sendbuf, strlen(sendbuf),0); ///sending 
        if(strcmp(sendbuf,"exit\n")==0)
            break;
        recv(tcpsockfd_cli, recvbuf, sizeof(recvbuf),0); ///receiving
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(tcpsockfd_cli);    */
   



     /*after sending TCP    */
         
     printf("The server B finishing sending its neighbor infomation to the Client with TCP port number ");
     printf("%d",PortNum_Client);
     printf (" and IP address %s\n",inet_ntoa(sockaddr_serv_tcp.sin_addr));
     printf("\n");
     printf("For this connection with the Client, the Server B has TCP port number %d and IP address %s \n",
                     ntohs(sockaddr_cli_tcp.sin_port),inet_ntoa(sockaddr_serv_tcp.sin_addr));



  /*  //UDP server 
     //define sockfd
    int server_sockfd_udp = socket(AF_INET,SOCK_DGRAM, 0);  //create UDP socket

    //define sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;  //address family
    server_sockaddr.sin_port = htons(PortNum); // address port number
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Internet address   

    //bind socket
    if(bind(server_sockfd_udp,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    //listen,success reture 0, fail return -1
    if(listen(server_sockfd_udp,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    //socket
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    // 
    int conn = accept(server_sockfd_udp, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer),0);
        if(strcmp(buffer,"exit\n")==0)
            break;
        fputs(buffer, stdout);
        send(conn, buffer, len, 0);
    }
    close(conn);
    close(server_sockfd_udp);*/
 
    return 0;
}




