
#include <stdio.h>
#include <stdlib.h>
struct root{
	struct Cisla *next;
};
struct Cisla{
	struct Cisla *next;
	int hodnota;
};

// vrati 1 ak 's' je podmnozina 't', inak vrati 0.
int is_subset(int s[], int n, int t[], int m)
{
	//printf("%d %d\n",n,m);
	struct root pole[m];
	int i;
	for(i=0;i<m;i++){
		pole[i].next=NULL;
	}
	struct Cisla *akt,*docasna;
	int prem,kon=1;
	for(i=0;i<m;i++){
		if(t[i]<0){
			prem=t[i];
			prem*=-1;
		}else{
			prem=t[i];
		}
		if(pole[prem%m].next==NULL){
			pole[prem%m].next=(struct Cisla*)malloc(sizeof(struct Cisla));
			akt=pole[prem%m].next;
			akt->hodnota=t[i];
			akt->next=NULL;
		}else{
			akt=pole[prem%m].next;
			while(1){
				if(akt->next==NULL){
					akt->next=(struct Cisla*)malloc(sizeof(struct Cisla));
					akt=akt->next;
					akt->next=NULL;
					akt->hodnota=t[i];
					break;
				}else{
					akt=akt->next;
				}
			}
		}
	}
	for(i=0;i<n;i++){
		if(kon==0)break;
		if(s[i]<0){
			prem=s[i];
			prem*=-1;
		}else{
			prem=s[i];
		}
		if(pole[prem%m].next!=NULL){
			akt=pole[prem%m].next;
			while(1){
				if(akt->hodnota==s[i])break;
				else{
					if(akt->next==NULL){
						kon=0;
						break;
					}else{
						akt=akt->next;
					}
				}
			}
		}else{
			kon=0;
			break;
		}
	}
	for(i=0;i<m;i++){
		if(pole[i].next!=NULL){
			akt=pole[i].next;
			while(1){
				docasna=akt->next;
				free(akt);
				if(docasna==NULL)break;
				akt=docasna;
			}
		}
	}
	return kon;
}

// ukazkovy test
int main(void)
{
	int i, a[10], na, b[10], nb;
	scanf("%d", &na);
	for (i = 0; i < na; i++)
		scanf("%d", &a[i]);
	scanf("%d", &nb);
	for (i = 0; i < nb; i++)
		scanf("%d", &b[i]);
	if (is_subset(a, na, b, nb))
		printf("PODMNOZINA\n");
	else
		printf("NIE\n");
	return 0;
}
