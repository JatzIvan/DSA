// uloha5-2.c -- Ivan Jatz, 21.10.2019 11:17

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Zaciatok{
	struct Prvky *dalsi;
};

struct Prvky{
	char *OP;
	struct Prvky *dalsi;
};

int vrat_kluc(char *string){
	int i,sucet=0;
	for(i=1;i<9;i++){
		sucet+=string[i-1]*i;
	}
	return sucet;
}

// spracuje cisla OP: vrati pocet najdenych duplikatov.
int vyhadzovac(char *a[], int n)
{
	struct Zaciatok POLE[n];
	struct Prvky *akt,*docasna;
	int i,identicke=0;
	for(i=0;i<n;i++){
		//printf("ddd\n");
		POLE[i].dalsi=NULL;
	}
	//printf("ddd\n");
	for(i=0;i<n;i++){
		int key=vrat_kluc(a[i]);
		//printf("%d %s\n",key, a[i]);
		akt=POLE[key%n].dalsi;
		while(1){
		if(akt==NULL){
			//printf("TU\n");
			POLE[key%n].dalsi=(struct Prvky*)malloc(sizeof(struct Prvky));
			akt=POLE[key%n].dalsi;
			akt->OP=strdup(a[i]);
			akt->dalsi=NULL;
			break;
		}else{
			if(akt->dalsi==NULL){
			if(strcmp(akt->OP,a[i])==0){
			//printf("Tam\n");
			identicke++;
			break;
			}
			akt->dalsi=(struct Prvky*)malloc(sizeof(struct Prvky));
			akt=akt->dalsi;
			akt->OP=strdup(a[i]);
			akt->dalsi=NULL;
			break;
			}else{
			//printf("tu\n");
			if(strcmp(akt->OP,a[i])==0){
				//printf("Tam\n");
				identicke++;
				break;
			}
			akt=akt->dalsi;
			}

		}
	}
	}
	for(i=0;i<n;i++){
		if(POLE[i].dalsi!=NULL){
			//printf("aaaa\n");
			akt=POLE[i].dalsi;
			while(1){
				docasna=akt->dalsi;
				free(akt->OP);
				free(akt);
				if(docasna==NULL)break;
				akt=docasna;
			}
		}
	}
	//printf("-------\n");
	//free(POLE);
	return identicke;
}

// ukazkovy test
int main(void)
{
	char **a = NULL, buf[100];
	int n = 0, len = 0;

	// nacitanie retazcov
	while (scanf("%s", buf) > 0)
	{
		if (n == len)
		{
			len = len + len + (len == 0);
			a = (char**)realloc(a, len * sizeof(char*));
		}
		a[n++] = strdup(buf);
	}

	printf("Pocet duplikatov: %d\n", vyhadzovac(a, n));
	return 0;
}
