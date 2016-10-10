// v 5.1 udp server 
// each servers are different in ser_id ,udp_port_ser,and read file name 

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


struct neigh_link 
  {
      char serv_neigh[20];
      int link_cost; 
  };

struct neigh_link neig_lnk[4];
struct neigh_link edge_lnk[6];

char sendbuf[BUFFER_SIZE];
char recvbuf[BUFFER_SIZE];

int udp_rec(int port, char *a);
int tcp_sed(int port, char *b);

int main()
{
      
      //const char str_title3[100]="The server B finishing sending its neighbor infomation to the Client with TCP port number ";

      memset(sendbuf,0,sizeof sendbuf);
      memset(neig_lnk,0,sizeof neig_lnk);
      memset(edge_lnk,0,sizeof edge_lnk);

     char *ser_id = "D"; 


    // server is running   
    printf("The server %s is up and running\n",ser_id);

   int tcp_port_cli =PortNum_Client ;
  if(tcp_sed(tcp_port_cli,ser_id)!=0)
  {
    printf("Tcp sending error..");
  }
  
  int udp_port_ser =PortNum_SerD; 
  
  if(udp_rec(udp_port_ser,ser_id)!=0)
  {
    printf("Network Topology Receiving error..\n");
  }


    return 0;
}


int tcp_sed(int port, char *b)
{

   /*reading the file containing its neighbor information */
    FILE *fp=NULL;
    char str[30];                          
    const char s[] = " ";
    char *token;
    char *token_trim;
    const char s_2[] ="-";
    int flag=0; 
    int a = 0;
    int i = 0;
    int count =0 ;
    fp= fopen("serverD.txt", "r");   //opening file for reading 
    if(fp == NULL) 
     {
        perror("Error opening file");
     }

    memset(str,0,sizeof str);
    memset(neig_lnk,0,sizeof neig_lnk);


    while(fgets(str,100,fp)!= NULL)
     {

        token = strtok(str, s);      //get the first token  
        while( token != NULL )       //walk through other tokens
         {  
           if(flag==0)                              //store the context of file in structure  neigh_link
           { 
             
             strcat(sendbuf,token);
             strcat(sendbuf,s_2);
             strcpy(neig_lnk[i].serv_neigh,token);
             //printf("%s\n",neig_lnk[i].serv_neigh );
             //printf( " %s\n", token );
             token = strtok(NULL, s);
             flag=1 ;
           }
           else 
           {    
             a = atoi(token);
             sprintf(token_trim,"%d",a);
             strcat(sendbuf,token_trim);
             strcat(sendbuf,s_2);
             neig_lnk[i].link_cost = a;
             //printf("%d\n",neig_lnk[i].link_cost );
             //printf( " %s\n", token );
             token = strtok(NULL, s);            
             flag =0 ;
           }     
         }
          i++;
      }   
    strcat(sendbuf,b);      //Adding the server id at the end of sendbuf
     fclose(fp);                 //finish reading the file 

    printf("The server %s has following neighbor information:\n",b);
    printf("\nNeighbor --- Cost\n");
     int j=0;
     while(j<i)                                   //print the neighbor information
     {
      printf("%s", neig_lnk[j].serv_neigh);
      printf("      ");
      printf("%d\n", neig_lnk[j].link_cost); 
      j++;
     }

 
    /* send the information to the clinet via TCP */
     

    //printf("%s\n",sendbuf);


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
        
        //recv(tcpsockfd_cli, recvbuf, sizeof(recvbuf),0);             //receiving
        //fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
  

    close(tcpsockfd_cli);

     /*after sending TCP    */
         
     printf("\nThe server %s finishing sending its neighbor infomation to the Client with TCP port number ",b);
     printf("%d ",PortNum_Client);
     printf ("and IP address %s\n",inet_ntoa(sockaddr_serv_tcp.sin_addr));
     printf("\n");
     printf("For this connection with the Client, the Server %s has TCP port number %d and IP address %s \n",b,
                     ntohs(sockaddr_cli_tcp.sin_port),inet_ntoa(sockaddr_serv_tcp.sin_addr));



   return 0;
}

// function for udp reciving 
int udp_rec(int port, char *a)
{
  struct sockaddr_in sockaddr_serv_udp;   //server address
  struct sockaddr_in sockaddr_cli_udp;   //client address 
  int udpsockfd_ser;
 // int udpsockfd_cli;  
  int recvlen;           // bytes received  
  char *host="127.0.0.1";

  //create a udp socket 
  if ((udpsockfd_ser = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("cannot create socket\n");
        exit(1);
    }

  

  memset((char *)&sockaddr_serv_udp,0,sizeof(sockaddr_serv_udp));
  sockaddr_serv_udp.sin_family = AF_INET; 
  sockaddr_serv_udp.sin_addr.s_addr = htonl(INADDR_ANY); 
  sockaddr_serv_udp.sin_port = htons(port);

   if (bind(udpsockfd_ser, (struct sockaddr *)&sockaddr_serv_udp, sizeof(sockaddr_serv_udp)) < 0) 
   {
        perror("bind failed");
        exit(1);
    }
   memset(recvbuf,0,sizeof recvbuf);
   memset((char *)&sockaddr_cli_udp,0,sizeof(sockaddr_cli_udp)); 
   
   socklen_t addrlen = sizeof(sockaddr_cli_udp);   // length of addresses
   //waiting for the topology of network
   for (;;)
   {
     recvlen = recvfrom(udpsockfd_ser, recvbuf, BUFFER_SIZE, 0, (struct sockaddr *)&sockaddr_cli_udp, &addrlen);
     if (recvlen > 0)
      {
            recvbuf[recvlen] = 0;
           // printf("received message: \"%s\" (%d bytes)\n", recvbuf, recvlen);
            break;
       }     
   }
   
   //printf("udp receive:%s\n",recvbuf);
  printf("\n"); 
  printf("The server %s has received the network topology from the Client with UDP port number ",a );
  printf("%d",ntohs(sockaddr_cli_udp.sin_port) );
  printf(" and IP address %s (Client’s UDP port number and IP address) as follows:\n ",inet_ntoa(sockaddr_cli_udp.sin_addr) );

  printf("\n");
  printf("Edge ------ Cost\n");     
  char *token;
  int flag =0 ;

  token = strtok(recvbuf, "-");      //get the first token  
  while( token != NULL )       //walk through other tokens
      {  
          if(flag==0)
          {
          printf( " %s", token );
          printf("          ");
          token = strtok(NULL, "-");
          flag =1;
          }
          else 
          {
           printf( " %s\n", token );
          token = strtok(NULL, "-");
          flag =0;
          }
         
      }     
   
   printf("\n");
   printf("For his connection with Client, The Server %s ",a );
   printf("has UDP port number %d ", port);
   printf("and IP address %s \n", host);
   memset(recvbuf,0,sizeof recvbuf);
   close(udpsockfd_ser);

   return 0;
}



