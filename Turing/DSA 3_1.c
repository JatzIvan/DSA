// uloha3-1.c -- Ivan Jatz, 7.10.2019 12:03
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct strom{
	int koren;
	int hlbka;
	struct strom *left,*right;
};

int main(){
	int vstup;
  struct strom *prvy=NULL,*akt;
	scanf("%d",&vstup);
	printf("0\n");
	prvy=(struct strom*)malloc(sizeof(struct strom));
	prvy->koren=vstup;
	prvy->hlbka=0;
	prvy->left=NULL;
	prvy->right=NULL;
	akt=prvy;
	while(scanf("%d",&vstup)!=EOF){
		while(1){
    if(vstup==akt->koren){
	printf("%d\n",akt->hlbka);
	akt=prvy;
	break;
	}
	if(vstup>akt->koren){
		if(akt->right==NULL){
			akt->right=(struct strom*)malloc(sizeof(struct strom));
			akt->right->koren=vstup;
			akt->right->hlbka=akt->hlbka+1;
			printf("%d\n",akt->right->hlbka);
			akt=akt->right;
			akt->left=NULL;
			akt->right=NULL;
			akt=prvy;
			break;
		}else{
			akt=akt->right;
		}
	}if(vstup<akt->koren){
		if(akt->left==NULL){
			akt->left=(struct strom*)malloc(sizeof(struct strom));
			akt->left->koren=vstup;
			akt->left->hlbka=akt->hlbka+1;
			printf("%d\n",akt->left->hlbka);
			akt=akt->left;
			akt->left=NULL;
			akt->right=NULL;
			akt=prvy;
			break;
		}else{
			akt=akt->left;
		}
	}
	}
	}
}
