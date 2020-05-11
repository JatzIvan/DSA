// uloha2-2.c -- Ivan Jatz, 7.10.2019 12:03

#include <stdio.h>
#include<stdlib.h>
// Utriedi n cisel v poli a
void utried(int *a, int n)
{
	int pomocny[n],box[n];
	int i,j;
  for(i=0;i<n;i++){
    box[i]=0;
  }
  	for(j=0;j<n;j++){
  		box[(a[j])%n]++;
	  }
	for(j=1;j<n;j++){
		box[j]=box[j]+box[j-1];
	}
	for(j=n-1;j>=0;j--){
		pomocny[box[((a[j])%n)]-1]=a[j];
		box[(a[j])%n]--;
	}
	for(j=0;j<n;j++){
		a[j]=pomocny[j];
	}
  for(i=0;i<n;i++){
    box[i]=0;
  }

  for(j=0;j<n;j++){
  		box[(a[j]/n)%n]++;
	  }
	for(j=1;j<n;j++){
		box[j]=box[j]+box[j-1];
	}
	for(j=n-1;j>=0;j--){
		pomocny[box[((a[j]/n)%n)]-1]=a[j];
		box[(a[j]/n)%n]--;
	}
	for(j=0;j<n;j++){
		a[j]=pomocny[j];
	}
  }


int main(void)
{
  int i, *x, n;

  scanf("%d", &n);
  x = (int*)malloc(n * sizeof(int));
  for (i = 0; i < n; i++)
    scanf("%d", &x[i]);

  utried(x, n);
  
  printf("%d", x[0]);
  for (i = 1; i < n; i++)
  {
    printf(" %d", x[i]);
    if (x[i-1] > x[i])
    {
      printf(" -- CHYBA\n");
      return 0;
    }
  }
  printf("\n");
  return 0;
}
