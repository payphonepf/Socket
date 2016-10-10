#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <netdb.h>

#define node 4      // the number of node in the network 
struct neigh_link  
  {
      char serv_neigh[20];
      int link_cost; 
  }  ;

  typedef struct
{
 int bgnvex,endvex;
 int length; // 
}edge;

void prim(int dist[][node],edge T[]);
void print_mst(int x,int y,int z);


int main()
{
 
	struct neigh_link edge_lnk[10];
	struct neigh_link neig_lnk[5];
   int matrix[4][4] ={{1000,1000,1000,1000},{1000,1000,1000,1000},{1000,1000,1000,1000},{1000,1000,1000,1000}}; 
   //1000 represent the link between the node is not accessable 


    strcpy(edge_lnk[0].serv_neigh,"AB");
    strcpy(edge_lnk[1].serv_neigh,"AC");
    strcpy(edge_lnk[2].serv_neigh,"AD");
    strcpy(edge_lnk[3].serv_neigh,"BC");
    strcpy(edge_lnk[4].serv_neigh,"BD");
    strcpy(edge_lnk[5].serv_neigh,"CD");
    edge_lnk[0].link_cost = 20;
    edge_lnk[1].link_cost = 10;
    edge_lnk[2].link_cost = 5;
    edge_lnk[3].link_cost = 30;
    edge_lnk[4].link_cost = 15;
    edge_lnk[5].link_cost = 50;

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
  for(i=0;i<4;i++)
  {
     for(j=0;j<4;j++){
         printf("%d  ",matrix[i][j]);
     }
      printf("\n");
  }


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
