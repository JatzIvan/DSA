#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Objednavka{
	char *meno;
	int hodnota;
};

struct Objednavka objednavky[100000];
int pocet=0;

void vloz(char *meno,int hodnota){
	objednavky[pocet].hodnota=hodnota;
	objednavky[pocet].meno=strdup(meno);
	int akt=pocet;
	char *mmm;
	while(1){
	if(akt==0)break;
	if(objednavky[akt].hodnota>objednavky[(akt-1)/2].hodnota){
		mmm=objednavky[akt].meno;
		int hodn=objednavky[akt].hodnota;
		objednavky[akt].meno=objednavky[(akt-1)/2].meno;
		objednavky[akt].hodnota=objednavky[(akt-1)/2].hodnota;
		objednavky[(akt-1)/2].meno=mmm;
		objednavky[(akt-1)/2].hodnota=hodn;
		akt--;
		akt/=2;
	}else{
		break;
	}
	}
  pocet++;
}
char *vyber_najvyssie(){
	pocet--;
	int i;
	char *x=objednavky[0].meno;
	//free(objednavky[0].meno);
	objednavky[0].hodnota=objednavky[pocet].hodnota;
	objednavky[0].meno=objednavky[pocet].meno;
	//objednavky[0].meno=strdup(objednavky[pocet].meno);
	objednavky[pocet].hodnota=NULL;
	//free(objednavky[pocet].meno);
	int pomoc=1;
	char *ss;
	while(1){
		if((objednavky[pomoc*2-1].hodnota==NULL)&&(objednavky[pomoc*2].hodnota==NULL)){
			break;
		}
		if((objednavky[pomoc-1].hodnota>objednavky[pomoc*2-1].hodnota)&&(objednavky[pomoc-1].hodnota>objednavky[pomoc*2].hodnota)){
			break;
		}
		if((objednavky[pomoc-1].hodnota<objednavky[pomoc*2-1].hodnota)||(objednavky[pomoc-1].hodnota<objednavky[pomoc*2].hodnota)){
				if(objednavky[pomoc*2-1].hodnota>objednavky[pomoc*2].hodnota){
						int pom=objednavky[pomoc-1].hodnota;
						ss=objednavky[pomoc-1].meno;
						objednavky[pomoc-1].hodnota=objednavky[pomoc*2-1].hodnota;
						objednavky[pomoc-1].meno=objednavky[pomoc*2-1].meno;
						//objednavky[pomoc-1].meno=strdup(objednavky[pomoc*2-1].meno);
						objednavky[pomoc*2-1].hodnota=pom;
						objednavky[pomoc*2-1].meno=ss;
						//objednavky[pomoc*2-1].meno=strdup(ss);
						pomoc*=2;
				}else{
						int pom=objednavky[pomoc-1].hodnota;
						ss=objednavky[pomoc-1].meno;
						objednavky[pomoc-1].hodnota=objednavky[pomoc*2].hodnota;
						objednavky[pomoc-1].meno=objednavky[pomoc*2].meno;
						//objednavky[pomoc-1].meno=strdup(objednavky[pomoc*2].meno);
						objednavky[pomoc*2].hodnota=pom;
						objednavky[pomoc*2].meno=ss;
						//objednavky[pomoc*2].meno=strdup(ss);
						pomoc*=2;
						pomoc++;
				}
		}
	}
	return x;
}
int main(){
	char nazov[100],typ[10];
	int hodnota;
	while((scanf("%s",&typ))!=EOF){
		if(strcmp(typ,"vloz")==0){
			scanf("%s %d",&nazov,&hodnota);
			vloz(nazov,hodnota);
		}else{
			printf("%s\n",vyber_najvyssie());
		}
	}
	return 0;
}
