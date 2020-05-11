// uloha1-2.c -- Ivan Jatz, 8.10.2019 18:22
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(){
	int vstup,i,j=0,poradie=0,k;
	int *prvocisla=(int*)malloc(100000*sizeof(int));
	int *pole=(int*)malloc(12997222*sizeof(int));
	for(i=0;i<=1299721;i++){
		pole[i]=i;
	}
	j=1;
	for(i=2;i<1299720;i++){
		if(pole[i]!=INT_MIN){
			for(k=2;k<=1299720;k++){
				if((i*k)<=1299720){
				pole[i*k]=INT_MIN;
			}else{
				break;
			}
		}
	}
	}
	for(i=j;i<1299720;i++){
		if(pole[i]!=INT_MIN){
			prvocisla[poradie]=pole[i];
			poradie++;
		}
	}
	 while(scanf("%d",&vstup)!=EOF){
	 	printf("%d\n",prvocisla[vstup]);
	 }
}
