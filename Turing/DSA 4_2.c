#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Pismenka{
  struct Pismenka *next[26];
  char koniec;
};
int dajStrom(char vstup[],int dlzka, struct Pismenka *prvy, int max){
  int i, pomocc=0;;
  struct Pismenka *akt=prvy;
    for(i=0;i<dlzka;i++){
      if(akt->next[(int)((vstup[i]-26)%39)]!=NULL){ 
        akt=akt->next[(int)((vstup[i]-26)%39)];
        pomocc++;
      }else{
        akt->next[(int)((vstup[i]-26)%39)]=(struct Pismenka*)malloc(sizeof(struct Pismenka));
        akt=akt->next[(int)((vstup[i]-26)%39)];
        akt->koniec='n';
      }
    }
    akt->koniec='y';
    if(pomocc>max)max=pomocc;
    return max;
}
int main()
{
  char vstup[50];
  struct Pismenka *prvy=(struct Pismenka*)malloc(sizeof(struct Pismenka));
  int max=0;
  while((scanf("%s",&vstup))!=EOF){
    max=dajStrom(vstup,strlen(vstup), prvy,max);
  }
  printf("%d\n",max);
  return 0;
}
