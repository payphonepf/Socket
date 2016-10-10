// v 5.0 establish udp client
// v 5.1 coding the udp server 
// v 7.0 Dijkstra algorithm  
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

#define PortNum_SerA  21526
#define PortNum_SerB  22526
#define PortNum_SerC  23526
#define PortNum_SerD  24526
#define PortNum_Client  25526

#define BUFFER_SIZE 1024
#define QUEUE   100  // the size of buffer
#define node 4      // the number of node in the network 
char buffer[BUFFER_SIZE];
char sendbuf[BUFFER_SIZE];
struct neigh_link 
  {
      char serv_neigh[20];
      int link_cost; 
  };
struct neigh_link neig_lnk[10];  //5 
struct neigh_link edge_lnk[10];

  typedef struct
{
 int bgnvex,endvex;
 int length; // 
}edge;


int num = 5 ; // current number of connection in edge_lnk  
int tcp_rec();
int udp_sed(int port ,char *a);
void print(int i);
void transferA(int i);
void transferB(int i);
void transferC(int i );
void transferD(int i);
int mst();

void prim(int dist[][node],edge T[]);
void print_mst(int x,int y,int z);

int main()
{
    strcpy(edge_lnk[0].serv_neigh,"AB");
    strcpy(edge_lnk[1].serv_neigh,"AC");
    strcpy(edge_lnk[2].serv_neigh,"AD");
    strcpy(edge_lnk[3].serv_neigh,"BC");
    strcpy(edge_lnk[4].serv_neigh,"BD");
    strcpy(edge_lnk[5].serv_neigh,"CD");
    int port_udp_A = PortNum_SerA;
    int port_udp_B = PortNum_SerB;
    int port_udp_C = PortNum_SerC;
    int port_udp_D = PortNum_SerD;

    memset(sendbuf,0,sizeof sendbuf);
    printf("The Client have TCP port number %d , and IP address %s\n ",  PortNum_Client,"127.0.0.1");
    if(tcp_rec()!= 0)
    {
      printf("Tcp receiving error...\n");
    }
    
    //printf("%s\n",sendbuf);

    if(udp_sed(port_udp_A,"A")!=0)
    {
      printf("Fail to send topology to serverA\n");
    }

    if(udp_sed(port_udp_B,"B")!=0)
    {
      printf("Fail to send topology to serverB\n");
    }

    if(udp_sed(port_udp_C,"C")!=0)
    {
      printf("Fail to send topology to serverC\n");
    }

    if(udp_sed(port_udp_D,"D")!=0)
    {
      printf("Fail to send topology to serverD\n");
    }
    
    if(mst()!=0)
    {
       printf("Fail to caculate the minimum spanning tree \n");
    }
    return 0;
}


int tcp_rec()
{
    // TCP server  
   //define sockaddr_in
    struct sockaddr_in sockaddr_serv_tcp;   //server address
    struct sockaddr_in sockaddr_client_tcp;   //client address 
    int tcpsockfd_ser;
    int tcpsockfd_cli;
    char hostname[128];

   
    gethostname(hostname, 128);

    //define sockfd
    if ((tcpsockfd_ser = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
            perror("cannot create socket");
            exit(1);
        }

    memset((char*)&sockaddr_serv_tcp, 0, sizeof(sockaddr_serv_tcp));
    sockaddr_serv_tcp.sin_family = AF_INET;
    sockaddr_serv_tcp.sin_port = htons(PortNum_Client);
    sockaddr_serv_tcp.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket
    if(bind(tcpsockfd_ser,(struct sockaddr *)&sockaddr_serv_tcp,sizeof(sockaddr_serv_tcp))==-1)
    {
        perror("bind failed");
        exit(1);
    }

    //listen,success reture 0, fail return -1
    int n;
    for (n=0;n<4;n++)
    {
      if(listen(tcpsockfd_ser,QUEUE) == -1)
        {
            perror("listen failed");
            exit(1);
        }

        //printf("The client have TCP port number %d , and IP address %s\n ",  PortNum_Client,"127.0.0.1");

        char buffer[BUFFER_SIZE];
        memset(buffer,0,sizeof buffer);
        socklen_t length = sizeof(sockaddr_client_tcp);
       
      
        tcpsockfd_cli = accept(tcpsockfd_ser, (struct sockaddr*)&sockaddr_client_tcp, &length);
        while(tcpsockfd_cli<0)
        {
         
        }
        read(tcpsockfd_cli, buffer, BUFFER_SIZE);



        //printf("received message: \"%s\"\n", buffer);
        
        char *token;
        char *token_trim;

        int i =0;
        token = strtok(buffer, "-");      //get the first token  
            while( token != NULL )       //walk through other tokens
             {  
                 strcpy(neig_lnk[i].serv_neigh,token);

                 //printf( " %s\n", token );
                 token = strtok(NULL, "-");


                 if(token != NULL)
                 {
                 int a;
                 a = atoi(token);
                 
                 neig_lnk[i].link_cost = a;
                 //printf( " %s\n", token );
                 token = strtok(NULL, "-");   
                 i=i+1;
                 }
               }     

        if(strcmp(neig_lnk[i].serv_neigh,"A")==0) 
        {
            printf("\nThe Client receivers neighbor information from the Server A with TCP port number port %d and IP address %s \n",
                         ntohs(sockaddr_client_tcp.sin_port),inet_ntoa(sockaddr_client_tcp.sin_addr));
            printf("\nThe Server A has the following neighbor information:\n");
            print(i);
            printf("\nFor this connection with Server A, The Client has TCP port number %d and IP address %s\n", PortNum_Client,"127.0.0.1");
            printf("\n" );
            transferA(i);
      
        }   

        if(strcmp(neig_lnk[i].serv_neigh,"B")==0) 
        {
            printf("The Client receivers neighbor information from the Server B with TCP port number port %d and IP address %s \n",
                         ntohs(sockaddr_client_tcp.sin_port),inet_ntoa(sockaddr_client_tcp.sin_addr));
            printf("The Server B has the following neighbor information:\n");
            print(i);
            printf("\nFor this connection with Server B, The Client has TCP port number %d and IP address %s \n", PortNum_Client,"127.0.0.1");
            printf("\n" );
           transferB(i); 
        } 

        if(strcmp(neig_lnk[i].serv_neigh,"C")==0) 
        {
            printf("The Client receivers neighbor information from the Server C with TCP port number port %d and IP address %s \n",
                         ntohs(sockaddr_client_tcp.sin_port),inet_ntoa(sockaddr_client_tcp.sin_addr));
            printf("The Server C has the following neighbor information:\n");
            print(i);
            printf("\nFor this connection with Server C, The Client has TCP port number %d and IP address %s\n", PortNum_Client,"127.0.0.1");
             printf("\n" );
            transferC(i);
         }

        if(strcmp(neig_lnk[i].serv_neigh,"D")==0) 
        {
            printf("The Client receivers neighbor information from the Server D with TCP port number port %d and IP address %s \n",
                         ntohs(sockaddr_client_tcp.sin_port),inet_ntoa(sockaddr_client_tcp.sin_addr));
            printf("The Server D has the following neighbor information:\n");
            print(i);
            printf("\nFor this connection with Server D, The Client has TCP port number %d and IP address %s\n", PortNum_Client,"127.0.0.1");
            printf("\n");
            transferD(i);
        } 
         memset(neig_lnk,0,sizeof neig_lnk);
         memset(buffer,0,sizeof buffer);

         close(tcpsockfd_cli);
    }
    




//printf("Edge --- Cost\n");
    int j=0;
     const char *pt_space="        ";
     char *str_cost;
     while(j<num+1)                                   //print the neighbor information

     {
      if(edge_lnk[j].link_cost!=0)
      {
        //printf("%s",edge_lnk[j].serv_neigh);
        strcat(sendbuf,edge_lnk[j].serv_neigh);
        strcat(sendbuf,"-");
        //printf("%s",pt_space);
        //printf("%d\n",edge_lnk[j].link_cost); 
        int a=edge_lnk[j].link_cost;
        sprintf(str_cost,"%d",a);
        strcat(sendbuf,str_cost);
        strcat(sendbuf,"-");
      }
        j++;
     }


    close(tcpsockfd_cli);
    close(tcpsockfd_ser);
    return 0;

}



void print(int i)
{
    printf("\n");
    printf("Neighbor --- Cost\n");
    int j=0;
     const char *pt_space="      ";
     while(j<i)                                   //print the neighbor information
     {
      printf("%s",neig_lnk[j].serv_neigh);
      printf("%s",pt_space);
      printf("%d\n",neig_lnk[j].link_cost); 
      j++;
     }
}

void transferA(int i)
{
int j=0;     //transfer neighbor link to edge link 
        int m;
        while(j<i)
     {
       if(strcmp(neig_lnk[j].serv_neigh,"serverB")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"AB")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverC")==0)
         {
         for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"AC")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverD")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"AD")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }    
         }
       j++;
     } 
}

void transferB(int i)
{
int j=0;     //transfer neighbor link to edge link 
        int m;
        while(j<i)
     {
       if(strcmp(neig_lnk[j].serv_neigh,"serverA")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"AB")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverC")==0)
         {
         for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"BC")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverD")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"BD")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }    
         }
       j++;
     } 
}

void transferC(int i)
{
int j=0;     //transfer neighbor link to edge link 
        int m;
        while(j<i)
     {
       if(strcmp(neig_lnk[j].serv_neigh,"serverA")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"AC")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverB")==0)
         {
         for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"BC")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverD")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"CD")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }    
         }
       j++;
     } 
}

void transferD(int i)
{
int j=0;     //transfer neighbor link to edge link 
        int m;
        while(j<i)
     {
       if(strcmp(neig_lnk[j].serv_neigh,"serverA")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"AD")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverB")==0)
         {
         for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"BD")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }
           
         }
       if(strcmp(neig_lnk[j].serv_neigh,"serverC")==0)
         {
           for(m=0;m<num+1;m++)
           { 
             if(strcmp(edge_lnk[m].serv_neigh,"CD")==0)
             edge_lnk[m].link_cost= neig_lnk[j].link_cost;
           }    
         }
       j++;
     } 
}

// create a udp client, send the network topology to ServerX
int udp_sed(int port, char *a)
{ 
    struct sockaddr_in sockaddr_serv_udp;                     
    struct sockaddr_in sockaddr_cli_udp;
    char *server = "127.0.0.1";
    int udpsockfd_cli ;  
    int len_cli;
    //printf("%s\n",a );

    //define udp client sockaddr_cli_udp 
    //creating a socket 
    if ((udpsockfd_cli=socket(AF_INET, SOCK_DGRAM, 0))==-1)
      {
        printf("socket created\n");
      }
    //binding to local address and dynamic port number
    memset((char *)&sockaddr_cli_udp, 0, sizeof(sockaddr_cli_udp));
    sockaddr_cli_udp.sin_family = AF_INET;
    sockaddr_cli_udp.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr_cli_udp.sin_port = htons(0);
     
    if (bind(udpsockfd_cli, (struct sockaddr *)&sockaddr_cli_udp, sizeof(sockaddr_cli_udp)) < 0) 
    {
       printf("bind failed");
    } 
        len_cli=sizeof(sockaddr_cli_udp);

    if (getsockname(udpsockfd_cli, (struct sockaddr *)&sockaddr_cli_udp, &len_cli) < 0) 
    {
                perror("getsockname failed");
                return 0;
      }
  /* printf("For this connection with Server A, The Client has UDP port number %d and IP address %s \n",
                         ntohs(sockaddr_cli_udp.sin_port),inet_ntoa(sockaddr_cli_udp.sin_addr));   */

    //define udp server 
    memset((char *) &sockaddr_serv_udp, 0, sizeof(sockaddr_serv_udp));
    sockaddr_serv_udp.sin_family = AF_INET;
    sockaddr_serv_udp.sin_port = htons(port);
    if (inet_aton(server, &sockaddr_serv_udp.sin_addr)==0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    printf("\n");
    printf("The Client has sent the network topology to the network topology to the Server %s with UDP port number ",a);
    printf("%d and IP address %s ",port, server);
    printf("(Server %s’s UDP port number and IP address) as follows: \n",a);


    printf("\nEdge --- Cost\n");
    int j=0;
     const char *pt_space="        ";
     char *str_cost;
     while(j<num+1)                                   //print the neighbor information

     {
      if(edge_lnk[j].link_cost!=0)
      {
        printf("%s",edge_lnk[j].serv_neigh);
        printf("%s",pt_space);
        printf("%d\n",edge_lnk[j].link_cost); 
      }
        j++;
     }


    if(sendto(udpsockfd_cli, sendbuf, strlen(sendbuf),0,(struct sockaddr*)&sockaddr_serv_udp,sizeof(sockaddr_serv_udp)) < 0) 
      { 
        printf("Send Network Topology Failed.."); 
        exit(1); 
      }
      printf("\n");
      printf("For this connection with Server %s", a);
      //printf(", The Client has UDP port number %d and IP address %s \n",ntohs(sockaddr_cli_udp.sin_port),inet_ntoa(sockaddr_cli_udp.sin_addr));
      printf(", The Client has UDP port number %d and IP address %s \n",ntohs(sockaddr_cli_udp.sin_port),server);
      close(udpsockfd_cli);

    return 0;
}   
//using Prim's algorithm to caluculate minimum spanning tree 
int mst()
{  
   int matrix[4][4] ={{1000,1000,1000,1000},{1000,1000,1000,1000},{1000,1000,1000,1000},{1000,1000,1000,1000}}; 
   //1000 represent the link between the node is not accessable 




//transfer the store information into matrix form 
    if(edge_lnk[0].link_cost==0)
    {
      matrix[0][1]=1000;
      matrix[1][0]=1000;
    }
    else 
    {
      matrix[0][1]=edge_lnk[0].link_cost ;
      matrix[1][0]=edge_lnk[0].link_cost ; 
    }
    if(edge_lnk[1].link_cost==0)
    {
      matrix[0][2]=1000;
      matrix[2][0]=1000;
    }
    else
    {
      matrix[0][2]=edge_lnk[1].link_cost ;
      matrix[2][0]=edge_lnk[1].link_cost ;
    }
    if(edge_lnk[2].link_cost==0)
    {
      matrix[0][3]=1000;
      matrix[3][0]=1000;
    }
    else
    {
      matrix[0][3]=edge_lnk[2].link_cost ;
      matrix[3][0]=edge_lnk[2].link_cost ;
    }
   if(edge_lnk[3].link_cost==0)
   {
      matrix[1][2]=1000;
      matrix[2][1]=1000;
   }
   else
   {
    matrix[1][2]=edge_lnk[3].link_cost ;
   matrix[2][1]=edge_lnk[3].link_cost ;
   }
   if(edge_lnk[4].link_cost==0)
   {
      matrix[1][3]=1000;
      matrix[3][1]=1000;
   }
   else
   {
    matrix[1][3]=edge_lnk[4].link_cost ;
    matrix[3][1]=edge_lnk[4].link_cost ;
   }
   
   if(edge_lnk[5].link_cost==0)
   {
      matrix[2][3]=1000;
      matrix[3][2]=1000;
   }
   else
   {
    matrix[2][3]=edge_lnk[5].link_cost ;
   matrix[3][2]=edge_lnk[5].link_cost ;

   }
   

  int i;
  int j;
  /*for(i=0;i<4;i++)
  {
     for(j=0;j<4;j++){
         printf("%d  ",matrix[i][j]);
     }
      printf("\n");
  }    */


  edge T[node-1]; 
  prim(matrix,T);
  int sum=0;
  for(i=0;i<node-1;i++)
  {
      sum= sum +T[i].length;
  }
 printf("\nThe Client has calculated a tree. The tree cost is %d\n", sum);
 printf("\nEdge -----Cost\n");

 for(i=0;i<node-1;i++)

   {
     //printf("\t(%d,%d) %d\n",T[i].bgnvex,T[i].endvex,T[i].length);
     int x;
     int y;
     int z;
     x=T[i].bgnvex;
     y= T[i].endvex;
     z=T[i].length;

     print_mst(x,y,z);
   }


  

   return 0;
}
void prim(int dist[][node],edge T[])
{
 int j,k,m,v,min,max=10000;
 int d;
 edge e;
 for(j=1;j<node;j++)  
 {
  T[j-1].bgnvex=1;  
  T[j-1].endvex=j+1;
  T[j-1].length=dist[0][j];
 }
 for(k=0;k<node-1;k++)
 {
  min=max;
  for(j=k;j<node-1;j++)
  {
   if(T[j].length<min)
   {
    min=T[j].length;
    m=j;
   }
  }
  e=T[m];T[m]=T[k];T[k]=e;
  v=T[k].endvex;
  for(j=k+1;j<node-1;j++)
  {
   d=dist[v-1][T[j].endvex-1];
   if(d<T[j].length)
   {
    T[j].length=d;
    T[j].bgnvex=v;
   }
  }
 }
}

void print_mst(int x,int y,int z)
{
    
   if (x ==1)
   {
     if(y==2)
     {
        printf("%s       %d\n", "AB",z);
     }
     if(y==3)
     {
        printf("%s       %d\n", "AC",z);
     }
     if(y==4)
     {
        printf("%s       %d\n", "AD",z);
     }
   }
  if (x==2)
  {
     if(y ==1)
     {
      printf("%s       %d\n", "AB",z);
     }
     if(y==3)
     {
       printf("%s       %d\n", "BC",z);
     }
     if(y ==4)
     {
       printf("%s       %d\n", "BD",z);
     }
  }
  if (x==3)
  {
     if(y==1)  
     {
      printf("%s       %d\n", "AC",z);
     } 
     if(y==2)
     {
      printf("%s       %d\n", "BC",z);
     } 
     if(y ==4)
     {
       printf("%s       %d\n", "CD",z);
     }
  }
  if(x==4)
  {
     if(y==1)
     {
       printf("%s       %d\n", "AD",z);
     }
     if(y==2)
     {
       printf("%s       %d\n", "BD",z);
     }
     if(y==3)
     {
       printf("%s       %d\n", "CD",z);
     }
  } 
}


