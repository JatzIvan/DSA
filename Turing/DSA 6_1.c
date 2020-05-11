// uloha6-1.c -- Ivan Jatz, 28.10.2019 11:13
#include <stdlib.h>
#include <string.h>

int pole[100000]={0};

void pridaj(int i,int inkrement){
	while(i<100000){
		pole[i]+=inkrement;
		i=i+(i&-i);
	}	
}
int sucet(int i,int k){
	int suc=0;
  	k=k+i;
	i--;
	while(i>0){
		suc+=pole[i];
		i=i-(i&-i);
	}
	int pom=0;
	while(k>0){
		pom+=pole[k];
		k=k-(k&-k);
	}
	return pom-suc;
}
int main(){
	
	 char x[4];
 	int i,k;
 	int id=5;
 while((scanf("%s %d %d",x,&i,&k))>0){
   if(strcmp(x,"inc")==0){
   	pridaj(i,k);
   }if(strcmp(x,"dec")==0){
     pridaj(i,-k);
   }if(strcmp(x,"get")==0){
    printf("%d\n",sucet(i,k));
   }
 
}
return 0;
}
