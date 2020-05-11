// uloha1-3.c -- Ivan Jatz, 21.10.2019 13:09
#include <stdio.h>
#include <stdlib.h>
void rekurzia(int s,char x[],int poz);
int rekpoc(int s,int poradie,int akt);
void vrekurzia(int dlzka){
	char *pomPole=(char*)malloc((dlzka)*sizeof(char));
	int poc=rekpoc(dlzka,0,1);
	printf("%d\n",poc);
	int h;
	rekurzia(dlzka,pomPole,0);
}
	
int rekpoc(int s,int pred,int akt){
	if(s==1){
		return akt+pred;
	}
	int pom=akt;
	akt=akt+pred;
	pred=pom;
	rekpoc(s-1,pred,akt);
}
void rekurzia(int s,char *x,int poz){
	if(s==0){
		printf("%.*s",poz,x);
	printf("\n");
		return;
	}
		if(s-2>=0){
			x[poz]='=';
			rekurzia(s-2,x,poz+1);
		}if(s-1>=0){
			x[poz]='|';
			rekurzia(s-1,x,poz+1);	
		}
}
int main(){
	int dlzkaChodby=0;
	while(scanf("%d",&dlzkaChodby)==1){
	vrekurzia(dlzkaChodby);
}
	return 0;
}
