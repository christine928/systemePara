#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>

int * tri (int * B, int fn);
void tri_merge (int ** B1, int ** B2);
void generator( int ** B1);

int main(){
	int N, K;
	printf("combien de tableaux?\n");
	scanf(" %d",&K);
	printf("quelle taille de tableaux?\n");
	scanf(" %d", &N);
	
	int tab[6]={6, 5, 4, 1, 2, 3};
	tri(tab, 6);
	
	return 0;
}


int* tri (int * B, int taille)//on a choisi le tri fusion pour sa complexité en temps qui est de n.logn(n), avec n la taille du tableau d'entrée
{
	int i;
	int * C =B;
	if(taille>1)
	{
		printf("on va vhercher tab[%d..%d] : \n", 0, taille/2);
		for(i=0; i<=taille/2; i++)
		{
			printf("B[%d]=%d\n", i, B[i]);
		}
		memcpy(B, tri(B, (taille)/2), ((taille)/2)*sizeof(int));
		printf("30, fin/2=%d\n", taille/2);
		for(i=0; i<=taille/2; i++)
		{
			printf("B[%d]=%d\n", i, B[i]);
		}
		sleep(1);
		C=&(B[(taille)/2+1]);
		memcpy(C, tri(C, taille), ((taille)/2)*sizeof(int));
		printf("37\n");
		for(i=0; i<= taille/2;i++)
		{
			printf("C[%d]=%d\n", i, B[i]);
		}
		
		
		return B;
	}
	
}

void tri_merge (int ** B1, int ** B2)
{
	
	
	
}

void generator( int ** B1)
{
	
	
	
}
