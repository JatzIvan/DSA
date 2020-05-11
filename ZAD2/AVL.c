// moj-vyvazeny.c -- Ivan Jatz, 21.10.2019 11:13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zadanie2.h"
struct Node{																							//struktura pre jeden Node v strome
	int hodnota,vyska;
	struct Node *lavy,*pravy;
};


struct Node *vytvor_strom(){																			//funkcia ktora sluzi na vytvorenie prazdneho stromu
	struct Node *zaciatok;
	zaciatok=(struct Node*)malloc(sizeof(struct Node));
	zaciatok->hodnota=0;
	zaciatok->vyska=-1;
	zaciatok->lavy=NULL;
	zaciatok->pravy=NULL;
	return zaciatok;
}

struct Node *LL_rotacia(struct Node *star_rodic,int prepinac){											//funkcia pre LL rotaciu
	struct Node *akt,*akt_p;
	if(prepinac==1){
		akt=star_rodic->lavy;
		akt_p=akt->pravy;
		star_rodic->lavy=akt_p;
	}if(prepinac==2){
		akt=star_rodic->pravy;
		akt_p=akt->pravy;
		star_rodic->pravy=akt_p;
	}if(prepinac==3){
		akt=star_rodic;
		akt_p=akt->pravy;
	}
	akt->pravy=akt_p->lavy;																				//urobim lavu rotaciu, to znamena ze prave dieta sa stane rodicom
	akt_p->lavy=akt;
	akt->vyska=zisti_hlbku(akt);
	akt_p->vyska=zisti_hlbku(akt_p);
	return akt_p;
	
}

struct Node *RR_rotacia(struct Node *star_rodic,int prepinac){												//funkcia pre RR rotaciu
	struct Node *akt,*akt_l;
	if(prepinac==1){
		akt=star_rodic->lavy;
		akt_l=akt->lavy;
		star_rodic->lavy=akt_l;
	}if(prepinac==2){
		akt=star_rodic->pravy;
		akt_l=akt->lavy;
		star_rodic->pravy=akt_l;
	}if(prepinac==3){
		akt=star_rodic;
		akt_l=akt->lavy;
	}
	akt->lavy=akt_l->pravy;																					//urobim pravu rotaciu, to znamena ze lave dieta sa stane rodicom
	akt_l->pravy=akt;
	akt->vyska=zisti_hlbku(akt);
	akt_l->vyska=zisti_hlbku(akt_l);
	return akt_l;
	
}
struct Node *RL_rotacia(struct Node *star_rodic,int prepinac){													//funkcia pre RL rotaciu
	struct Node *akt;
	if(prepinac==1){
		akt=star_rodic->lavy;
		akt->pravy=RR_rotacia(akt,2);
		star_rodic->lavy=LL_rotacia(star_rodic,prepinac);
		return star_rodic->lavy;
	}if(prepinac==2){
		akt=star_rodic->pravy;
		akt->pravy=RR_rotacia(akt,2);
		star_rodic->pravy=LL_rotacia(star_rodic,prepinac);
		return star_rodic->pravy;
	}if(prepinac==3){
		akt=star_rodic;
		akt->pravy=RR_rotacia(akt,2);
		star_rodic=LL_rotacia(star_rodic,prepinac);
		return star_rodic;
	}
}
struct Node *LR_rotacia(struct Node *star_rodic,int prepinac){												//funkcia pre LR rotaciu
	struct Node *akt,*akt_l,*pom_l,*pom_p;
	if(prepinac==1){
		akt=star_rodic->lavy;
		akt->lavy=LL_rotacia(akt,1);
		star_rodic->lavy=RR_rotacia(star_rodic,prepinac);
		return star_rodic->lavy;
	}if(prepinac==2){
		akt=star_rodic->pravy;
		akt->lavy=LL_rotacia(akt,1);
		star_rodic->pravy=RR_rotacia(star_rodic,prepinac);
		return star_rodic->pravy;
	}if(prepinac==3){
		akt=star_rodic;
		akt->lavy=LL_rotacia(akt,1);
		star_rodic=RR_rotacia(star_rodic,prepinac);
		return star_rodic;
	}
}
int zisti_koeff(struct Node *koren){																		//funkcia vrati koeficient pre dany Node, ziska ho tak ze odpocita lavu a pravu stranu stromu
	int hodnota=0;
	if(koren->lavy!=NULL){
		hodnota=1+koren->lavy->vyska;
	}else{
		hodnota=0;
	}
	if(koren->pravy!=NULL){
		hodnota=hodnota-(1+koren->pravy->vyska);
	}
	return hodnota;
}

int zisti_hlbku(struct Node *koren){																		//funkcia ktora zisti hlbku daneho prvku v strome(hlbka znamena najdlhsia cesta ku koncu stromu)
	int h_l,h_p;
	if(koren->lavy==NULL)h_l=0;
	else h_l=koren->lavy->vyska+1;
	if(koren->pravy==NULL)h_p=0;
	else h_p=koren->pravy->vyska+1;
	
	if(h_p>h_l)return h_p;	
	else return h_l;
}

struct Node *vloz_prvok(struct Node *koren,int hodnota){													//funkcia na vkladanie prvku do stromu, vracia smernik na Node
	struct Node *pred,*pomoc2;
	int prepinac=0;
	if(koren!=NULL&&(koren->vyska==-1)&&(koren->hodnota==0)){											//ak vkladam prvy prvok(root)
		koren->hodnota=hodnota;
		pred=koren;
		return koren;
	}else{
	if((koren==NULL)){																						//ak som sa dostal na koniec stromu, mozem vytvorit novy zaznam
		koren=(struct Node*)malloc(sizeof(struct Node));
		koren->hodnota=hodnota;
		koren->vyska=0;
		koren->lavy=NULL;
		koren->pravy=NULL;
		pred=koren;
	}else{
			if(hodnota<koren->hodnota){																		//ak je hodnota mensia ako dany Node tak sa posunie na lavu vetvu
				pred=koren;
				koren->lavy = vloz_prvok(koren->lavy,hodnota);
				pomoc2=koren->lavy;
				prepinac=1;
			}else{																							//ak je hodnota vacsia ako dany Node tak sa posunie na pravu vetvu
				pred=koren;
				koren->pravy = vloz_prvok(koren->pravy,hodnota);
				pomoc2=koren->pravy;
				prepinac=2;
			}
	}
}
	int koeff;
	if(pred->vyska!=-1)
	koren->vyska=zisti_hlbku(koren);																		//upravuje vysku po vkladani
	if(prepinac==0)koeff=zisti_koeff(koren);																//zistujem koeficient, vdaka ktoremu viem urcit, ci potrebujem rotovat
	if(prepinac==1)koeff=zisti_koeff(pred->lavy);
	if(prepinac==2)koeff=zisti_koeff(pred->pravy);
	if(koeff>1){																							//ak je koeficient vacsi ako 1, tak viem ze budem musiet rotovat 
		int pomoc_koeff=zisti_koeff(pomoc2->lavy);
		if(pomoc_koeff>0){																					//ak nasledujuci prvok ma koeff vacsi ako 0 tak bude vykonavat rotaciu RR(lave dieta sa stane rodicom)
			if(prepinac==1){
				pred->lavy=RR_rotacia(pred,prepinac);
			}if(prepinac==2){	
			pred->pravy=RR_rotacia(pred,prepinac);
			}
		}else{																								//inak urobi rotaciu LR(najskor sa lave-prave dieta stane rodicom, a potom sa stane aj prarodicom)
			if(prepinac==1){
				pred->lavy=LR_rotacia(pred,prepinac);
			}if(prepinac==2){	
			pred->pravy=LR_rotacia(pred,prepinac);
			}
		}
		if(pred->vyska!=-1)
		pred->vyska=zisti_hlbku(pred);																		//vyrovnavam vysku po rotacii
	}if(koeff<-1){																							//ak je koeficient mensi ako 1 tak budem rotovat
		int pomoc_koeff=zisti_koeff(pomoc2->pravy);
		if(pomoc_koeff>0){																					//ak nasledujuci prvok ma koeficient vacsi ako 0 tak bude robit rotaciu RL(najskor lave-prave dieta sa stane rodicom, potom sa tento prvok stane prarodicom)		
			if(prepinac==1){
				pred->lavy=RL_rotacia(pred,prepinac);
			}if(prepinac==2){	
			pred->pravy=RL_rotacia(pred,prepinac);
			}
		}else{																								//inak urobi rotaciu LL(lave dieta sa stane rodicom)
			if(prepinac==1){
				pred->lavy=LL_rotacia(pred,prepinac);
			}if(prepinac==2){	
			pred->pravy=LL_rotacia(pred,prepinac);
			}
		}
		if(pred->vyska!=-1)
		pred->vyska=zisti_hlbku(pred);
	}
	if(pred->vyska==-1){																					//ak som sa dostal ku korenu, sledujem ci mozem rotovat koren
		pred->vyska=zisti_hlbku(pred);
		prepinac=3;
		koeff=zisti_koeff(pred);
	if(koeff>1){
		int pomoc_koeff=zisti_koeff(koren->lavy);
		if(pomoc_koeff>0){
			pred=RR_rotacia(pred,prepinac);
		}else{
			pred=LR_rotacia(pred,prepinac);
		}
		pred->vyska=zisti_hlbku(pred);
	}if(koeff<-1){
		int pomoc_koeff=zisti_koeff(koren->pravy);
		if(pomoc_koeff>0){
			pred=RL_rotacia(pred,prepinac);
		}else{
			pred=LL_rotacia(pred,prepinac);
		}
		pred->vyska=zisti_hlbku(pred);
	}
	pred->vyska=-1;
	}
	return pred;																							//vraciam smernik na Node
}

int vyhladaj_prvok(struct Node *koren,int prvok){									//funkcia na vyhladanie prvku v strome
	int pocH=0;
	while(1){
		pocH++;																		//pocita pocet prechodov
		if(prvok==koren->hodnota){													//ak sa nasiel prvok v strome
			//nachadza=1;
			//printf("Nachadza sa\n");
			//printf("pocet prechodov: %d\n",pocH-1);
			break;
		}
		if(prvok>koren->hodnota){
			if(koren->pravy==NULL){													//ked sa prvok v danej vetve uz nemoze nachadat ukoncim cyklus
				//printf("Nenachadza sa\n");
				break;
			}else{
				koren=koren->pravy;
			}
		}if(prvok<koren->hodnota){
			if(koren->lavy==NULL){													//ked sa prvok v danej vetve uz nemoze nachadat ukoncim cyklus
				//printf("Nenachadza sa\n");
				break;
			}else{
				koren=koren->lavy;
			}
		}
	}
	return pocH-1;
}

void preorder(struct Node *koren){														//funkcia na preorder vypis
	    printf("%d->", koren->hodnota);
    if (koren->lavy != NULL)
        preorder(koren->lavy);
    if (koren->pravy != NULL)
        preorder(koren->pravy);
}
