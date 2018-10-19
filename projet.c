#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>

int * tri (int * B, int fn);
void tri_merge (int ** B1, int ** B2);
void generator( int ** B1);
int * fusion(int * A, int * B, int tailleB);


int main(){
	int N, K;
	/*printf("combien de tableaux?\n");
	scanf(" %d",&K);
	printf("quelle taille de tableaux?\n");
	scanf(" %d", &N);*/
	
	int tab[6]={6, 5, 4, 1, 2, 3};
	tri(tab, 6);
	
	return 0;
}


int* tri (int * B, int taille)//on a choisi le tri fusion pour sa complexité en temps qui est de n.logn(n), avec n la taille du tableau d'entrée
{
	int i;
	int * C =malloc((taille*2)*sizeof(int));;
	printf("taille=%d\n", taille);
	if(taille>1)
	{
		printf("on va vhercher tab[%d..%d] : \n", 0, taille/2-1);
		for(i=0; i<taille/2; i++)
		{
			printf("B[%d]=%d\n", i, B[i]);
		}
		memcpy(B, tri(B, (taille)/2), ((taille)/2)*sizeof(int));
		printf("30, fin/2-1=%d\n", taille/2-1);
		for(i=0; i<taille/2; i++)
		{
			printf("B[%d]=%d\n", i, B[i]);
		}
		sleep(1);
		C=&(B[(taille)/2]);
		memcpy(C, tri(C, (taille+1)/2), ((taille+1)/2)*sizeof(int));
		printf("37, taille=%d\n", taille);
		for(i=0; i< taille/2+1;i++)
		{
			printf("B[%d]=%d  C[%d]=%d\n", i, B[i],  i, C[i]);
		}
		fusion(B, C, (taille+1)/2);
		printf("52 : \n");
		for(i=0; i<taille; i++)
		{
			printf("tab[%d]=%d\n", i, B[i]);
		}
	}
	return B;

}

int * fusion(int * A, int * B, int tailleB)//B peut etre d'une case plus grand que A
{
	int * C=malloc((2*tailleB)*sizeof(int));
	int i=0, j=0, k=0, max, min;
	printf("tailleB=%d\n", tailleB);
	for(i=0; i<tailleB; i++)
	{
		printf("fusionA[%d]=%d, fusionB[%d]=%d\n", i, A[i], i, B[i]);
	}
	i=0;
	
	for(k=0; k<2*tailleB; k++)
	{
		if(i==tailleB)
		{
			printf("i=tB, i=%d, j=%d, k=%d\n", i, j, k);

			C[k]=B[j];
			j++;
		}
		else if(j==tailleB)
		{
			printf("j=tB, i=%d, j=%d, k=%d\n", i, j, k);

			C[k]=A[i];
			i++;
		}
		
		
		else if(A[i]<B[j])
		{
			printf("a<b, i=%d, j=%d, k=%d\n", i, j, k);
			C[k]=A[i];
			i++;
		}
		else if(j<tailleB)
		{
			printf("a>b, i=%d, j=%d, k=%d\n", i, j, k);

			C[k]=B[j];
			j++;
		}
	}
	
	for(i=0; i<2*tailleB; i++)
	{
		printf("fusionC[%d]=%d\n", i, C[i]);
	}
	
	return C;
}
		

void tri_merge (int ** B1, int ** B2)
{
	
	
	
}

void generator( int ** B1)
{
	
	
	
}
