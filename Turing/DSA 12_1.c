// uloha12-1.c -- Ivan Jatz, 21.12.2019 16:12

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int palindrom(int x,int y, char slovo[], int dlzka){
  int maximum=0;
    if ((x>=0 && y<dlzka)&&(slovo[x]==slovo[y])){
      maximum=palindrom(x-1,y+1,slovo,dlzka);
    }else{
      maximum=y-x-1;
    }
    return maximum;
}

int najdipalindrom(char slovo[],int dlzka){
  int x,y,i,maximum=0;
  for(i=0;i<dlzka;i++){
    int pomoc=palindrom(i,i+1,slovo,dlzka);
    if(pomoc>maximum)maximum=pomoc;
    pomoc=palindrom(i,i+2,slovo,dlzka);
    if(pomoc>maximum)maximum=pomoc;
  }
  return maximum;
}

int main()
{
  char slovo[50000];
  while(scanf("%s",&slovo)>0){
    printf("%d\n",najdipalindrom(slovo,strlen(slovo)));
  }

  return 0;
}
