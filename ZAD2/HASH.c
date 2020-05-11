// moj-hash.c -- Ivan Jatz, 21.10.2019 11:13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "zadanie2.h"
// Vlastna implementacia hashovania.

// ...

struct Tabulka{																				//struktura tabulky, ktora obsahuje smernik na spajany zoznam
	struct Node *prvok;
};

struct Node{																				//struktura prvku v spajanom zozname
	struct Node *dalsi;
	char *hodnota;
	char *kluc;
};

struct Size{																				//zaciatocna struktura, ktora obsahuje informacie o velkosti, aktualnej dlzke a smernik na zaciatok tabulky
	struct Tabulka *pointer;
	int dlzka_pola,akt_dlzka;
};


struct Size *vytvor_tabulku(){																//funkcia, ktora vytvori prazdnu hash tabulku, na zaciatok ju nastavim na velkost 10
	struct Size *zaciatok=(struct Size*)malloc(sizeof(struct Size));
	struct Tabulka *pointer;
	zaciatok->pointer=(struct Tabulka*)malloc(11*sizeof(struct Tabulka));
	int i;
	zaciatok->dlzka_pola=11;
	zaciatok->akt_dlzka=0;
	pointer=zaciatok->pointer;
	for(i=0;i<11;i++){
		pointer[i].prvok=NULL;
	}
	return zaciatok;
}

int najdi_prvocislo(int size){
	int i,j=size,prvocislo=0;
	while(1){
		for(i=2;i<j/2;i++){
			if(j%i==0){
				prvocislo=1;
				break;
			}
		}
		if(prvocislo==0){
			break;
		}else{
			prvocislo=0;
			j++;
		}
	}
	return j;
}

int vrat_kluc(char *vstup){																	//funkcia, ktora vrati hash kluc, podla ktoreho budem ukladat do tabulky
	int i=0,suc=0;
	while(1){
		if(vstup[i]==NULL)break;
		suc+=(int)(vstup[i]*pow(2,i)-i)*3;														//kluc tvorim ako nasobok hodnoty daneho charakteru s druhou mocninou podla pozicie - pozicia
		i++;
	}
	return suc;
}

struct Tabulka *rehash(char *vstup,char *klucik, struct Size *zaciatok){					//funckia, ktora sluzi na zvacsenie tabulky, a prenesenie, preusporiadanie prvkov zo starej do novej hash tabulky
	struct Tabulka *f=zaciatok->pointer;
	int dlzka_pola=zaciatok->dlzka_pola;
	int docasna_velkost=najdi_prvocislo(dlzka_pola*4),i,kluc=vrat_kluc(klucik);
	struct Tabulka *docasna=(struct Tabulka*)malloc(docasna_velkost*sizeof(struct Tabulka));
	struct Node *akt=docasna[kluc%docasna_velkost].prvok,*pomocny;
	for(i=0;i<docasna_velkost;i++){
		docasna[i].prvok=NULL;
	}
	docasna[kluc%docasna_velkost].prvok=(struct Node*)malloc(sizeof(struct Node));			//ulozi prvy prvok, ktory som uz neukladal do starej hash tabulky
	docasna[kluc%docasna_velkost].prvok->hodnota=strdup(vstup);
	docasna[kluc%docasna_velkost].prvok->dalsi=NULL;
	docasna[kluc%docasna_velkost].prvok->kluc=strdup(klucik);
	for(i=0;i<dlzka_pola;i++){																//prepocitam kluc kazdeho prvku zo starej tabulky, a premiestnim ho na spravne miesto v novej tabulke
		akt=f[i].prvok;
		if(f[i].prvok!=NULL){
			while(1){
			kluc=vrat_kluc(akt->kluc);
			pomocny=docasna[kluc%docasna_velkost].prvok;
			if(docasna[kluc%docasna_velkost].prvok==NULL){
				docasna[kluc%docasna_velkost].prvok=(struct Node*)malloc(sizeof(struct Node));
				docasna[kluc%docasna_velkost].prvok->hodnota=strdup(akt->hodnota);
				docasna[kluc%docasna_velkost].prvok->dalsi=NULL;
				docasna[kluc%docasna_velkost].prvok->kluc=strdup(akt->kluc);
			}else{
				struct Node *pomoc=docasna[kluc%docasna_velkost].prvok;
				docasna[kluc%docasna_velkost].prvok=(struct Node*)malloc(sizeof(struct Node));
				docasna[kluc%docasna_velkost].prvok->hodnota=strdup(akt->hodnota);
				docasna[kluc%docasna_velkost].prvok->dalsi=pomoc;
				docasna[kluc%docasna_velkost].prvok->kluc=strdup(akt->kluc);
			}	
			if(akt->dalsi!=NULL){
				struct Node *prehod=akt->dalsi;
				free(akt->hodnota);															//stare prvky postupne uvolnujem(pre setrenie miesta)
				free(akt->kluc);
				free(akt);
				akt=prehod;
			}else{
				free(akt->hodnota);
				free(akt->kluc);
				free(akt);
				break;
			}
	}
	}
	}
	
	free(f);																				//uvolnim staru tabulku
	zaciatok->dlzka_pola=docasna_velkost;
	zaciatok->pointer=docasna;
	return docasna;									
}

struct Tabulka *pridaj_prvok(char *vstup,char *klucik, struct Size *zaciatok){				//funkcia, ktora sluzi na pridanie prvku do tabulky
	struct Tabulka *f=zaciatok->pointer;
	int kluc=vrat_kluc(klucik),dlzka_retazca=0,dlzka_pola=zaciatok->dlzka_pola,akt_dlz=zaciatok->akt_dlzka;			//vypocitam hash kluc, podla kluca ktory som vygeneroval pre uzivatela
	struct Node *pomoc=f[kluc%dlzka_pola].prvok;
	zaciatok->akt_dlzka+=1;
	if((zaciatok->akt_dlzka/dlzka_pola)>=2){
		f=rehash(vstup,klucik,zaciatok);
		return f;
	}
	if(pomoc==NULL){																								//ak je prazdne policko v tabulke, pridam prvok(vytvorim retaz)
		f[kluc%dlzka_pola].prvok=(struct Node*)malloc(sizeof(struct Node));
		f[kluc%dlzka_pola].prvok->hodnota=strdup(vstup);
		f[kluc%dlzka_pola].prvok->dalsi=NULL;
		f[kluc%dlzka_pola].prvok->kluc=strdup(klucik);
		return f;
	}else{																											//ak sa tam uz prvok nachadza, pridam ho pred neho a napojim na existujucu retaz
		struct Node *prem=f[kluc%dlzka_pola].prvok;
		f[kluc%dlzka_pola].prvok=NULL;
		f[kluc%dlzka_pola].prvok=(struct Node*)malloc(sizeof(struct Node));
		f[kluc%dlzka_pola].prvok->hodnota=strdup(vstup);
		f[kluc%dlzka_pola].prvok->dalsi=prem;
		f[kluc%dlzka_pola].prvok->kluc=strdup(klucik);
	}
	return f;
}

void vypis(struct Size *zaciatok){																					//funkcia, ktora sluzi na vypis prvkov hash tabulky
	struct Tabulka *f=zaciatok->pointer;
	int i,pocet=0,ciast=0;
	struct Node *akt;
	for(i=0;i<zaciatok->dlzka_pola;i++){
		if(f[i].prvok!=NULL){
			akt=f[i].prvok;
			while(1){
				ciast++;
				pocet++;
				printf("%s\n",akt->hodnota);
				if(akt->dalsi!=NULL){
					akt=akt->dalsi;
				}else{
					//printf("%d******\n",ciast);
					ciast=0;
					//printf("----------------------------------\n");
					break;
				}
			}
		}
	}
}

struct Node *najdi_prvok(struct Size *zaciatok, char *string){										//funkcia, ktora sluzi na vyhladanie zaznamu, a vratenie jeho struktury
	struct Tabulka *tabulka=zaciatok->pointer;
	struct Node *pomoc;
	int kluc=vrat_kluc(string);
	if(tabulka[kluc%zaciatok->dlzka_pola].prvok!=NULL){
		pomoc=tabulka[kluc%zaciatok->dlzka_pola].prvok;
		while(1){
			if(strcmp(pomoc->kluc,string)==0){														//ak sa kluce zhoduju, prvok sa nachadza v tabulke, vratim ho
				//printf("prvok sa tu nachadza\n");
				return pomoc;
				//break;
			}
			if(pomoc->dalsi==NULL){																	//ak je koniec retaze a prvok som este nenasie, tak sa v tabulke nenachadza
				//printf("neexistuje\n");
				return NULL;
				break;
			}else{
				pomoc=pomoc->dalsi;
			}
		}
	}else{																						//ak v tabulke na danom mieste neexistuje ziaden zaznam, prvok neexistuje
		return NULL;
		//printf("neexistuje\n");
	}
}

char *daj_klucik(int dlzka){																	//funkcia, ktora vygeneruje kluc(simulacia zadavania kluca uzivatelom, vyuzivam pri velkych testoch, pri ktorych nechcem rucne zadavat milion klucov)
	char *a=(char*)malloc((dlzka+1)*sizeof(char));
	int i;
	for(i=0;i<dlzka;i++){
		a[i]='0'+rand()%72;																		//kluce generujem pridanim nahodnej hodnoty do 72 k 0
	}
	a[dlzka]='\0';
	return a;
}
