// uloha3-3.c -- Ivan Jatz, 7.10.2019 12:03
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct Mem{
  int koren,vyska;
  struct Mem *left,*right,*pred;
};
struct Mem *akt;
int vratvysku(struct Mem *koren){
  int h_l,h_p;
  if(koren->left!=NULL)h_l=koren->left->vyska+1;
  if(koren->left==NULL)h_l=0;
  if(koren->right!=NULL)h_p=koren->right->vyska+1;
  if(koren->right==NULL)h_p=0;

  if(h_l>h_p)return h_l;
  else return h_p;
}

int zistikoeff(struct Mem *koren){
  int hodnota=0;
	if(koren->left!=NULL){
		hodnota=1+koren->left->vyska;
	}else{
		hodnota=0;
	}
	if(koren->right!=NULL){
		hodnota=hodnota-(1+koren->right->vyska);
	}
	return hodnota;
}

struct Mem *RR(struct Mem *koren, struct Mem *pred){
  struct Mem *hlav=koren->left;
    if(pred==NULL){
      struct Mem *pom=hlav->pred,*pra=hlav->right;
      hlav->pred=NULL;
      koren->left=pra;
      koren->pred=hlav;
      hlav->right=koren;
      koren->vyska=vratvysku(koren);
      //hlav->right->vyska=vratvysku(hlav->right);
      hlav->vyska=vratvysku(hlav);
      if(koren->left!=NULL)
      koren->left->pred=koren;
      if(koren->right!=NULL)
      koren->right->pred=koren;
      if(hlav->left!=NULL)
      hlav->left->pred=hlav;
      if(hlav->right!=NULL)
      hlav->right->pred=hlav;
      return hlav;
    }else{
      struct Mem *pom=hlav->pred,*pra=hlav->right;
      if(pred->left==koren){
          pred->left=hlav;
          hlav->pred=pred;
      }if(pred->right==koren){
        pred->right=hlav;
        hlav->pred=pred;
      }
      hlav->pred=pred;
      koren->left=pra;
      koren->pred=hlav;
      hlav->right=koren;
      koren->vyska=vratvysku(koren);
      if(koren->left!=NULL)
      koren->left->pred=koren;
      if(koren->right!=NULL)
      koren->right->pred=koren;
      //hlav->right->vyska=vratvysku(hlav->right);
      hlav->vyska=vratvysku(hlav);
      if(hlav->left!=NULL)
      hlav->left->pred=hlav;
      if(hlav->right!=NULL)
      hlav->right->pred=hlav;
      return hlav;
    }
}
struct Mem *LL(struct Mem *koren, struct Mem *pred){
struct Mem *hlav=koren->right;
    if(pred==NULL){
      struct Mem *pom=hlav->pred,*pra=hlav->left;
      hlav->pred=NULL;
      koren->right=pra;
      koren->pred=hlav;
      hlav->left=koren;
      koren->vyska=vratvysku(koren);
      //hlav->right->vyska=vratvysku(hlav->right);
      hlav->vyska=vratvysku(hlav);
      if(koren->left!=NULL)
      koren->left->pred=koren;
      if(koren->right!=NULL)
      koren->right->pred=koren;
      if(hlav->left!=NULL)
      hlav->left->pred=hlav;
      if(hlav->right!=NULL)
      hlav->right->pred=hlav;
      return hlav;
    }else{
      struct Mem *pom=hlav->pred,*pra=hlav->left;
      if(pred->left==koren){
          pred->left=hlav;
          hlav->pred=pred;
      }if(pred->right==koren){
        pred->right=hlav;
        hlav->pred=pred;
      }
      hlav->pred=pred;
      koren->right=hlav->left;
      koren->pred=hlav;
      hlav->left=koren;
      koren->vyska=vratvysku(koren);
      //hlav->right->vyska=vratvysku(hlav->right);
      hlav->vyska=vratvysku(hlav);
      if(koren->left!=NULL)
      koren->left->pred=koren;
      if(koren->right!=NULL)
      koren->right->pred=koren;
      if(hlav->left!=NULL)
      hlav->left->pred=hlav;
      if(hlav->right!=NULL)
      hlav->right->pred=hlav;
      return hlav;
    }
}
struct Mem *RL(struct Mem *koren, struct Mem *pred){
    koren->right=RR(koren->right,koren);
    struct Mem *pomm;
    pomm=LL(koren,pred);
    if(pred!=NULL){
      if(pred->left==koren)pred->left=pomm;
      if(pred->right==koren)pred->right==pomm;
    }
    return pomm;
}
struct Mem *LR(struct Mem *koren, struct Mem *pred){
    koren->left=LL(koren->left,koren);
    struct Mem *pomm;
    pomm=RR(koren,pred);
    if(pred!=NULL){
      if(pred->left==koren)pred->left=pomm;
      if(pred->right==koren)pred->right==pomm;
    }
    return pomm;
}

struct Mem *vlozprvok(struct Mem *zaciatok,int hondnota,struct Mem *pred){
  
  if(zaciatok==NULL){
    zaciatok=(struct Mem*)malloc(sizeof(struct Mem));
    zaciatok->koren=hondnota;
    zaciatok->pred=pred;
    zaciatok->left=NULL;
    zaciatok->right=NULL;
    zaciatok->vyska=0;
    akt=zaciatok;
    return zaciatok;
  }else{
    if(hondnota<zaciatok->koren){
        zaciatok->left=vlozprvok(zaciatok->left,hondnota,zaciatok);
        //return zaciatok;
    }if(hondnota>zaciatok->koren){
        zaciatok->right=vlozprvok(zaciatok->right,hondnota,zaciatok);
        //return zaciatok;
    }if(hondnota == zaciatok->koren){
      akt=zaciatok;
    }
  }
  zaciatok->vyska=vratvysku(zaciatok);
  int koefff=zistikoeff(zaciatok);
  //printf("%d\n",koefff);
  if(koefff<-1){
    int pomk=zistikoeff(zaciatok->right);
    //printf("-+-+%d\n",pomk);
    if(pomk>0){
        zaciatok=RL(zaciatok,pred);
    }else{
        zaciatok=LL(zaciatok,pred);
    }
  }if(koefff>1){
    int pomk=zistikoeff(zaciatok->left);
    if(pomk>0){
        zaciatok=RR(zaciatok,pred);
    }else{
        zaciatok=LR(zaciatok,pred);
    }
  }
  zaciatok->vyska=vratvysku(zaciatok);
  return zaciatok;
}
void preorder(struct Mem *koren){
	    printf("%d->", koren->koren);
      //if(koren->pred!=NULL){
        //printf("/%d->",koren->pred->koren);
      //}else{
        //printf("->");
      //}
    if (koren->left != NULL)
        preorder(koren->left);
    if (koren->right != NULL)
        preorder(koren->right);
}

int vypis(struct Mem *zaciatok,int hodnota,int *min){
  //printf("%d\n",hodnota);
    struct Mem *pomoc;
    int zatial=INT_MAX;
    while(1){
      if(zaciatok->koren==hodnota)break;
      if(abs(zaciatok->koren-hodnota)<abs(zatial-hodnota))zatial=zaciatok->koren;
      if(abs(zaciatok->koren-hodnota)==abs(zatial-hodnota)){
        if(zatial>zaciatok->koren)zatial=zaciatok->koren;
      }
      if(hodnota<zaciatok->koren){
        zaciatok=zaciatok->left;
      }else if(hodnota>zaciatok->koren){
        zaciatok=zaciatok->right;
      }
    }
    pomoc=zaciatok;
    if(zaciatok->left!=NULL){
      pomoc=pomoc->left;
      while(1){
        if(pomoc->right==NULL)break;
        pomoc=pomoc->right;
      }
      if(abs(pomoc->koren-hodnota)<abs(zatial-hodnota))zatial=pomoc->koren;
      if(abs(pomoc->koren-hodnota)==abs(zatial-hodnota)){
        if(pomoc->koren<zatial)zatial=pomoc->koren;
      }
    }if(zaciatok->right!=NULL){
        pomoc=zaciatok;
        pomoc=pomoc->right;
        while(1){
          if(pomoc->left==NULL)break;
          pomoc=pomoc->left;
        }
      if(abs(pomoc->koren-hodnota)<abs(zatial-hodnota))zatial=pomoc->koren;
      if(abs(pomoc->koren-hodnota)==abs(zatial-hodnota)){
        if(pomoc->koren<zatial)zatial=pomoc->koren;
    }
}
*min=zatial;
}
int main(){
  struct Mem *zaciatok=(struct Mem*)malloc(sizeof(struct Mem));
  int hondnota;
  scanf("%d",&hondnota);
  printf("-1\n");
  zaciatok->koren=hondnota;
  zaciatok->pred=NULL;
  zaciatok->left=NULL;
  zaciatok->right=NULL;
  zaciatok->vyska=0;
  int min=zaciatok->koren;
  while((scanf("%d",&hondnota))>0){
    zaciatok=vlozprvok(zaciatok,hondnota,NULL);
    //printf("%d %d\n",hondnota,akt->koren);
    //printf("POSL");
    vypis(zaciatok,hondnota,&min);
    printf("%d\n",min);
    //printf("HODNOTA %d\n",hondnota);
    //printf("\n");
    //preorder(zaciatok);
    //printf("\n\n\n\n");
  }
  //preorder(zaciatok);
  return 0;
  //preorder(zaciatok);
}
