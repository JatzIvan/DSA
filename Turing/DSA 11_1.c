// uloha11-1.c -- Ivan Jatz, 20.12.2019 17:15

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
double xcentre(int **suradnice, int pocet){
  double a,gx,t;
  a=0.0;
  int i,ij=1;
  for(i=0;i<pocet;i++){
    a+=suradnice[i][0]*suradnice[ij][1]-suradnice[ij][0]*suradnice[i][1];
    ij=(ij+1)%pocet;
  }
  a*=0.5;
  gx=0.0;
  ij=1;
  for(i=0;i<pocet;i++){
    t=suradnice[i][0]*suradnice[ij][1]-suradnice[ij][0]*suradnice[i][1];
    gx+=(suradnice[i][0]+suradnice[ij][0])*t;
    ij=(ij+1)%pocet;
  }
  return gx/(6.0*a);
}
double ycentre(int **suradnice, int pocet){
  double a,gy,t;
  a=0.0;
  int i,ij=1;
  for(i=0;i<pocet;i++){
    a+=suradnice[i][0]*suradnice[ij][1]-suradnice[ij][0]*suradnice[i][1];
    ij=(ij+1)%pocet;
  }
  a*=0.5;
  gy=0.0;
  ij=1;
  for(i=0;i<pocet;i++){
    t=suradnice[i][0]*suradnice[ij][1]-suradnice[ij][0]*suradnice[i][1];
    gy+=(suradnice[i][1]+suradnice[ij][1])*t;
    ij=(ij+1)%pocet;
  }
  return gy/(6.0*a);
}

int moze(int y, int sy1, int sy2){
  if(sy1>sy2){
    if(y<=sy1 && y>=sy2){
      return 1;
    }else{
      return 0;
    }
  }if(sy1<sy2){
      if(y<=sy2 && y>=sy1){
        return 1;
      }else{
        return 0;
      }
  }if((sy1==sy2)&&(sy1==y)){
    return 2;
  }else{
    return 0;
  }
}
int pocetPrechodP(int **suradnice,int pocet, double x, double y){
  int i,pocetPries=0;
  int PREPP=0;
  double BodyX[pocet];
  int poradie=0;
  for(i=0;i<pocet-1;i++){
    int smerx=suradnice[i+1][0]-suradnice[i][0];
    int smery=suradnice[i+1][1]-suradnice[i][1];
    int c=(-1*suradnice[i][0]*smerx)+(-1*suradnice[i][1]*smery);
    int prepinac=moze(y,suradnice[i+1][1],suradnice[i][1]);
    if(prepinac==1){
      if((suradnice[i][1]==y)||((suradnice[i+1][1]==y))){
      BodyX[poradie]=INT_MIN;
      PREPP=1;
      }else{
      BodyX[poradie]=(-1*smery*y-c)/smerx;
      }
    }if(prepinac==0){
      BodyX[poradie]=INT_MIN;
    }if(prepinac==2){
      if(suradnice[i+1][0]>x)BodyX[poradie]=suradnice[i+1][0];
      if(suradnice[i][0]>x)BodyX[poradie]=suradnice[i][0];
    }
    poradie++;
  }
    int smerx=suradnice[pocet-1][0]-suradnice[0][0];
    int smery=suradnice[pocet-1][1]-suradnice[0][1];
    int c=(-1*suradnice[pocet-1][0]*smerx)+(-1*suradnice[pocet-1][1]*smery);
    int prepinac=moze(y,suradnice[0][1],suradnice[pocet-1][1]);
    if(prepinac==1){
      if((suradnice[pocet-1][1]==y)||((suradnice[0][1]==y))){
        BodyX[poradie]=INT_MIN;
        PREPP=1;
      }else{
        BodyX[poradie]=(-1*smery*y-c)/smerx;
      }
    }if(prepinac==0){
      BodyX[poradie]=INT_MIN;
    }if(prepinac==2){
      if(suradnice[0][0]>x)BodyX[poradie]=suradnice[0][0];
      if(suradnice[pocet-1][0]>x)BodyX[poradie]=suradnice[pocet-1][0];
    }
    poradie++;
    for(i=0;i<pocet;i++){
      //printf("%lf %lf\n",BodyX[i],x);
      if(BodyX[i]>=x)pocetPries++;
    }
    if(pocetPries==0&&PREPP==1)pocetPries=1;
    return pocetPries;
}
int pocetPrechodH(int **suradnice,int pocet, double x, double y){
     int i,pocetPries=0;
  int PREPP=0;
  double BodyX[pocet];
  int poradie=0;
  for(i=0;i<pocet-1;i++){
    int smerx=suradnice[i+1][0]-suradnice[i][0];
    int smery=suradnice[i+1][1]-suradnice[i][1];
    int c=(-1*suradnice[i][0]*smerx)+(-1*suradnice[i][1]*smery);
    int prepinac=moze(y,suradnice[i+1][1],suradnice[i][1]);
    if(prepinac==1){
      if((suradnice[i][1]==y)||((suradnice[i+1][1]==y))){
      BodyX[poradie]=INT_MAX;
      PREPP=1;
      }else{
      BodyX[poradie]=(-1*smery*y-c)/smerx;
      }
    }if(prepinac==0){
      BodyX[poradie]=INT_MAX;
    }if(prepinac==2){
      if(suradnice[i+1][0]<x)BodyX[poradie]=suradnice[i+1][0];
      if(suradnice[i][0]<x)BodyX[poradie]=suradnice[i][0];
    }
    poradie++;
  }
    int smerx=suradnice[pocet-1][0]-suradnice[0][0];
    int smery=suradnice[pocet-1][1]-suradnice[0][1];
    int c=(-1*suradnice[pocet-1][0]*smerx)+(-1*suradnice[pocet-1][1]*smery);
    int prepinac=moze(y,suradnice[0][1],suradnice[pocet-1][1]);
    if(prepinac==1){
      if((suradnice[pocet-1][1]==y)||((suradnice[0][1]==y))){
        BodyX[poradie]=INT_MAX;
        PREPP=1;
      }else{
        BodyX[poradie]=(-1*smery*y-c)/smerx;
      }
    }if(prepinac==0){
      BodyX[poradie]=INT_MAX;
    }if(prepinac==2){
      if(suradnice[0][0]<x)BodyX[poradie]=suradnice[0][0];
      if(suradnice[pocet-1][0]<x)BodyX[poradie]=suradnice[pocet-1][0];
    }
    poradie++;
    for(i=0;i<pocet;i++){
      //printf("%lf %lf\n",BodyX[i],x);
      if(BodyX[i]<=x)pocetPries++;
    }
    if(pocetPries==0&&PREPP==1)pocetPries=1;
    return pocetPries;
}

int main()
{
  int pocet,i;
  while(scanf("%d",&pocet)>0){
  int **suradnice=(int**)malloc(pocet*sizeof(int*));
  for(i=0;i<pocet;i++){
    suradnice[i]=(int*)malloc(2*sizeof(int));
    scanf("%d %d",&suradnice[i][0],&suradnice[i][1]);
  }
  double x=xcentre(suradnice,pocet);
  double y=ycentre(suradnice,pocet);
  int pocetP=pocetPrechodP(suradnice,pocet,x,y);
  //printf("-------------------\n");
  int pocetPP=pocetPrechodH(suradnice,pocet,x,y);
  //printf("%d %d\n",pocetP, pocetPP);
  if(pocetP%2==0&&pocetPP%2==0){
    printf("%.3lf %.3lf OUT\n",x,y);
  }else{
    printf("%.3lf %.3lf IN\n",x,y);
  }
  free(suradnice);
  }
  return 0;
}
