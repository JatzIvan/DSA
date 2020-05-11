// uloha7-1.c -- Ivan Jatz, 4.11.2019 11:38
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vrchol{
	char bols;
	struct Hrana *hrany;
	int pocH;
};

struct Hrana{
	int dlzka;
	struct Vrchol *ciel;
	struct Vrchol *odk;
};

struct Pole{
	struct Vrchol *vrch;
};

int vypocet_dlzky(struct Vrchol *z,struct Vrchol *kam,struct Vrchol *akt,int suc){
	int i,ii;
	akt->bols='y';
	if(akt==kam){
	return 0;
}

	for(i=0;i<akt->pocH;i++){
		if(akt->hrany[i].ciel==NULL){
			if(akt->hrany[i].odk->bols!='y'){
				suc=vypocet_dlzky(z,kam,akt->hrany[i].odk,suc);
			}
		}if(akt->hrany[i].odk==NULL){
			if(akt->hrany[i].ciel->bols!='y'){
		suc=vypocet_dlzky(z,kam,akt->hrany[i].ciel,suc);
	}
	}
		if(suc!=-1){
			//printf("s\n");
			suc+=akt->hrany[i].dlzka;
			return suc;
		}
	}
	return suc;
}
int main(){
	int pocV,pocH,zac,kon,i,j;
	scanf("%d %d %d %d",&pocV,&pocH,&zac,&kon);
	//pole=(struct Vrchol*)malloc(pocV*sizeof(struct Vrchol));
	struct Pole *pole=(struct Pole*)malloc(pocV*sizeof(struct Pole));
	for(i=0;i<pocV;i++){
		pole[i].vrch=(struct Vrchol*)malloc(sizeof(struct Vrchol*));
		pole[i].vrch->hrany=NULL;
		pole[i].vrch->pocH=0;
}
	for(i=0;i<pocH;i++){
		int pom1,pom2,dlzka;
		scanf("%d %d %d",&pom1,&pom2,&dlzka);
		if(pole[pom1-1].vrch->hrany==NULL){
			pole[pom1-1].vrch->hrany=(struct Hrana*)malloc(sizeof(struct Hrana));
			pole[pom1-1].vrch->pocH+=1;
			pole[pom1-1].vrch->hrany[pole[pom1-1].vrch->pocH-1].dlzka=dlzka;
			pole[pom1-1].vrch->hrany[pole[pom1-1].vrch->pocH-1].ciel=pole[pom2-1].vrch;
			pole[pom1-1].vrch->hrany[pole[pom1-1].vrch->pocH-1].odk=NULL;
			if(pole[pom2-1].vrch->hrany==NULL){
				pole[pom2-1].vrch->pocH+=1;
				pole[pom2-1].vrch->hrany=(struct Hrana*)malloc(sizeof(struct Hrana));
			}else{
				pole[pom2-1].vrch->pocH+=1;
				pole[pom2-1].vrch->hrany=(struct Hrana*)realloc(pole[pom2-1].vrch->hrany,pole[pom2-1].vrch->pocH*sizeof(struct Hrana));
			}
			pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].dlzka=dlzka;
			pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].odk=pole[pom1-1].vrch;
			pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].ciel=NULL;
		}else{
			pole[pom1-1].vrch->pocH+=1;
			pole[pom1-1].vrch->hrany=(struct Hrana*)realloc(pole[pom1-1].vrch->hrany,pole[pom1-1].vrch->pocH*sizeof(struct Hrana));
			pole[pom1-1].vrch->hrany[pole[pom1-1].vrch->pocH-1].dlzka=dlzka;
			pole[pom1-1].vrch->hrany[pole[pom1-1].vrch->pocH-1].ciel=pole[pom2-1].vrch;
			pole[pom1-1].vrch->hrany[pole[pom1-1].vrch->pocH-1].odk=NULL;
			if(pole[pom2-1].vrch->hrany==NULL){
				pole[pom2-1].vrch->pocH+=1;
				pole[pom2-1].vrch->hrany=(struct Hrana*)malloc(sizeof(struct Hrana));
				pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].dlzka=dlzka;
				pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].odk=pole[pom1-1].vrch;
			}else{
				pole[pom2-1].vrch->pocH+=1;
				pole[pom2-1].vrch->hrany=(struct Hrana*)realloc(pole[pom2-1].vrch->hrany,pole[pom2-1].vrch->pocH*sizeof(struct Hrana));
				pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].dlzka=dlzka;
				pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].odk=pole[pom1-1].vrch;
				pole[pom2-1].vrch->hrany[pole[pom2-1].vrch->pocH-1].ciel=NULL;
			}
			
		}
	}
	int koniec;
	koniec=-1;
	koniec=vypocet_dlzky(pole[zac-1].vrch,pole[kon-1].vrch,pole[zac-1].vrch,koniec);
	if(koniec==-1)koniec=0;
	printf("%d\n",koniec);
	return 0;
}
