// zadanie1.c -- Ivan Jatz, 7.10.2019 11:02
#include <stdio.h>
#include <string.h>
#include <time.h>
struct memory{
	struct memory *next;
};

void *MemoryPointer;

void memory_init(void *ptr, unsigned int size);
void *memory_alloc(unsigned int size);
int memory_free(void *valid_ptr);
int memory_check(void *ptr);
void rozdel(int *ptr,int size,struct memory *pred, struct memory *point);
int SkontrolujVolne(struct memory *point);
void Usporiadaj(struct memory *point);
int kontrola_volnych();

void Usporiadaj(struct memory *point)								//Funkcia najde vstupnemu smerniku miesto v spajanom zozname
{ 	
	int *Zaciatok=(void*)MemoryPointer;		
	Zaciatok++;				
	struct memory *Akt_P,*Pred_P=NULL,*Prvy_P=(void*)Zaciatok;
	Akt_P=Prvy_P->next;
	int *Velkost_Bloku=(void*)point;
	Velkost_Bloku--;
	if(Akt_P==NULL){												//Ak je to prvy smernik v poli tak sa da na zaciatok inak zacne porovnavat
		Prvy_P->next=point;
		point->next=NULL;
	}else{
		while(1){
			int *Akt_Blok=(void*)Akt_P;
			Akt_Blok--;
			if(*Akt_Blok>*Velkost_Bloku){								//Ak je blok pamati ktory chcem usporiadat mensi ako blok pamati s ktorym porovnavam tak ho vlozim pred ten blok
				point->next=Akt_P;
			if(Pred_P!=NULL){										//Ak existuje predchadzajuci blok tak ho napojim ak nie tak sa tento moj blok pamati stava prvym
				Pred_P->next=point;
			}else{
				Prvy_P->next=point;
			}
			break;
		}
			if(Akt_P->next==NULL){										//Ak tento blok pamati najvacsi tak ho vlozim na koniec zoznamu
				Akt_P->next=point;
				point->next=NULL;
				break;
		}
			Pred_P=Akt_P;
			Akt_P=Akt_P->next;
		}
	}
}
void rozdel(int *ptr,int size,struct memory *pred,struct memory *point)			//Funkcia sluzi na rozdelenie velkeho bloku pamate na mensie kusky
{
	int *Zaciatok=(void*)MemoryPointer;
	Zaciatok++;
	struct memory *Prvy_P=(void*)Zaciatok,*Prenos_Point=point;
	int novyB;
	if(size%sizeof(int)!=0){													//Testujem ci velkost ktoru chcem alokovat je nasobkom 4. Ak je tak sa nic neznemi a ak nie je tak sa size zaokruhli na najblizsi nasobok 4
		novyB=((size/sizeof(int))+1)*sizeof(int);
	}else{
		novyB=size;
	}
	int staryB=*ptr&~0x1;
	int increment=(novyB/sizeof(int))+1;
	*ptr=novyB|1;
	if(novyB<staryB){
		if(staryB-novyB>sizeof(int)){
			*(ptr+increment)=staryB-novyB-sizeof(int);
		}
	}
	struct memory *novy=(void*)((void*)ptr+novyB+sizeof(int)*2);        //Vytvori novy pointer na ptr + velkost alokovaneho bloku + 2 krat velkost headeru (int) takze 8
	if((pred!=NULL)){
		pred->next=Prenos_Point->next;
		Usporiadaj(novy);												//Novy vytovreny blok usporiadam do spajaneho zoznamu volnych blokov
	}else{
		Prvy_P->next=novy;
		novy->next=Prenos_Point->next;
	}
}
int SkontrolujVolne(struct memory *ptr)													//Funkcia sluzi na spajanie volnych blokov ktore sa nachadzaju vedla seba
{
	int *Zaciatok=(void*)MemoryPointer;
	Zaciatok++;
	struct memory *Pomocny_P,*Pred_P=(void*)Zaciatok,*Akt_P=ptr;
	Pomocny_P=Pred_P->next;
	int *Prva_H=(void*)ptr,*Druha_H;
	int kon=1;										
	if(Pred_P->next==NULL)return 1;
	else Druha_H=(void*)Pomocny_P;
	Druha_H--;
	Prva_H--;
	int Increment1=((*Prva_H)/sizeof(int))+1;						//Zistujem velkost posunu pre smernik hlavicky
	while(1){
		int Increment2=((*Druha_H)/sizeof(int))+1;
		if(((Prva_H+Increment1)==Druha_H)||((Druha_H+Increment2)==Prva_H)){		//Zistujem ci sa oba volne bloky nachadzaju vedla seba
			if(Prva_H<Druha_H){													//Ak sa porovnavajuci smernik nachadza skor v pamati, tak pripocita size 2. smernika a odstrani ho zo zoznamu
				Pred_P->next=Pomocny_P->next;													
				*Prva_H=*Druha_H+sizeof(int)+*Prva_H;										
				kon=0;
			}else{																//Ak sa druhy smernik nachadza skor v bloku pamati, tak ho nastavi ako dominantny(1.) a taktiez ho odstrani zo zoznamu
				*Druha_H=*Prva_H+sizeof(int)+*Druha_H;
				Pred_P->next=Pomocny_P->next;
				Akt_P=Pomocny_P;
				Prva_H=(void*)Akt_P;
				Prva_H--;
				Increment1=((*Prva_H)/sizeof(int))+1;
				kon=0;
			}
			if(Pomocny_P->next==NULL)break;										//Pokracuje az do konca zoznamu
			Pomocny_P=Pomocny_P->next;
			Druha_H=(void*)Pomocny_P;
			Druha_H--;
		}else{
			if(Pomocny_P->next==NULL)break;
			Pred_P=Pomocny_P;
			Pomocny_P=Pomocny_P->next;
			Druha_H=(void*)Pomocny_P;
			Druha_H--;
		}
	}
	if(kon==0){																	//Ak som spojil nejake bloky, tak ich aj musim usporiadat
		Usporiadaj(Akt_P);
		return 0;
	}
	return 1;
}


int memory_check(void *ptr)								//Funkcia ktora sluzi na kontrolu smernika, zistuje ci sa nachadza v bloku pamati
{
	int *size=MemoryPointer;
	size--;
	if((ptr>=(MemoryPointer-sizeof(int)))&&(ptr<=(MemoryPointer+*size+sizeof(int))))return 1;
	else return 0;

}
int memory_free(void *valid_ptr)						//Funkcia ktora sluzi na uvolnenie bloku pamati podla smerniku
{
	
	int *akt=(void*)valid_ptr;
	akt--;
	if((*akt&1)==0){
		return 1;
	}else{
		*akt&=~(1<<0);									//Zmenim podlebny bit na 0 a tak urcim ze je blok volny
		struct memory *point=(void*)++akt;
		valid_ptr=NULL;
		if(SkontrolujVolne(point)==0)return 0;
		else{ 
			Usporiadaj(point);
			return 0;
		}
	}
}
void *memory_alloc(unsigned int size)					//Funkcia sluzi na priradenie pamate uzivatelovi
{
	int *Zaciatok=(void*)MemoryPointer;
	Zaciatok++;
	struct memory *Pred_P=NULL, *point=(void*)Zaciatok, *Prvy_P=(void*)Zaciatok;
	point=point->next;
	int *Akt_Blok=(void*)(point);
	Akt_Blok--;
	void* smer=NULL;
	if(point==NULL){
		//printf("Nedostatok pamate\n");
		return smer;
	}
	while(1){
		if(((*Akt_Blok&~0x1)>=size)&&((*Akt_Blok&~0x1)<(size+(sizeof(int)+sizeof(struct memory))))){        //Zisti ci blok ktory chcem alokovat je rovnakej alebo trosku vacsej velkosti (header(4B)+pointer(8B))
			*Akt_Blok=*Akt_Blok|1;
			smer=(void*)(++Akt_Blok);
			if(Pred_P==NULL)Prvy_P->next=point->next;
			else Pred_P->next=point->next;
			//printf("Alokoval sa presny blok\n");
			break;
		}
		if((*Akt_Blok&~0x1)>=(size+12)){																		//Ak je blok pamate vacsi ako pozadovana velkost tak dany blok rozdelim
			rozdel(Akt_Blok,size,Pred_P,point);
			smer=(void*)(++Akt_Blok);
			//printf("Blok volnej pamate som rozdelil\n");
			break;
		}
		if(point->next==NULL){																				//Ak som sa dostal na koniec zoznamu a nenasiel som dostatocne velky blok
			//printf("Nedostatok pamate\n");
			break;
		}
		Pred_P=point;
		point=point->next;
		Akt_Blok=(void*)point;
		Akt_Blok--;
	}
	return smer;
}
void memory_init(void *ptr, unsigned int size)		//Funkcia sluzi na inicializaciu bloku pamate, smernikov a hlaviciek
{
	MemoryPointer=(void*)ptr;
	int *prechod=(void*)MemoryPointer;
	*prechod=size; 									//prenechava si prvych 4 bytov pre velkost mojho bloku pamate
	struct memory *pointer=(void*)++prechod;		//tu si vytvaram pointer aby som sa vedel vratit k tomu bloku								
	prechod=(void*)++pointer;
	--pointer;
	*prechod=size-16;								//vytvaram si hlavicku pre prvy blok pamate a kedze viem ze ulozenie velkosti mi zabralo 12 byte-ov a hlavicka zabera dalsie 4 tak odcitam 16
	*prechod&=~(1<<0);
	pointer->next=(void*)++prechod;
	pointer=pointer->next;
	pointer->next=NULL;
}

void Testovac(int size){											//Tu zacinaju testovacie funkcie, ktore som si vytvoril na zjednodusenie testovania
	char pismena[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','r','s','t','x'};
	rand(time(0));
	printf("Velkost bloku %d\n",size);
	int maxVelkostBloku,i=0,priem=16,k=0;
	printf("Zadajte max velkost bloku na testovanie\n");
	scanf("%d",&maxVelkostBloku);
	printf("Zadajte pocet mallocov\n");
	int pocInit;
	scanf("%d",&pocInit);
	char *x[pocInit];
	//for(i=0;i<pocInit;i++){
	while(1){
		char *pom;
		pom=x[i];
		int r=(rand() % maxVelkostBloku);     //využívam pri teste s nahodnymi velkostami
		if(r<=7){
		r=8;
	}
		printf("%d %d %d\n",i,r,priem);
		pom=(char*)memory_alloc(r);
		//if(pom==NULL)break;						//pri teste s konstantymi dlzkami vyuzivam tuto podmienku na ukoncenie cyklu
		if(kontrola_volnych()==0)break;
		if(pom==NULL)i--;
		x[i]=pom;	
		if(k==19)k=0;
		if(pom){
		memset(pom,pismena[k],r);
		pom[r-1]='\0';
	}
		pom=NULL;
		k++;
		i++;
    }
	printf("Napiste pocet smernikov na free\n");
	int free;
	scanf("%d",&free);
	for(i=0;i<free;i++){
		int prem;
		char *pom;
		printf("Zadajte %d:",i);
		scanf("%d",&prem);
		printf("\n");
		printf("%p\n",x[prem-1]);
		pom=x[prem-1];
		if(pom){
		printf("%d\n",memory_free(pom));
		pom=NULL;
		x[prem-1]=pom;
		kontrola_volnych();
	}
	}
	kontrola_volnych();
	int *s=(void*)MemoryPointer;
	s++;
	struct memory* ptr=(void*)s;
	int pocet=0;
	while(1){
		if(ptr->next!=NULL){
			pocet++;
			ptr=ptr->next;
		}else break;
	}
	char *iu[pocet];
	ptr=(void*)s;
	if(pocet!=0)ptr=ptr->next;
	int *pomc;
	for(i=0;i<pocet;i++){
	pomc=(void*)ptr;
	pomc--;
	char *pom;
	pom=iu[i];
	pom=(char*)memory_alloc(*pomc);
	kontrola_volnych();
	memset(pom,'x',*pomc);
	ptr=(void*)s;
	ptr=ptr->next;
		}
	for(i=0;i<pocInit;i++){
		char *pom;
		pom=x[i];
		if(pom){
			printf("%s %d\n",pom, memory_check(pom));
		}
	}
	}
int kontrola_volnych(){
		printf("------------------------------------\n");
		int *Zaciatok=(void*)MemoryPointer;
		Zaciatok++;
		struct memory *akt=(void*)Zaciatok;
		int *x;
		if(akt->next!=NULL){
			while(1){
			akt=akt->next;
			if(akt==NULL)break;
			x=(void*)akt;
			x--;
			printf("%d %p\n",*x,akt);
		}
		printf("------------------------------------\n");
		return 1;
		}else{
			printf("------------------------------------\n");
			return 0;
		}
	}
// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main()
{
  printf("Zadajte velkost Bloku\n");
  int velkost;
  scanf("%d",&velkost);
  char region[velkost];
  memory_init(region, velkost);
  Testovac(velkost);
  char *sss1=(char*)memory_alloc(8563);
  memset(sss1,'a',8563);
  char *sss2=(char*)memory_alloc(522);
  memset(sss2,'b',522);
  char *sss3=(char*)memory_alloc(165);
  memset(sss3,'c',165);
  char *sss4=(char*)memory_alloc(52);
  memset(sss4,'d',52);
  char *sss5=(char*)memory_alloc(5148);
  memset(sss5,'e',5148);
  char *sss6=(char*)memory_alloc(75);
  memset(sss6,'f',75);
  char *sss7=(char*)memory_alloc(88);
  memset(sss7,'g',88);
  char *sss8=(char*)memory_alloc(42);
  memset(sss8,'h',42);
  kontrola_volnych();
  char *sss9=(char*)memory_alloc(1874);
  memset(sss9,'i',1874);
  char *sss10=(char*)memory_alloc(146);
  memset(sss10,'j',146);
  char *sss11=(char*)memory_alloc(3642);
  memset(sss11,'k',3642);
  char *sss12=(char*)memory_alloc(642);
  memset(sss12,'l',642);
  char *sss13=(char*)memory_alloc(830);
  memset(sss13,'m',830);
  char *sss14=(char*)memory_alloc(1360);
  memset(sss14,'n',1360);
  char *sss15=(char*)memory_alloc(8103);
  memset(sss15,'o',8103);
  char *sss16=(char*)memory_alloc(265);
  memset(sss16,'p',265);
  char *sss17=(char*)memory_alloc(9000);
  memset(sss17,'r',9000);
  char *sss18=(char*)memory_alloc(7601);
  memset(sss18,'s',7401);
  char *sss19=(char*)memory_alloc(367);
  memset(sss19,'t',367);
  char *sss20=(char*)memory_alloc(1396);
  memset(sss20,'x',1396);
  printf("//////////////////////////////\n");
  kontrola_volnych();
  memory_free(sss6);
  kontrola_volnych();
  memory_free(sss4);
  kontrola_volnych();
  memory_free(sss2);
  kontrola_volnych();
  memory_free(sss3);
  kontrola_volnych();
  memory_free(sss9);
  kontrola_volnych();
  memory_free(sss7);
  kontrola_volnych();
  memory_free(sss10);
  memory_free(sss20);
  memory_free(sss16);
  memory_free(sss12);
  kontrola_volnych();
  sss6=(char*)memory_alloc(24);
  memset(sss6,'-',24);
  kontrola_volnych();
  sss4=(char*)memory_alloc(52);
  memset(sss4,'-',52);
  kontrola_volnych();
  sss2=(char*)memory_alloc(42);
  memset(sss2,'-',42);
  kontrola_volnych();
  sss3=(char*)memory_alloc(16);
  memset(sss3,'-',16);
  kontrola_volnych();
  sss9=(char*)memory_alloc(20);
  memset(sss9,'-',20);
  kontrola_volnych();
  sss7=(char*)memory_alloc(16);
  memset(sss7,'-',16);
  sss10=(char*)memory_alloc(462);
  memset(sss10,'-',462);
  sss20=(char*)memory_alloc(974);
  memset(sss20,'-',974);
  char *sss21=(char*)memory_alloc(852);
  memset(sss21,'-',852);
  kontrola_volnych();
  char *sss22=(char*)memory_alloc(974);
  memset(sss22,'-',974);
  kontrola_volnych();
  char *sss23=(char*)memory_alloc(745);
  memset(sss23,'-',745);
  char *sss24=(char*)memory_alloc(215);
  memset(sss24,'-',215);
  char *sss25=(char*)memory_alloc(416);
  memset(sss25,'-',416);
  kontrola_volnych();
  char *sss26=(char*)memory_alloc(176);
  memset(sss26,'-',176);
  char *sss27=(char*)memory_alloc(192);
  memset(sss27,'-',192);
  char *sss28=(char*)memory_alloc(24);
  memset(sss28,'-',24);
  kontrola_volnych();
  printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",sss1,sss5,sss8,sss11,sss13,sss14,sss15,sss17,sss18,sss19);
  return 0;
}
