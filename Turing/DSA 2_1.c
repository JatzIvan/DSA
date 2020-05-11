// uloha2-1.c -- Ivan Jatz, 6.10.2019 21:20

#include <stdio.h>
#include <stdlib.h>

#pragma GCC optimize ("O3")

long najdiMedian(int Pole[],int pocet){
	int i=0,j,prem=0;

	for(i=0;i<pocet-1;i++){
		for(j=0;j<pocet-1-i;j++){
		if(Pole[j]>Pole[j+1]){
			int pom=Pole[j];
			Pole[j]=Pole[j+1];
			Pole[j+1]=pom;
			prem=1;
		}
		}
	}
	//pomocna(Pole,0,pocet);

	return Pole[pocet/2];
}
void pomocna(int pole[],int spodna, int vrchna,int poc,int mod){
	if(spodna<vrchna){
		int pr=quicksort(pole,spodna,vrchna);
		if(mod==1){
		if((poc<=(vrchna-pr))){
			pomocna(pole,pr+1,vrchna,poc,mod);
		}else{
		pomocna(pole,pr+1,vrchna,poc,mod);
		pomocna(pole,spodna,pr-1,poc,mod);
		}
	}	if(mod==2){
		if((poc<=(vrchna-pr))){
			pomocna(pole,pr+1,vrchna,poc,mod);
		}else{
			pomocna(pole,pr+1,vrchna,poc,mod);
			pomocna(pole,spodna,pr-1,poc,mod);
		}
	}
	}
}
int quicksort(int pole[],int spodna,int horna){
	int kluc=pole[horna];
	int i,j=spodna-1;
	for(i=spodna;i<=horna-1;i++){
		if(pole[i]<kluc){
			j++;
			int pom=pole[j];
			pole[j]=pole[i];
			pole[i]=pom;
		}
	}
	pole[horna]=pole[j+1];
	pole[j+1]=kluc;
	return j+1;
}
long najdiKprvok(int pole[],int n){
	int i,j,pocDielov,dlzkaZ=n,poradie=0;
	if(n%5==0)
	pocDielov=n/5;
	else
	pocDielov=(n/5)+1;
	int Mediann[pocDielov],DocasPole[5];
	for(i=0;i<n;i++){
		DocasPole[i%5]=pole[i];
		if((((i+1)%5==0))||(i==n-1)){
			if(dlzkaZ>=5){
				Mediann[poradie]=najdiMedian(DocasPole,5);
				dlzkaZ-=5;
				poradie++;
			}else{
				Mediann[poradie]=najdiMedian(DocasPole,dlzkaZ);
				poradie++;
				dlzkaZ=0;
		}
		}
	}
	if(pocDielov>5)najdiKprvok(Mediann,pocDielov);
	if(pocDielov<=5)return najdiMedian(Mediann,pocDielov);
}
// urci sucet k najvacsich cisel z cisel cena[0],...,cena[n-1]
long sucet_k_najvacsich(int cena[], int n, int k)
{
	int pocDielov,i,dlzkaZ=n,poradie=0,j1=0,j2=0;
	long suc=0;
	int K=najdiKprvok(cena,n);
	int pole1[n],pole2[n];
	for(i=0;i<n;i++){
		if(cena[i]<K){
			pole1[j1]=cena[i];
			j1++;
		}else{
			pole2[j2]=cena[i];
			j2++;
		}
	}
	//printf("%d %d\n",j2,k);
	if(j2==k){
		for(i=0;i<j2;i++){
			suc+=pole2[i];
		}
	}if(j2>k){
		pomocna(pole2,0,j2-1,k,1);
		for(i=j2-1;i>=j2-k;i--){
			suc+=pole2[i];
		}
		//printf("%d %d %d\n",suc,k,j2);
	}if(j2<k){
		for(i=0;i<j2;i++){
			suc+=pole2[i];
		}
		pomocna(pole1,0,j1-1,k-j2,2);
		for(i=j1-1;i>=j1-(k-j2);i--){
			suc+=pole1[i];
		}
	}
	//printf("%d\n",suc);
	return suc;
}

int main(void)
{
  // tu si mozete nieco testovat
  int i, *x, n, k;

  scanf("%d %d", &n, &k);
  x = (int*)malloc(n * sizeof(int));
  for (i = 0; i < n; i++)
    scanf("%d", &x[i]);

  printf("%ld\n", sucet_k_najvacsich(x, n, k));
  return 0;
}
