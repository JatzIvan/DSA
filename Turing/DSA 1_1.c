// uloha1-1.c -- Ivan Jatz, 6.10.2019 21:20
#include <stdio.h>
#include <stdlib.h>
int main(){
	int pScen=0;
	int i;
	scanf("%d",&pScen);
	if(pScen==0){
		printf("0\n");
		return 0;
	}
	char f;
	int *maxSceny=(int*)malloc(pScen*sizeof(int));
	int *cislo;
	int por=0;
	for(i=0;i<pScen;i++){
		cislo=(int*)malloc(1*sizeof(int));
		int pom,p;
		por=0;
		scanf("%d",&p);
		if(p==0)maxSceny[i]=0;
		else{
		while(1){
			cislo=(int*)realloc(cislo,(por+1)*sizeof(int));
			scanf("%d",&pom);
			cislo[por]=pom;
			pom=0;
			por++;
			if(p==por)break;
		}
		maxSceny[i]=0;
		int max=0,j,k,kup=0,ppom=0;
		for(j=0;j<p;j++){
			if((cislo[j]<cislo[j+1])&&(kup==0)){
				ppom=cislo[j];
				kup=1;
			}
			if(((j==p-1)||(cislo[j]>cislo[j+1]))&&(kup==1)){
				maxSceny[i]+=(cislo[j]-ppom);
				kup=0;
			}
		}
	}
	}
	for(i=0;i<pScen;i++){
	printf("%d\n",maxSceny[i]);
	}	
	return 0;
}
