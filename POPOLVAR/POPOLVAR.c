// zadanie3.c -- Ivan Jatz, 11.11.2019 11:00

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

struct Vrchol{
	int suradnice;
};

struct Informacie{
	int hodnotapol;
	int pred;
	int heaplok;
};

struct Princezne{											//struktura v krorej ukladam cesty pre princezne bez pouzitia teleportu
	int **dc;
	int **p1;
	int **p2;
	int **p3;
	int **p4;
	int **p5;
};

struct PrinceznePortal{										//struktura v ktorej ukladam cesty pre princezne s pouzitim teleportu
	int **GP;
	int **P1;
	int **P2;
	int **P3;
	int **P4;
	int **P5;
};

void popFinal(int *FinalnaPostupnost, int *konecnaDrak, int dlzkaPostup,int zaciatok){					//funkcia, ktora vyberie prvky zo zasobnika do pola
	int i;
	for(i=zaciatok;i<dlzkaPostup;i+=2){
		FinalnaPostupnost[i]=konecnaDrak[dlzkaPostup-i-2];
		FinalnaPostupnost[i+1]=konecnaDrak[dlzkaPostup-i-1];
	}
}

void heapify(struct Vrchol *vrch,int miesto,struct Informacie *info){								//funkcia navrati haldu spat do haldoveho stavu
	while(1){																						//cyklus bude prebiehat dokial sa prvok nedostane na vrch, alebo rodic nie je mensi		
		if(abs(info[vrch[(miesto-1)/2].suradnice].hodnotapol)==abs(info[vrch[miesto].suradnice].hodnotapol))break;
		if(abs(info[vrch[(miesto-1)/2].suradnice].hodnotapol)>abs(info[vrch[miesto].suradnice].hodnotapol)){
			struct Vrchol pom=vrch[miesto];
			info[vrch[miesto].suradnice].heaplok=(miesto-1)/2;
			info[vrch[(miesto-1)/2].suradnice].heaplok=miesto;
			vrch[miesto]=vrch[(miesto-1)/2];
			vrch[(miesto-1)/2]=pom;
			miesto=(miesto-1)/2;
		}else{
			break;
		}
		if(miesto==0)break;
	}
}

int daj_dlzku(char x){																				//funckia vrati velkost hrany pre dany prvok mapy
	if(x=='N')return -1;
	if((x=='C')||(x=='P')||(x=='D')||(x=='G')||((x>='0')&&(x<='9')))return 1;
	if(x=='H')return 2;
}

void relax(struct Vrchol *vrch,struct Informacie *info,int dlzka){									//funkcia sa zavola po odstraneni vrcholu z haldy, upravi haldu do spravneho tvaru
	int i=0;
	while(1){
		if(((i+1)*2>=dlzka)&&((i+1)*2-1>=dlzka))break;												//ak uz deti vrcholu su vacsie ako vrchol, ukonci cyklus
		if((((i+1)*2<dlzka)&&(abs(info[vrch[(i+1)*2].suradnice].hodnotapol)<abs(info[vrch[i].suradnice].hodnotapol)))){		
			if(((i+1)*2-1<dlzka)&&(abs(info[vrch[(i+1)*2-1].suradnice].hodnotapol)<abs(info[vrch[(i+1)*2].suradnice].hodnotapol))){
				struct Vrchol pomoc=vrch[i];
				info[vrch[i].suradnice].heaplok=(i+1)*2-1;
				info[vrch[(i+1)*2-1].suradnice].heaplok=i;
				vrch[i]=vrch[(i+1)*2-1];
				vrch[(i+1)*2-1]=pomoc;
				i=(i+1)*2-1;
			}else{
				struct Vrchol pomoc=vrch[i];
				info[vrch[i].suradnice].heaplok=(i+1)*2;
				info[vrch[(i+1)*2].suradnice].heaplok=i;
				vrch[i]=vrch[(i+1)*2];
				vrch[(i+1)*2]=pomoc;
				i=(i+1)*2;
			}
		}if((((i+1)*2-1<dlzka)&&(abs(info[vrch[(i+1)*2-1].suradnice].hodnotapol)<abs(info[vrch[i].suradnice].hodnotapol)))){
			if(((i+1)*2<dlzka)&&(abs(info[vrch[(i+1)*2].suradnice].hodnotapol)<abs(info[vrch[(i+1)*2-1].suradnice].hodnotapol))){
				struct Vrchol pomoc=vrch[i];
				info[vrch[i].suradnice].heaplok=(i+1)*2;
				info[vrch[(i+1)*2].suradnice].heaplok=i;
				vrch[i]=vrch[(i+1)*2];
				vrch[(i+1)*2]=pomoc;
				i=(i+1)*2;
			}else{
				struct Vrchol pomoc=vrch[i];
				info[vrch[i].suradnice].heaplok=(i+1)*2-1;
				info[vrch[(i+1)*2-1].suradnice].heaplok=i;
				vrch[i]=vrch[(i+1)*2-1];
				vrch[(i+1)*2-1]=pomoc;
				i=(i+1)*2-1;
			}
		}if((((i+1)*2-1<=dlzka))&&(abs(info[vrch[(i+1)*2-1].suradnice].hodnotapol)>=abs(info[vrch[i].suradnice].hodnotapol))&&((((i+1)*2<=dlzka))&&(abs(info[vrch[(i+1)*2].suradnice].hodnotapol)>=abs(info[vrch[i].suradnice].hodnotapol))))break;
	}
}
void poleH(struct Informacie *info,int x, int y){									//funkcia na vypis grafu s dlzkami ciest
	int i,j,increment=0;
	for(i=0;i<x;i++){
		for(j=0;j<y;j++){
			printf("%d ",info[increment].hodnotapol);
			increment++;
		}
		printf("\n");
	}
	printf("\n----------------------------------\n");
}
void heap(struct Vrchol *vrch, int *hodnoty, int poc){								//funckia, ktora mi vypise haldu
	int i;
	for(i=0;i<poc;i++){
		printf("%d %d\n",vrch[i].suradnice,hodnoty[vrch[i].suradnice]);
	}
}

void inicializujPrincezne(struct Princezne *pp,int m, int n,int iterator){			//funkcia sluzi na prvotnu inicializaciu poli pre cesty z princezien
	int j;
	for(j=0;j<iterator;j++){
		pp->dc[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->p1[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->p2[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->p3[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->p4[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->p5[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->dc[j][0]=0;
		pp->dc[j][1]=0;
		pp->p1[j][0]=0;
		pp->p1[j][1]=0;
		pp->p2[j][0]=0;
		pp->p2[j][1]=0;
		pp->p3[j][0]=0;
		pp->p3[j][1]=0;
		pp->p4[j][0]=0;
		pp->p4[j][1]=0;
		pp->p5[j][0]=0;
		pp->p5[j][1]=0;
}
}
void inicializujPrinceznePort(struct PrinceznePortal *pp,int m,int n,int iterator){		//funkcia mi inicializuje polia pre cesty princezien, ale po aktivacii generatoru
	int j;
	for(j=0;j<iterator;j++){
		pp->GP[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->P1[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->P2[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->P3[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->P4[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->P5[j]=(int*)malloc((m*n+2)*sizeof(int));
		pp->GP[j][0]=0;
		pp->GP[j][1]=0;
		pp->P1[j][0]=0;
		pp->P1[j][1]=0;
		pp->P2[j][0]=0;
		pp->P2[j][1]=0;
		pp->P3[j][0]=0;
		pp->P3[j][1]=0;
		pp->P4[j][0]=0;
		pp->P4[j][1]=0;
		pp->P5[j][0]=0;
		pp->P5[j][1]=0;
}
}

int *pridajDocesty(int adresaPrinc, int predchadz, int *FinalnaPostupnost, int *dlzkaPostupnosti,struct Informacie *info, int x, int y, int prepinac){	//funkcia vytvori cestu k vrcholu zo suradnic
	int *DocasPost=(int*)malloc(x*y*sizeof(int)),dlzkaP=0,celkVZ=info[adresaPrinc].hodnotapol;
	DocasPost[dlzkaP]=adresaPrinc%y;
	DocasPost[dlzkaP+1]=adresaPrinc/y;
	dlzkaP+=2;
	while(1){																								//cyklus spatne prechadza cestu, pridava vrcholy
		if(info[adresaPrinc].hodnotapol==0){
			break;
		}
		DocasPost[dlzkaP]=info[adresaPrinc].pred%y;
		DocasPost[dlzkaP+1]=info[adresaPrinc].pred/y;
		adresaPrinc=info[adresaPrinc].pred;
		dlzkaP+=2;
	}
	popFinal(FinalnaPostupnost,DocasPost,dlzkaP+*dlzkaPostupnosti,*dlzkaPostupnosti);						//cesta sa uklada do zasobnika, musi sa "popnut" do pola
	*dlzkaPostupnosti+=dlzkaP;
	if(prepinac==1){
	FinalnaPostupnost[0]=celkVZ;
	FinalnaPostupnost[1]=dlzkaP;
	}else{
	FinalnaPostupnost[0]+=celkVZ;
	}
	return FinalnaPostupnost;
}


void vycisti_pole(struct Vrchol *vrch, struct Informacie *info, int pocV, struct Vrchol *prehladane, int prehlad){			//funkcia, ktora vycisti pole, inicializuje ho na max hodnotu
		int i;
		for(i=0;i<pocV;i++){
			prehladane[vrch[i].suradnice]=vrch[i];
			prehlad++;
		}
		for(i=0;i<prehlad;i++){
			vrch[i]=prehladane[i];
			info[i].hodnotapol=INT_MAX;
			info[i].pred=-1;
			info[i].heaplok=i;
		}
}

int *Drakdijkstra(int min1,int ciel,struct Vrchol *vrch, struct Informacie *info,int *DrakPost,struct Vrchol *prehladane, int aktpocVrch, char **mojaMapa,int n, int m, int **Generator, int stavGen){
		//funkcia, ktora prebehne dijkstrov algoritmus pre mapu po draka
		int prehladan=0,minhodnota,min=min1;
		int i;
		while(1){						//cyklus bude prebiehat pokial nenavstivi vsetky vrcholy
		min=vrch[0].suradnice;         //vezme minimum z heapu
		minhodnota=info[min].hodnotapol;
		if((aktpocVrch==0)||(info[min].hodnotapol==INT_MAX)){				//ak navstivil vsetky vrcholy(heap je prazdny), ukonci cyklus
			break;
			}
		int hodnota=daj_dlzku(mojaMapa[min/m][min%m]);
		prehladane[vrch[0].suradnice]=vrch[0];
		vrch[0]=vrch[aktpocVrch-1];
		relax(vrch,info,aktpocVrch-1);	
		if(stavGen==1){						//ak je generator aktivny, a narazil na portal, navstivi vsetky mozne teleporty
				if((mojaMapa[min/m][min%m]>='0')&&(mojaMapa[min/m][min%m]<='9')){
					for(i=0;i<Generator[mojaMapa[min/m][min%m]-48][0];i++){
						if((minhodnota<info[Generator[mojaMapa[min/m][min%m]-48][i+1]].hodnotapol)&&(Generator[mojaMapa[min/m][min%m]-48][i+1]!=min)){
							info[Generator[mojaMapa[min/m][min%m]-48][i+1]].hodnotapol=minhodnota;
							info[Generator[mojaMapa[min/m][min%m]-48][i+1]].pred=min;
							heapify(vrch,info[Generator[mojaMapa[min/m][min%m]-48][i+1]].heaplok,info);
						}
					}
				}
			}
			if((min%m)-1>=0){				//ak moze ist dolava <-- , tak navstivi vrchol
				if(daj_dlzku(mojaMapa[min/m][min%m-1])!=-1){
					if(minhodnota+hodnota<info[min-1].hodnotapol){			//ak je to kratsia cesta, tak ju prepise
						info[min-1].hodnotapol=minhodnota+hodnota;
						info[min-1].pred=min;															//ked bude cesta na lavo
						heapify(vrch,info[min-1].heaplok,info);				//napravi haldovu vlastnost
					}		
				}
			}if((min%m)+1<m){				//ak moze ist doprava -->, tak navstivi vrchol
				if(daj_dlzku(mojaMapa[min/m][min%m+1])!=-1){
					if(minhodnota+hodnota<info[min+1].hodnotapol){			//ak je to kratsia cesta, tak ju prepise
						info[min+1].hodnotapol=minhodnota+hodnota;
						info[min+1].pred=min;																//ked bude cesta na pravo					
						heapify(vrch,info[min+1].heaplok,info);				//napravi haldovu vlastnost
					}
				}
			}
			if((min/m)-1>=0){												//ak moze ist nad seba ^ , tak navstivi vrchol
				if(daj_dlzku(mojaMapa[min/m-1][min%m])!=-1){
					if(minhodnota+hodnota<info[min-m].hodnotapol){			//ak je to kratsia cesta, tak ju prepise
						info[min-m].hodnotapol=minhodnota+hodnota;
						info[min-m].pred=min;																//ked bude cesta nad
						heapify(vrch,info[min-m].heaplok,info);				//napravi haldovu vlastnost
					}
				}
			}if((min/m)+1<n){												//ak moze ist pod seba v , tak navstivi vrchol
				if(daj_dlzku(mojaMapa[min/m+1][min%m])!=-1){
					if(minhodnota+hodnota<info[min+m].hodnotapol){			//ak je to kratsia cesta, tak ju prepise
						info[min+m].hodnotapol=minhodnota+hodnota;
						info[min+m].pred=min;														//ked bude cesta pod
						heapify(vrch,info[min+m].heaplok,info);				//upravi haldovu vlastnost
					}
				}
			}
			aktpocVrch--;
			prehladan++;
			
	}
	DrakPost=pridajDocesty(ciel,0,DrakPost,&DrakPost[1],info,n,m,0);		//najkratsiu cestu prida do cesty
	vycisti_pole(vrch,info,aktpocVrch,prehladane,prehladan);
	return DrakPost;
}

int **dijkstra(int min1,struct Vrchol *vrch, struct Informacie *info,int **FinalnaPostupnost, struct Vrchol *prehladane,int aktpocVrch, char **mojaMapa,int prepinac, int n,int m,int pocP, int drak[],int poradie,int **Generator, int stavGen, int PolohaG){
		//funckia prebehne dijkstrov algoritmus pre princezne
		int prehladan=0,minhodnota;
		int min=min1;
		int i;
		while(1){						//cyklus bude prebiehat, az kym nenavstivil kazdy vrchol
		min=vrch[0].suradnice;         //vezme minimum z heapu
		minhodnota=info[min].hodnotapol;
		if((aktpocVrch==0)||(info[min].hodnotapol==INT_MAX)){
			break;
			}
		int hodnota=daj_dlzku(mojaMapa[min/m][min%m]);
		prehladane[vrch[0].suradnice]=vrch[0];
		vrch[0]=vrch[aktpocVrch-1];
		relax(vrch,info,aktpocVrch-1);
		if(stavGen==1){					//ak je generator aktivny, a je na teleporte, tak navstivi kazdy teleport
				if((mojaMapa[min/m][min%m]>='0')&&(mojaMapa[min/m][min%m]<='9')){
					for(i=0;i<Generator[mojaMapa[min/m][min%m]-48][0];i++){
					if((minhodnota<info[Generator[mojaMapa[min/m][min%m]-48][i+1]].hodnotapol)&&(Generator[mojaMapa[min/m][min%m]-48][i+1]!=min)){
							info[Generator[mojaMapa[min/m][min%m]-48][i+1]].hodnotapol=minhodnota;
							info[Generator[mojaMapa[min/m][min%m]-48][i+1]].pred=min;
							heapify(vrch,info[Generator[mojaMapa[min/m][min%m]-48][i+1]].heaplok,info);
						}
				}
			}
			}	
			if((min%m)-1>=0){										//ak moze ist dolava <--, tak navstivi vchol
				if(daj_dlzku(mojaMapa[min/m][min%m-1])!=-1){
					if(minhodnota+hodnota<info[min-1].hodnotapol){	//ak nasiel kratsiu cestu, tak ju prepise
						info[min-1].hodnotapol=minhodnota+hodnota;
						info[min-1].pred=min;															//ked bude cesta na lavo
						heapify(vrch,info[min-1].heaplok,info);		//napravi haldovu vlastnost
					}		
				}
			}if((min%m)+1<m){										//ak moze ist doprava --> , tak navstivi vrchol
				if(daj_dlzku(mojaMapa[min/m][min%m+1])!=-1){
					if(minhodnota+hodnota<info[min+1].hodnotapol){	//ak nasiel kratsiu cestu, tak ju prepise
						info[min+1].hodnotapol=minhodnota+hodnota;
						info[min+1].pred=min;																//ked bude cesta na pravo					
						heapify(vrch,info[min+1].heaplok,info);		//napravi haldovu vlastnost	
					}
				}
			}
			if((min/m)-1>=0){										//ak moze ist nad seba ^, tak navstivi vrchol
				if(daj_dlzku(mojaMapa[min/m-1][min%m])!=-1){
					if(minhodnota+hodnota<info[min-m].hodnotapol){	//ak nasiel kratsiu cestu, tak ju prepise
						info[min-m].hodnotapol=minhodnota+hodnota;
						info[min-m].pred=min;																//ked bude cesta nad
						heapify(vrch,info[min-m].heaplok,info);		//napravi haldovu vlastnost
					}
				}
			}if((min/m)+1<n){										//ak moze ist pod seba v, tak navstivi vrchol
				if(daj_dlzku(mojaMapa[min/m+1][min%m])!=-1){
					if(minhodnota+hodnota<info[min+m].hodnotapol){	//ak nasiel kratsiu cestu, tak ju prepise	
						info[min+m].hodnotapol=minhodnota+hodnota;
						info[min+m].pred=min;														//ked bude cesta pod
						heapify(vrch,info[min+m].heaplok,info);		//napravi haldovu vlastnost	
					}
				}
			}
			
			aktpocVrch--;
			prehladan++;
			
}
	int k=0;
	if(prepinac==1){
		for(i=1;i<pocP+1;i++){										//pridam vsetky mozne cesty od draka k princeznam (draka medzi princezne nemozem ratat, preto ma vlastny cyklus)
			if(i<pocP){
			min=drak[i];
			if((info[min].pred==-1)){
				FinalnaPostupnost[k][0]=INT_MAX;
				FinalnaPostupnost[k][1]=2;
			}else{
			FinalnaPostupnost[k]=pridajDocesty(min,0,FinalnaPostupnost[k],&FinalnaPostupnost[k][1],info,n,m,1);
		}
	}
			if((stavGen==-1)&&(PolohaG!=0)&&(i==pocP)){
				FinalnaPostupnost[k]=pridajDocesty(PolohaG,0,FinalnaPostupnost[k],&FinalnaPostupnost[k][1],info,n,m,1);
		}
			k++;
		}
	}else{
	for(i=1;i<pocP+1;i++){											//pridam vsetky mozne cesty z princeznej k ostatnym princeznam
		if((i<pocP)){
		min=drak[i];
		if(info[min].pred==-1){
			FinalnaPostupnost[i-1][0]=INT_MAX;
			FinalnaPostupnost[i-1][1]=2;
		}else{
		if(i!=poradie){
			FinalnaPostupnost[i-1]=pridajDocesty(min,0,FinalnaPostupnost[i-1],&FinalnaPostupnost[i-1][1],info,n,m,1);
		}else{
			FinalnaPostupnost[i-1][0]=0;
		}
		}
	}
		if((stavGen==-1)&&(PolohaG!=0)&&(i==pocP)){
			FinalnaPostupnost[i-1]=pridajDocesty(PolohaG,0,FinalnaPostupnost[i-1],&FinalnaPostupnost[i-1][1],info,n,m,1);
		}
	}
}
	return FinalnaPostupnost;
}

int **vratPrinceznu(int iterator,struct Princezne *princ){			//funckia, ktora vrati ziadanu princeznu podla cisla
	int **akt;
	switch(iterator){
		case -1:
			akt=princ->dc;
			break;
		case 0:
			akt=princ->p1;
			break;
		case 1:
			akt=princ->p2;
			break;
		case 2:
			akt=princ->p3;
			break;
		case 3:
			akt=princ->p4;
			break;
		case 4:
			akt=princ->p5;
			break;
	}
	return akt;
}
int **vratPrinceznuPort(int iterator,struct PrinceznePortal *princ){	//funkcia, ktora vrati ziadanu princeznu(s aktivovanym generatorom) podla cisla
	int **akt;
	switch(iterator){
		case -7:
			akt=princ->GP;
			break;
		case -1:
			akt=princ->P1;
			break;
		case -2:
			akt=princ->P2;
			break;
		case -3:
			akt=princ->P3;
			break;
		case -4:
			akt=princ->P4;
			break;
		case -5:
			akt=princ->P5;
			break;
	}
	return akt;
}

void freeArray(int **a, int dlzka){					//funkcia ktora vyprazdni polia
	int i;
	for(i=0;i<dlzka;i++){
		free(a[i]);
	}
	free(a);
}
void najmensie(int *pole,int *aktP,int bola,int iterator,int pocV,int chybV, struct Princezne *princ,int *min){			//funkcia, ktora vysklada najkratsiu cestu podla suradnic
	int **akt=vratPrinceznu(iterator,princ);
	int i;
		if(chybV-1==0){													//ak som vytvoril postupnost vsetkych vrcholov idem pozriet dlzku mojej cesty
			int pomoc=0;
			for(i=-1;i<pocV-2;i++){
				int **smer;
				if(i==-1){
					smer=princ->dc;
				}else{
				smer=vratPrinceznu(pole[i],princ);
				}
				if(smer[pole[i+1]][0]==INT_MAX){						//ak sa mi nepodarilo dosiahnut ciel danou cestou
					pomoc=INT_MAX;
					break;
				}
				pomoc+=smer[pole[i+1]][0];
			}
			if(pomoc<*min)												//ak tato cesta je mensia ako predchadzajuca najmensia, ulozim si aktualnu
			{
				for(i=0;i<pocV-1;i++){
					aktP[i]=pole[i];
				}
				*min=pomoc;
				
			}
			return;
		}
	for(i=0;i<pocV-1;i++){												//cyklus ktory bude vytvarat zvysne permutacie
		pole[pocV-chybV]=i;
		if((akt[i][1]!=0)&&(((int)(bola/pow(10,i)))%10!=1)){
			bola+=1*pow(10,i);
			najmensie(pole,aktP,bola,i,pocV,chybV-1,princ,min);
			bola-=1*pow(10,i);
		}
	}
}
void najmensiePort(int *pole,int *aktP,int bola,int iterator,int pocV,int chybV, struct Princezne *princ,struct PrinceznePortal *sPort,int *min,int navstivilGen){
	//funkcia, ktora zosklada najkratsiu cestu, ale s aktivovanym generatorom
	int **akt;
	if(navstivilGen==0)
	akt=vratPrinceznu(iterator,princ);
	if(navstivilGen==1)
	akt=vratPrinceznuPort(iterator,sPort);
	int i;
		if(chybV==0){													//ak som zoskladal cestu so vsetkymi vrcholmi, mozem pozriet jej dlzku
			int pomoc=0;
			for(i=-1;i<pocV-1;i++){
				int **smer;
				if(i==-1){
					smer=princ->dc;
				}else{
					if(pole[i]<0){										//vybera princeznu z funkcie
						if(pole[i]==-7){
						smer=vratPrinceznuPort((pole[i]),sPort);
						}else{
						smer=vratPrinceznuPort((pole[i]),sPort);
						}
					}else{
						smer=vratPrinceznu(pole[i],princ);
					}	
				}
				if(pole[i+1]==-7){
				pomoc+=smer[pocV-1][0];	
				}else{
				int pomocka;
				if(pole[i+1]<0)pomocka=abs(pole[i+1])-1;
				else pomocka=pole[i+1];
				if(smer[pomocka][0]==INT_MAX){						//ak sa mi nepodarilo dosiahnut ciel danou cestou
					pomoc=INT_MAX;
				}
				pomoc+=smer[pomocka][0];
				}
			}
			if(pomoc<*min)											//ak je aktualna cesta mensia ako predchadzajuce minimum, ulozim si ju
			{
				for(i=0;i<pocV;i++){
					aktP[i]=pole[i];
				}
				*min=pomoc;
			}
			return;
		}
	for(i=0;i<pocV;i++){											//cyklus, ktory mi vytvara zvysne permutacie
		if(navstivilGen==0){
		pole[pocV-chybV]=i;
		}else{
		pole[pocV-chybV]=(i*-1)-1;
		}
		if((akt[i][1]!=0)&&(((int)(bola/pow(10,i)))%10!=1)){		//podmienka kontroluje ci som uz danu princeznu nezachranil
			bola+=1*pow(10,i);
			iterator=i;
			if(navstivilGen==1){
				iterator*=-1;
				iterator-=1;
			}
			if(i==pocV-1){
				navstivilGen=1;
				iterator=-7;
				pole[pocV-chybV]=-7;
			}
			najmensiePort(pole,aktP,bola,iterator,pocV,chybV-1,princ,sPort,min,navstivilGen);
			bola-=1*pow(10,i);
		}
}
}
void vypisPrinc(int **princ){				//funckia na vypis suradnic
	int i,j;
	printf("\n\n-----------------------\n");
	for(j=0;j<2;j++){
	for(i=0;i<princ[j][1];i++){
		printf("%d ",princ[j][i]);
	}
	printf("\n--------------------------\n");
}
}

void neskorsiD(int *Docas, int *gg,int *neskor, struct Princezne *princ, int iterator){			//funkcia, ktora vlozi suradnice k drakovi
		int i,j,pomoc1=0,pomoc2=0;
		for(i=-1;i<iterator-2;i++){
		int **aktP;
		if(i==-1){
			aktP=princ->dc;
		}else{
			aktP=vratPrinceznu(neskor[i],princ);
		}
		for(j=0;j<aktP[neskor[i+1]][1]-2;j++){
			Docas[pomoc1]=aktP[neskor[i+1]][j+2];
			pomoc1++;
		}
	}
	*gg=pomoc1;
}
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
	int drak[7]={0},i,j,iterator=1,pomocny=0,aktpocVrch=0,prehladan=0,min=0,dlzkaPostupnosti=0;
	int **portaly=(int**)malloc(10*sizeof(int*));							//alokujem si pole pre suradnice portalov
	for(i=0;i<10;i++){
		portaly[i]=(int*)malloc(m*n*sizeof(int));
		portaly[i][0]=0;
	}
	struct Vrchol *vrch=(struct Vrchol*)malloc(m*n*sizeof(struct Vrchol));
	struct Informacie *info=(struct Informacie*)malloc(m*n*sizeof(struct Informacie));
	struct Vrchol *prehladane=(struct Vrchol*)malloc(m*n*sizeof(struct Vrchol));
	int *konecnaPostupnostDrak;
	int GeneratorGlobal=-1,GeneratorLokacia=0;
	for(i=0;i<n;i++){													//cyklus, ktory najde body zaujmu, mojimi bodmi zaujmu su : Drak, Princezne, Generator, Teleporty
		for(j=0;j<m;j++){
			if(mapa[i][j]=='D'){									
				drak[0]=pomocny;
			}if(mapa[i][j]=='P'){
				drak[iterator]=pomocny;
				iterator++;
			}if(mapa[i][j]=='0'){
				portaly[0][0]+=1;
				portaly[0][portaly[0][0]]=pomocny;
			}if(mapa[i][j]=='1'){
				portaly[1][0]+=1;
				portaly[1][portaly[1][0]]=pomocny;
			}if(mapa[i][j]=='2'){
				portaly[2][0]+=1;
				portaly[2][portaly[2][0]]=pomocny;
			}if(mapa[i][j]=='3'){
				portaly[3][0]+=1;
				portaly[3][portaly[3][0]]=pomocny;
			}if(mapa[i][j]=='4'){
				portaly[4][0]+=1;
				portaly[4][portaly[4][0]]=pomocny;
			}if(mapa[i][j]=='5'){
				portaly[5][0]+=1;
				portaly[5][portaly[5][0]]=pomocny;
			}if(mapa[i][j]=='6'){
				portaly[6][0]+=1;
				portaly[6][portaly[6][0]]=pomocny;
			}if(mapa[i][j]=='7'){
				portaly[7][0]+=1;
				portaly[7][portaly[7][0]]=pomocny;
			}if(mapa[i][j]=='8'){
				portaly[8][0]+=1;
				portaly[8][portaly[8][0]]=pomocny;
			}if(mapa[i][j]=='9'){
				portaly[9][0]+=1;
				portaly[9][portaly[9][0]]=pomocny;
			}if(mapa[i][j]=='G'){
				GeneratorLokacia=pomocny;
			}
			info[pomocny].hodnotapol=INT_MAX;
			info[pomocny].pred=-1;
			vrch[pomocny].suradnice=pomocny;
			info[pomocny].heaplok=pomocny;
			pomocny++;
		}
	}
	for(i=0;i<10;i++){
		portaly[i]=(int*)realloc(portaly[i],(portaly[i][0]+1)*sizeof(int));
	}
	struct Princezne *princC=(struct Princezne*)malloc(sizeof(struct Princezne));
	struct Princezne *princCP=(struct Princezne*)malloc(sizeof(struct Princezne));
	struct PrinceznePortal *princP=(struct PrinceznePortal*)malloc(sizeof(struct PrinceznePortal));						//inicializujem polia pre princezne
	princC->dc=(int**)malloc((iterator+1)*sizeof(int*));
	princC->p1=(int**)malloc((iterator+1)*sizeof(int*));
	princC->p2=(int**)malloc((iterator+1)*sizeof(int*));
	princC->p3=(int**)malloc((iterator+1)*sizeof(int*));
	princC->p4=(int**)malloc((iterator+1)*sizeof(int*));
	princC->p5=(int**)malloc((iterator+1)*sizeof(int*));
	princCP->dc=(int**)malloc((iterator+1)*sizeof(int*));
	princCP->p1=(int**)malloc((iterator+1)*sizeof(int*));
	princCP->p2=(int**)malloc((iterator+1)*sizeof(int*));
	princCP->p3=(int**)malloc((iterator+1)*sizeof(int*));
	princCP->p4=(int**)malloc((iterator+1)*sizeof(int*));
	princCP->p5=(int**)malloc((iterator+1)*sizeof(int*));
	princP->GP=(int**)malloc((iterator+1)*sizeof(int*));
	princP->P1=(int**)malloc((iterator+1)*sizeof(int*));
	princP->P2=(int**)malloc((iterator+1)*sizeof(int*));
	princP->P3=(int**)malloc((iterator+1)*sizeof(int*));
	princP->P4=(int**)malloc((iterator+1)*sizeof(int*));
	princP->P5=(int**)malloc((iterator+1)*sizeof(int*));
	inicializujPrincezne(princC,m,n,iterator);
	inicializujPrincezne(princCP,m,n,iterator);
	inicializujPrinceznePort(princP,m,n,iterator);
	aktpocVrch=pomocny;
	info[0].hodnotapol=0;
	vrch[0].suradnice=0;
	konecnaPostupnostDrak=(int*)malloc(2*m*n*sizeof(int));																//inicializujem pole pre draka
	konecnaPostupnostDrak[0]=0;
	konecnaPostupnostDrak[1]=0;
	konecnaPostupnostDrak=Drakdijkstra(0,drak[0],vrch,info,konecnaPostupnostDrak,prehladane,aktpocVrch,mapa,n,m,portaly,-1);	//Urobim dijkstra pre draka bez pouzitia generatora
	int *DrakSportalmi=(int*)malloc(m*n*2*sizeof(int));
	DrakSportalmi[0]=INT_MAX;
	vycisti_pole(vrch,info,aktpocVrch,prehladane,prehladan);
	prehladan=0;
	aktpocVrch=pomocny;
	info[0].hodnotapol=0;
	if(GeneratorLokacia!=0){
		DrakSportalmi=Drakdijkstra(0,GeneratorLokacia,vrch,info,DrakSportalmi,prehladane,aktpocVrch,mapa,n,m,portaly,-1);		//Urobim dijkstra pre draka s pouzitim generatora, v prvej casti sa snazim dostat ku generatoru
		info[GeneratorLokacia].hodnotapol=0;
		DrakSportalmi[1]-=2;
		heapify(vrch,vrch[GeneratorLokacia].suradnice,info);
		DrakSportalmi=Drakdijkstra(GeneratorLokacia,drak[0],vrch,info,DrakSportalmi,prehladane,aktpocVrch,mapa,n,m,portaly,1);	//potom sa musim dostat od generatoru k drakovi
	}
	int *FinalnaPostupnost;
	if(DrakSportalmi[0]<=konecnaPostupnostDrak[0]){												//zistim, ktora cesta sa viac oplati, tu si zaznamenam
		FinalnaPostupnost=(int*)malloc((DrakSportalmi[1]+1)*sizeof(int));
		FinalnaPostupnost[0]=0;
		FinalnaPostupnost[1]=0;
		for(i=0;i<DrakSportalmi[1]-2;i++){
			FinalnaPostupnost[i+2]=DrakSportalmi[i+2];
		}
		dlzkaPostupnosti+=DrakSportalmi[1];
		GeneratorGlobal=1;
	}else{
		FinalnaPostupnost=(int*)malloc((konecnaPostupnostDrak[1]+1)*sizeof(int));
		dlzkaPostupnosti+=konecnaPostupnostDrak[1];
		FinalnaPostupnost[0]=0;
		FinalnaPostupnost[1]=0;
		for(i=0;i<konecnaPostupnostDrak[1]-2;i++){
			FinalnaPostupnost[i+2]=konecnaPostupnostDrak[i+2];
		}
	}
	for(i=0;i<iterator;i++){																	//teraz musim najst cestu od draka ku princeznam, najskor bez teleportu, ak bol uz teleport aktivovany, tak nemusim nic riesit
		min=drak[i];
		vycisti_pole(vrch,info,aktpocVrch,prehladane,prehladan);
		info[min].hodnotapol=0;
		prehladan=0;
		aktpocVrch=pomocny;
		heapify(vrch,min,info);
				switch(i){																		//kazdy case je pre inu princeznu a draka
					case 0:
						princC->dc=dijkstra(min,vrch,info,princC->dc,prehladane,aktpocVrch,mapa,1,n,m,iterator,drak,i,portaly,GeneratorGlobal,GeneratorLokacia);
						break;
					case 1:
						princC->p1=dijkstra(min,vrch,info,princC->p1,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,GeneratorGlobal,GeneratorLokacia);
						break;
					case 2:
						princC->p2=dijkstra(min,vrch,info,princC->p2,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,GeneratorGlobal,GeneratorLokacia);
						break;
					case 3:
						princC->p3=dijkstra(min,vrch,info,princC->p3,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,GeneratorGlobal,GeneratorLokacia);
						break;
					case 4:
						princC->p4=dijkstra(min,vrch,info,princC->p4,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,GeneratorGlobal,GeneratorLokacia);
						break;
					case 5:
						princC->p5=dijkstra(min,vrch,info,princC->p5,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,GeneratorGlobal,GeneratorLokacia);
						break;				
				}
	}
	int docasne[iterator+1],Sport[iterator+1],bezPort[iterator-1],SportNesk[iterator-1];
	int zatialNajm=INT_MAX;
	int prva;
	for(prva=0;prva<iterator-1;prva++){															//z danych ciest najdem najkratsiu moznu
		docasne[0]=prva;
		int bola=1*pow(10,prva);
		najmensie(&docasne,&bezPort,bola,prva,iterator,iterator-1,princC,&zatialNajm);
	}
	int PrincBezP=INT_MAX,PrincSP=INT_MAX,PrincPP=INT_MAX;
	if(((GeneratorGlobal==1)||(GeneratorGlobal==-1))&&(zatialNajm!=INT_MAX)){					//vycislim aktualnu najkratsiu cestu pre neskorsie porovnavanie
			PrincBezP=0;
	for(i=-1;i<iterator-2;i++){
		int **aktP;
		if(i==-1){
			aktP=princC->dc;
		}else{
			aktP=vratPrinceznu(bezPort[i],princC);
		}
		PrincBezP+=aktP[bezPort[i+1]][0];
		
	}
	}if((GeneratorGlobal==-1)&&(GeneratorLokacia!=0)){											//ak na mape existuje generator, ale este som ho stale neaktivoval, tak sa pokusim najst najkratsiu cestu s generatorom k princeznam
		PrincSP=0;
		for(i=0;i<=iterator;i++){
		if(i<=iterator){
		min=drak[i];
		}if(i==0){
			min=GeneratorLokacia;
		}
		vycisti_pole(vrch,info,aktpocVrch,prehladane,prehladan);
		info[min].hodnotapol=0;
		prehladan=0;
		aktpocVrch=pomocny;
		heapify(vrch,min,info);
				switch(i){																		//kazdy case je pre inu princeznu a generator
					case 0:
						princP->GP=dijkstra(min,vrch,info,princP->GP,prehladane,aktpocVrch,mapa,1,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 1:
						princP->P1=dijkstra(min,vrch,info,princP->P1,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 2:
						princP->P2=dijkstra(min,vrch,info,princP->P2,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 3:
						princP->P3=dijkstra(min,vrch,info,princP->P3,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 4:
						princP->P4=dijkstra(min,vrch,info,princP->P4,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 5:
						princP->P5=dijkstra(min,vrch,info,princP->P5,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;				
				}
	}
	zatialNajm=INT_MAX;
	for(prva=0;prva<iterator;prva++){												//najdem najkratsiu moznu cestu z mapy
		docasne[0]=prva;
		int bola=1*pow(10,prva);
		if(prva<iterator-1){
		najmensiePort(&docasne,&Sport,bola,prva,iterator,iterator-1,princC,princP,&zatialNajm,0);
		}else{
			docasne[0]=-7;
		najmensiePort(&docasne,&Sport,bola,-7,iterator,iterator-1,princC,princP,&zatialNajm,1);
		}
	}
	for(i=-1;i<iterator-1;i++){														//vycislim si tuto cestu pre neskorsie porovanie
		int **akt;
		if(i==-1){
			akt=princC->dc;
		}else{
			if(Sport[i]<0){
				akt=vratPrinceznuPort(Sport[i],princP);
			}else{
				akt=vratPrinceznu(Sport[i],princC);
			}
		}
		int pomocn=0;
		if(Sport[i+1]<0){
			if(Sport[i+1]!=-7)
			pomocn=abs(Sport[i+1])-1;
			else
			pomocn=iterator-1;
		}
		PrincSP+=akt[pomocn][0];
	}
	}
	if((GeneratorLokacia!=0)&&(DrakSportalmi[0]>konecnaPostupnostDrak[0])&&(DrakSportalmi[0]<t)){		//tu sa pokusim vyriesit poslednu moznost, a to ked cesta ku drakovi bude s aktivovanim generatora vacsia, ale celkovo sa viac oplati
		PrincPP=0;
		for(i=0;i<iterator;i++){
		min=drak[i];
		vycisti_pole(vrch,info,aktpocVrch,prehladane,prehladan);
		info[min].hodnotapol=0;
		prehladan=0;
		aktpocVrch=pomocny;
		heapify(vrch,min,info);
				switch(i){																				//kazdy case je pre inu princeznu a draka, ak sa generator aktivuje pri drakovi
					case 0:
						princCP->dc=dijkstra(min,vrch,info,princCP->dc,prehladane,aktpocVrch,mapa,1,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 1:
						princCP->p1=dijkstra(min,vrch,info,princCP->p1,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 2:
						princCP->p2=dijkstra(min,vrch,info,princCP->p2,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 3:
						princCP->p3=dijkstra(min,vrch,info,princCP->p3,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 4:
						princCP->p4=dijkstra(min,vrch,info,princCP->p4,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;
					case 5:
						princCP->p5=dijkstra(min,vrch,info,princCP->p5,prehladane,aktpocVrch,mapa,0,n,m,iterator,drak,i,portaly,1,GeneratorLokacia);
						break;				
				}
	}
		zatialNajm=INT_MAX;
		for(i=0;i<iterator-1;i++){									//najdem najkratsiu moznu cestu
		docasne[0]=i;
		int bola=1*pow(10,i);
		najmensie(&docasne,&SportNesk,bola,i,iterator,iterator-1,princCP,&zatialNajm);
	}
		for(i=-1;i<iterator-1;i++){									//vycislim si danu cestu, pre neskorsie porovnavanie
		int **akt;
		if(i==-1){
			akt=princCP->dc;
		}else{
			akt=vratPrinceznu(SportNesk[i],princCP);
		}
		PrincPP+=akt[SportNesk[i+1]][0];
	}
	}
	int gg=0;
	int Docas[m*n];
	if((PrincSP!=INT_MAX)&&(DrakSportalmi[0]!=INT_MAX)){		//pocitam celkovu dlzku cesty pre porovanie
		if(konecnaPostupnostDrak[0]<DrakSportalmi[0]){
			PrincSP+=konecnaPostupnostDrak[0];
		}else{
			PrincSP+=DrakSportalmi[0];
		}
	}if((PrincBezP!=INT_MAX)&&(konecnaPostupnostDrak[0]!=INT_MAX)){		//pocitam celkovu dlzku cesty pre porovanie
		if(konecnaPostupnostDrak[0]<DrakSportalmi[0]){
		PrincBezP+=konecnaPostupnostDrak[0];
		}else{
		PrincBezP+=DrakSportalmi[0];
		}
	}
	if(PrincBezP<PrincSP){											//ak je cesta bez generatora kratsia, tak si ju zvolim
		if((PrincPP!=INT_MAX)&&(PrincBezP>(PrincPP+DrakSportalmi[0]))){			//skusim, ci s neskorsim aktivovanim generatora, by nebola cesta lepsia
			free(FinalnaPostupnost);
			FinalnaPostupnost=(int*)malloc((DrakSportalmi[1])*sizeof(int));
			FinalnaPostupnost[0]=0;
			FinalnaPostupnost[1]=0;
			for(i=0;i<DrakSportalmi[1]-2;i++){
			FinalnaPostupnost[i+2]=DrakSportalmi[i+2];
			}
			dlzkaPostupnosti=DrakSportalmi[1];
			neskorsiD(&Docas,&gg,SportNesk,princCP,iterator);
		}else{														
		for(i=-1;i<iterator-2;i++){
		int **aktP;
		if(i==-1){
			aktP=princC->dc;
		}else{
			aktP=vratPrinceznu(bezPort[i],princC);
		}
		for(j=0;j<aktP[bezPort[i+1]][1]-2;j++){
			Docas[gg]=aktP[bezPort[i+1]][j+2];
			gg++;
		}
	}
	}
	}else{																			//ak je cesta s generatorom kratsia, tak si ju zvolim
		if((PrincPP!=INT_MAX)&&(PrincSP>(PrincPP+DrakSportalmi[0]))){				//skusim, ci s neskorsim aktivovanim generatora, by nebola cesta lepsia
			free(FinalnaPostupnost);
			FinalnaPostupnost=(int*)malloc((DrakSportalmi[1])*sizeof(int));
			FinalnaPostupnost[0]=0;
			FinalnaPostupnost[1]=0;
			for(i=0;i<DrakSportalmi[1]-2;i++){
			FinalnaPostupnost[i+2]=DrakSportalmi[i+2];
			}
			dlzkaPostupnosti=DrakSportalmi[1];
			neskorsiD(&Docas,&gg,SportNesk,princCP,iterator);
		}else{
		for(i=-1;i<iterator-1;i++){
		int **akt;
		if(i==-1){
			akt=princC->dc;
		}else{
			if(Sport[i]<0){
				akt=vratPrinceznuPort(Sport[i],princP);
			}else{
				akt=vratPrinceznu(Sport[i],princC);
			}
		}
		int pomocn;
		if(Sport[i+1]<0){
			if(Sport[i+1]!=-7)
			pomocn=abs(Sport[i+1])-1;
			else
			pomocn=iterator-1;
		}else{
			pomocn=Sport[i+1];
		}
		for(j=0;j<akt[pomocn][1]-2;j++){
			Docas[gg]=akt[pomocn][j+2];
			gg++;
		}
		}
	}
	}						
	FinalnaPostupnost=(int*)realloc(FinalnaPostupnost,(gg+dlzkaPostupnosti)*sizeof(int));
	for(i=0;i<gg;i++){															//Prehodim finalnu postupnost do pola, ktore vratim
		FinalnaPostupnost[dlzkaPostupnosti+i]=Docas[i];
	}
	dlzkaPostupnosti+=gg;
	*dlzka_cesty=dlzkaPostupnosti/2;
	free(vrch);																	//uvolnujem pamat
	free(info);
	free(konecnaPostupnostDrak);
	free(prehladane);
	free(DrakSportalmi);
	free(princC->dc);
	free(princC->p1);
	free(princC->p2);
	free(princC->p3);
	free(princC->p4);
	free(princC->p5);
	free(princP->GP);
	free(princP->P1);
	free(princP->P2);
	free(princP->P3);
	free(princP->P4);
	free(princP->P5);
	free(portaly);
	return FinalnaPostupnost;
}
char **vrat_mapu(int cislo){
	char **a;
	int i;
	switch(cislo){
		case 1:
			a=(char**)malloc(10*sizeof(char*));
			for(i=0;i<10;i++){
				a[i]=(char*)malloc(10*sizeof(char));
			}
			a[0]="CCHCNHCCHN";
			a[1]="NNCCCHHCCC";
			a[2]="DNCCNNHHHC";
			a[3]="CHHHCCCCCC";
			a[4]="CCCCCNHHHH";
			a[5]="PCHCCCNNNN";
			a[6]="NNNNNHCCCC";
			a[7]="CCCCCPCCCC";
			a[8]="CCCNNHHHHH";
			a[9]="HHHPCCCCCC";
			break;
		case 2:
			a=(char**)malloc(10*sizeof(char*));
			for(i=0;i<10;i++){
				a[i]=(char*)malloc(10*sizeof(char));
			}
			a[0]="CCHHHHHHGN";
			a[1]="NNCCCHHNNN";
			a[2]="DCHCC0CCCC";
			a[3]="CCHCCCCCCC";
			a[4]="0HHPCCCCCC";
			a[5]="NNNNCCHHHH";
			a[6]="CCCCNNNNNC";
			a[7]="CCPCCCCCHH";
			a[8]="CHNNCCCCCC";
			a[9]="CCPCHHCCCC";
			break;
		case 3:
			a=(char**)malloc(10*sizeof(char*));
			for(i=0;i<10;i++){
				a[i]=(char*)malloc(10*sizeof(char));
			}
			a[0]="CCCNPHHCC0";
			a[1]="HHCCNNNNNH";
			a[2]="CCCCNCCCDH";
			a[3]="CCCCCCCCCC";
			a[4]="CHHCCCCNNN";
			a[5]="NGCCCHHCCC";
			a[6]="PCCCCHHCC0";
			a[7]="NNNHHCCCNN";
			a[8]="PHHCCCCCCC";
			a[9]="CCCCC0CCCC";
			break;
		case 4:
			a=(char**)malloc(10*sizeof(char*));
			for(i=0;i<10;i++){
				a[i]=(char*)malloc(10*sizeof(char));
			}
			a[0]="CCNN0DNNCC";
			a[1]="CCCHHHCCCC";
			a[2]="CCGCCCHHNP";
			a[3]="HHCC0NCCH1";
			a[4]="CCCHHC1HHH";
			a[5]="CCCCCCCCCP";
			a[6]="HHCCC3CCHH";
			a[7]="CCCHHHNNCC";
			a[8]="PCCCHHCCCC";
			a[9]="CCHHH3PNNN";
			break;
		case 5:
			a=(char**)malloc(6*sizeof(char*));
			for(i=0;i<6;i++){
				a[i]=(char*)malloc(6*sizeof(char));
			}
			a[0]="CCCGNN";
			a[1]="NCNNNN";
			a[2]="NDNNNN";
			a[3]="N0HHHH";
			a[4]="NNNHHH";
			a[5]="NNN0PN";
			break;
		case 6:
			a=(char**)malloc(20*sizeof(char*));
			for(i=0;i<20;i++){
				a[i]=(char*)malloc(10*sizeof(char));
			}
			a[0]="CCHHCNNPCC";
			a[1]="CCHCCHHHHD";
			a[2]="CCHCC1HCCC";
			a[3]="CCCHHCCGCC";
			a[4]="HHHHCHHHHH";
			a[5]="1CCHCCCPCC";
			a[6]="NNHCCNNCCC";
			a[7]="CCC1CCHHCC";
			a[8]="CCCHHHHHCC";
			a[9]="CCCC1HHH0C";
			a[10]="NNNNNNNCCH";
			a[11]="PCCCHHHCCH";
			a[12]="NHHCCC0CCH";
			a[13]="NNNCCNNNHH";
			a[14]="CPCCHHHHNN";
			a[15]="0CCHHHHHCC";
			a[16]="CCCCHHCCN3";
			a[17]="CNNCPCCHNC";
			a[18]="NNCCCCNCCC";
			a[19]="0CCCCCCCCC";
			break;
		case 7:
			a=(char**)malloc(10*sizeof(char*));
			for(i=0;i<10;i++){
				a[i]=(char*)malloc(50*sizeof(char));
			}
			a[0]="CHCCNNCHCCCHHCHPCCHHCCCCC0CCCCHH1CCCCCCCCCCHHHHHNN";
			a[1]="NNNCCCHHCCCNHHHCHCHHHCCCCNNCCCHHHCCCCCCCNCCCCCCHHH";
			a[2]="0CCCHHCCNNCCCCHHHCCCCCC1CCCCC2CCCHHHHHHHHCCCDCCCCC";
			a[3]="CCCCCCHHHCGCCCCCCCHHCHCCCCCNNNCCCHHCCCPCCCHHHHHHHH";
			a[4]="CHHHHHHHHHHHCCCCNNPCCCCCCCCHHHHHHCCCCCHHCCC1CCCHHC";
			a[5]="CCHHCCCHHHHHHHCCC3CCCHHCCCHCCCHCHHHCCCNNNCCCC3HHHC";
			a[6]="CC0CCCCHHHCCCC1CCCCCNNNNNNNNNNCCCHHHCCCCCHHHHHHHCC";
			a[7]="CCCCCCCC8CCCCHHHCCNNNNNNNN8NNNNNNNNNNNNCCCCCCCCCCC";
			a[8]="CCCCCHHHCCPCCCCCCCCCCNNNCCCCPCCCCCCCCHHHCCCCCCCCCC";
			a[9]="CCCCCCCCCCHHHCCCCCCCCCCCCCCCCCCCHHHHCCNNNNNNCCCCCC";
			break;
		case 8:
			a=(char**)malloc(5*sizeof(char*));
			for(i=0;i<5;i++){
				a[i]=(char*)malloc(40*sizeof(char));
			}
			a[0]="PCCCHHCCNNNNCCCCHHCNNNNNNNNNNCCCCHHCCDCC";
			a[1]="CCCCCNHHCCCGCCC0CCCNNNHHHCCCC0CCCCCCCNNN";
			a[2]="CCCHHHHHHCCCNNCCCCHHCC1CCCHHCCCCCHHHHHP1";
			a[3]="HCCNPCCCCCHHCCCCCCC0CHHCCCPCCCCCCCCCCCCC";
			a[4]="1CCCHHHHHHHHHCCCCCCCCCCHHHCHCNNNNNCHHCHC";
			break;
		case 9:
			a=(char**)malloc(15*sizeof(char*));
			for(i=0;i<15;i++){
				a[i]=(char*)malloc(20*sizeof(char));
			}
			a[0]="CHCCHHCCNNHHC1GHCCNN";
			a[1]="CCNNCCCCCHHHHHHHCCCC";
			a[2]="CCCCCHHNNNNCCCHHHCCC";
			a[3]="CCC1CCCDCCHHHHHCNHNN";
			a[4]="HHHCCCNNNNNNNNCCHH1C";
			a[5]="PCCCHHC0CCCHHHCCNNCC";
			a[6]="NNCCCHHCCC1CCCHCCCCC";
			a[7]="NNNNNCCCCCCC0CCHHCCC";
			a[8]="PCCCHH1CCCHHCCCCC0CC";
			a[9]="NNNNCCCCCCHHHHHHHHHH";
			a[10]="HHHHCCCC0CCCHHHHHHH0";
			a[11]="CCCCCCCCCCCHHHHHNNNC";
			a[12]="CCCCCCCNNNNNPCCCCC1C";
			a[13]="CCCHHHCHHHHHHCCCCCCN";
			a[14]="NNNNNNNNNNCCCCCNNNNH";
			break;
		case 10:
			a=(char**)malloc(6*sizeof(char*));
			for(i=0;i<6;i++){
				a[i]=(char*)malloc(6*sizeof(char));
			}
			a[0]="PCNP0N";
			a[1]="CCNNNN";
			a[2]="CGCCN0";
			a[3]="CCCCND";
			a[4]="CPCCNN";
			a[5]="0CCCCP";
			break;
	}
	return a;
}
void testovac(){
	//mapy
	char **a;
	int test,dlzka,*HH,cas=0,i;
	while(1){
	printf("Zadajte cislo testu(1-10,11 ukonci)\n");
	scanf("%d",&test);
	switch(test){
		case 1:
			//test bez generatoru
			a=vrat_mapu(1);
			HH=zachran_princezne(a,10,10,30,&dlzka);
			break;
		case 2:
			a=vrat_mapu(2);
			//test s generatorom, ale najkratsia cesta bude bez neho
			HH=zachran_princezne(a,10,10,30,&dlzka);
			break;
		case 3:
			a=vrat_mapu(3);
			//test s generatorom, ale aktivuje sa az pri princeznach
			HH=zachran_princezne(a,10,10,30,&dlzka);
			break;
		case 4:
			a=vrat_mapu(4);
			//test s generatorom, ale aktivuje sa pri drakovi
			HH=zachran_princezne(a,10,10,30,&dlzka);
			break;
		case 5:
			//test s generatorom, kde cesta k drakovi bude s nim dlhsia, ale overall bude kratsia
			a=vrat_mapu(5);
			HH=zachran_princezne(a,6,6,50,&dlzka);
			break;
		case 6:
			a=vrat_mapu(6);
			HH=zachran_princezne(a,20,10,35,&dlzka);
			break;
		case 7:
			a=vrat_mapu(7);
			HH=zachran_princezne(a,10,50,30,&dlzka);
			break;
		case 8:
			a=vrat_mapu(8);
			HH=zachran_princezne(a,5,40,20,&dlzka);
			break;
		case 9:
			a=vrat_mapu(9);
			HH=zachran_princezne(a,15,20,30,&dlzka);
			break;
		case 10:
			//chcel som otestovat zabitie draka, zachranu princeznej bez priamej cesty, tj mohol pouzit len teleport
			a=vrat_mapu(10);
			HH=zachran_princezne(a,6,6,30,&dlzka);
			break;
		case 11:
			return;
			break;											
	}
	for(i=0;i<dlzka;i++){
		printf("%d %d %c\n",HH[i*2],HH[i*2+1],a[HH[i*2+1]][HH[i*2]]);
		if(a[HH[i*2+1]][HH[i*2]]=='H')cas+=2;
		else cas+=1;
	}
	free(HH);
	free(a);
	printf("%d\n",cas);
	cas=0;
	}
}
            
// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main()
{
   testovac();
  return 0;
}
