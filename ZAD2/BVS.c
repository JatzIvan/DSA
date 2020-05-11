// bvs.c -- Ivan Jatz, 21.10.2019 11:13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementacia nevyvazeneho BVS patri do tohto suboru.

// ...

struct Node{
	int hodnota;
	struct Node *lavy,*pravy;
};

struct Node *vytvorStrom(){
	struct Node *zaciatok=(struct Node*)malloc(sizeof(struct Node));
	zaciatok->hodnota=NULL;
	zaciatok->pravy=NULL;
	zaciatok->lavy=NULL;
	return zaciatok;
}
void vloz_blok(struct Node *koren, int hodnota){
	struct Node *akt=koren;
	if(koren->hodnota==NULL){
		koren->hodnota=hodnota;
	}else{
	while(1){
		if(hodnota<akt->hodnota){
			if(akt->lavy==NULL){
				akt->lavy=(struct Node*)malloc(sizeof(struct Node));
				akt=akt->lavy;
				akt->hodnota=hodnota;
				akt->lavy=NULL;
				akt->pravy=NULL;
				break;
			}else{
				akt=akt->lavy;
			}
		}else{
			if(akt->pravy==NULL){
				akt->pravy=(struct Node*)malloc(sizeof(struct Node));
				akt=akt->pravy;
				akt->hodnota=hodnota;
				akt->lavy=NULL;
				akt->pravy=NULL;
				break;
			}else{
				akt=akt->pravy;
			}
		}
	}
	}
}
int prehladaj_strom(struct Node *koren, int kluc){
	struct Node *akt=koren;
	while(1){
		if(kluc==akt->hodnota){
			return 1;
		}else{
			if(kluc<akt->hodnota){
				if(akt->lavy==NULL)return 0;
				else akt=akt->lavy;
			}else{
				if(akt->pravy==NULL)return 0;
				else akt=akt->pravy;
			}
		}
	}
}
