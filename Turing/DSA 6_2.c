// uloha6-2.c -- Ivan Jatz, 28.10.2019 11:16
#include <string.h>
#include <stdio.h>

struct Vrchol{
	char znak;
	char zaf;
};

struct Vrchol pole[1000][1000];

void hladaj_cestu(int x,int y, int pohyb){
	pole[x][y].zaf='y';
	if((pohyb!=0)&&((pole[x+1][y].znak==NULL)||(pole[x][y+1].znak==NULL)||(pole[x-1][y].znak==NULL)||(pole[x][y-1].znak==NULL))){
		pole[x][y].znak='*';
		return;
	}if((x-1>=0)&&(pole[x-1][y].zaf!='y')&&(pole[x-1][y].znak=='.')&&(pohyb!=1)){
		hladaj_cestu(x-1,y,3);
		//pohyb=3;
		if(pole[x-1][y].znak=='*'){
			pole[x][y].znak='*';
			return;
		}
	}if((y-1>=0)&&(pole[x][y-1].zaf!='y')&&(pole[x][y-1].znak=='.')&&(pohyb!=2)){
		hladaj_cestu(x,y-1,4);
		//pohyb=4;
		if(pole[x][y-1].znak=='*'){
			pole[x][y].znak='*';
			return;
		}
	}
		if((pole[x+1][y].zaf!='y')&&(pole[x+1][y].znak=='.')&&(pohyb!=3)){
		hladaj_cestu(x+1,y,1);
		//pohyb=1;
		if(pole[x+1][y].znak=='*'){
			pole[x][y].znak='*';
			return;
		}
	}
	if((pole[x][y+1].zaf!='y')&&(pole[x][y+1].znak=='.')&&(pohyb!=4)){
		hladaj_cestu(x,y+1,2);
		//pohyb=2;
		if(pole[x][y+1].znak=='*'){
			pole[x][y].znak='*';
			return;
		}
	}
}

int main(){
	int riadok=0,i=0;
	/*while((scanf("%s",pole[riadok]))>0){
		riadok++;
	}*/
	int j=0;
	char x;
	while((scanf("%c",&x))>0){
		j=0;
		pole[i][j].znak=x;
		for(j=1;;j++){
		scanf("%c",&x);
		if(x=='\n'){
			pole[i][j].znak='\n';
			break;
		}
		pole[i][j].znak=x;
	}
	i++;
	}
	//printf("%c\n",pole[0][1]);
	hladaj_cestu(0,1,0);
	int k;
	for(k=0;k<i;k++){
		for(j=0;;j++){
		if(pole[k][j].znak=='\n')break;
		printf("%c",pole[k][j].znak);
		}
		printf("\n");
	}
	return 0;
}
