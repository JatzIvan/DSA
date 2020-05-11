// uloha8-2.c -- Ivan Jatz, 13.11.2019 13:32

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

struct Vrchol{
  double vzdialenost;
  int vrch;
  int heaplok;
};

void heapyfi(struct Vrchol *hrany, int *heapL, int miesto){
  while(1){
    if(hrany[heapL[(miesto-1)/2]].vzdialenost==hrany[heapL[miesto]].vzdialenost)break;
    if(hrany[heapL[(miesto-1)/2]].vzdialenost>hrany[heapL[miesto]].vzdialenost){
      int pom=heapL[miesto];
      hrany[heapL[miesto]].heaplok=(miesto-1)/2;
      hrany[heapL[(miesto-1)/2]].heaplok=miesto;
      heapL[miesto]=heapL[(miesto-1)/2];
      heapL[(miesto-1)/2]=pom;
      miesto=(miesto-1)/2;
    }else{
      break;
    }
    if(miesto==0)break;
  }
}
void heapOP(int *heapL,int HEAPDL,struct Vrchol *hrany){
  int i=0;
  while(1){
    //printf("SSSS\n");
    if(((i+1)*2>=HEAPDL)&&((i+1)*2-1>=HEAPDL))break;
    if((((i+1)*2<HEAPDL)&&(hrany[heapL[(i+1)*2]].vzdialenost)<(hrany[heapL[i]].vzdialenost))){
      if(((i+1)*2-1<HEAPDL)&&((hrany[heapL[(i+1)*2-1]].vzdialenost)<(hrany[heapL[(i+1)*2]].vzdialenost))){
        int pomoc=heapL[i];
        hrany[heapL[i]].heaplok=(i+1)*2-1;
        hrany[heapL[(i+1)*2-1]].heaplok=i;
        heapL[i]=heapL[(i+1)*2-1];
        heapL[(i+1)*2-1]=pomoc;
        i=(i+1)*2-1;
      }else{
        int pomoc=heapL[i];
        hrany[heapL[i]].heaplok=(i+1)*2;
        hrany[heapL[(i+1)*2]].heaplok=i;
        heapL[i]=heapL[(i+1)*2];
        heapL[(i+1)*2]=pomoc;
        i=(i+1)*2;
      }
    } if((((i+1)*2-1<HEAPDL)&&((hrany[heapL[(i+1)*2-1]].vzdialenost))<(hrany[heapL[i]].vzdialenost))){
      if(((i+1)*2<HEAPDL)&&((hrany[heapL[(i+1)*2]].vzdialenost)<(hrany[heapL[(i+1)*2-1]].vzdialenost))){
        int pomoc=heapL[i];
        hrany[heapL[i]].heaplok=(i+1)*2;
        hrany[heapL[(i+1)*2]].heaplok=i;
        heapL[i]=heapL[(i+1)*2];
        heapL[(i+1)*2]=pomoc;
        i=(i+1)*2;
      }else{
        int pomoc=heapL[i];
        hrany[heapL[i]].heaplok=(i+1)*2-1;
        hrany[heapL[(i+1)*2-1]].heaplok=i;
        heapL[i]=heapL[(i+1)*2-1];
        heapL[(i+1)*2-1]=pomoc;
        i=(i+1)*2-1;
      }
      }if((((i+1)*2-1<=HEAPDL))&&((hrany[heapL[(i+1)*2-1]].vzdialenost)>=(hrany[heapL[i]].vzdialenost))&&((((i+1)*2<=HEAPDL))&&((hrany[heapL[(i+1)*2]].vzdialenost)>=(hrany[heapL[i]].vzdialenost))))break;
	}
}
double vrat_VZ(int x, int y,int x1, int y1){
  int xx=abs(x-x1),yy=abs(y-y1);
  double sum;
  sum=(xx*xx)+(yy*yy);
  sum=sqrt(sum);
  return sum;
}
int je_cesta(int **Cesty,int z,int tam, int pocCiest){
  int je=0,j;
  for(j=0;j<pocCiest;j++){
    if(((Cesty[j][0]==z+1)&&(Cesty[j][1]==tam+1))||((Cesty[j][0]==tam+1)&&(Cesty[j][1]==z+1))){
      Cesty[j][0]=INT_MAX;
      Cesty[j][1]=INT_MAX;
      je=1;
      break;
    }
  }
  return je;
}
void HEAPP(int *heapL, int pocMiest, struct Vrchol *hrany){
  int i;
  printf("--------------------\n");
  for(i=0;i<pocMiest;i++){
    printf("%lf\n",hrany[heapL[i]].vzdialenost);
  }
  printf("--------------------\n");
}
double daj_vzdialenost(struct Vrchol *hrany,int *heapL, int **Mesta, int **Cesty,int navstivene[],int pocMiest,int pocCiest){
  int aktPoc=0,i;
  double suc;
  int HEAPDL=pocMiest;
  //HEAPP(heapL,HEAPDL,hrany);
  while(aktPoc!=pocMiest){
    int aktVrch=hrany[heapL[0]].vrch;
    suc+=hrany[aktVrch].vzdialenost;
    navstivene[aktVrch]=1;
    heapL[0]=heapL[HEAPDL-1];
    heapOP(heapL,HEAPDL-1,hrany);
    int x=Mesta[aktVrch][0],y=Mesta[aktVrch][1];
    for(i=0;i<pocMiest;i++){
      if(navstivene[i]!=1){
        //printf("%d %d\n",aktVrch,i);
        if(je_cesta(Cesty,aktVrch,i,pocCiest)==1){
          //printf("SOM TU %d %d\n",aktVrch,i);
          hrany[i].vzdialenost=0;
          heapyfi(hrany,heapL,hrany[i].heaplok);
        }
        else{
        double pomoccc=vrat_VZ(x,y,Mesta[i][0],Mesta[i][1]);
        if(pomoccc < hrany[i].vzdialenost){
          hrany[i].vzdialenost=pomoccc;
          heapyfi(hrany,heapL,hrany[i].heaplok);
        }
        //printf("VZDIALENOST %lf\n",hrany[i].vzdialenost);
        }
        //heapyfi(hrany,heapL,hrany[i].heaplok);
      }
    }
    //HEAPP(heapL,HEAPDL,hrany);
    aktPoc++;
    HEAPDL--;
  }
  return suc;
}

int main()
{
  int pocMiest,i,pocCiest;
  scanf("%d",&pocMiest);
  int navstivene[pocMiest];
  int **Mesta=(int**)malloc((pocMiest+1)*sizeof(int*));
  struct Vrchol *hrany=(struct Vrchol*)malloc(pocMiest*sizeof(struct Vrchol));
  int *heapL=(int*)malloc(pocMiest*sizeof(int));
  for(i=0;i<pocMiest;i++){
    navstivene[i]=0;
    Mesta[i]=(int*)malloc(2*sizeof(int));
    scanf("%d %d",&Mesta[i][0],&Mesta[i][1]);
    hrany[i].vzdialenost=INT_MAX;
    hrany[i].vrch=i;
    hrany[i].heaplok=i;
    heapL[i]=i;
  }
  hrany[0].vzdialenost=0;
  scanf("%d",&pocCiest);
  int **Cesty=(int**)malloc((pocCiest+1)*sizeof(int*));
  for(i=0;i<pocCiest;i++){
    Cesty[i]=(int*)malloc(2*sizeof(int));
    scanf("%d %d",&Cesty[i][0],&Cesty[i][1]);
  }
  printf("%.3lf",daj_vzdialenost(hrany,heapL,Mesta,Cesty,navstivene,pocMiest,pocCiest));

  return 0;
}
