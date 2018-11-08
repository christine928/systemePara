#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


float * tri(float * B, int taille);
float** tri_parallele (float ** B, int N, int K);
void tri_merge (float * B1, float * B2, int K);
void generator (float ** B1, int N, int K);
float * fusion(float * A, int tailleA, float * B, int tailleB);
void aff (float * tab, int taille);



int main(){
	int N, K;
	char *tab=calloc(40*sizeof(char), 1);
	//char tab[40]=" ";//calloc(40*sizeof(char), 1);
	float * tab2;
	int taille=0;
	int finCmde=0;
	printf("combien de tableaux?\n");
	scanf(" %d",&N);
	printf("quelle taille de tableaux?\n");
	scanf(" %d", &K);
	float ** tableau=calloc(N, sizeof(float*));
	int i, j;
	float temps=omp_get_wtime();
	for(i=0; i<N; i++)
	{
		tableau[i]=calloc(K, sizeof(float));
	}
	generator(tableau, N, K);
	tri_parallele(tableau, N, K);
	printf("ca a pris %f sec\n", omp_get_wtime()-temps);
	return 0;
}

float * tri(float * B, int taille)
{
	int i;
	float * C =malloc((taille*2)*sizeof(float));;

	if(taille>1)
	{
		//on copie dans B l'appel récursif la premiere partie du tableau
		memcpy(B, tri(B, (taille)/2), ((taille)/2)*sizeof(float));
		
		//on créé un pointeur sur la partie non traitee du tableau
		C=&(B[(taille)/2]);
		
		//on copie dans la deuxieme partie du tableau l'appel récursif de cette moitie de tableau
		memcpy(C, tri(C, (taille+1)/2), ((taille+1)/2)*sizeof(float));
		
		B=fusion(B, taille/2, C, taille-taille/2);//on fusionne les 2 tableaux en 1 seul
		
	}
	return B;
}

float** tri_parallele (float ** B, int N, int K)//on a choisi le tri fusion pour sa complexité en temps qui est de n.logn(n), avec n la taille du tableau d'entrée
{
	//B est un tableau de tableau, N la taille de B (donc le nombre de tableaux), K la taille de chacun des tableaux
	//int i, j, k, min, max;
	#pragma omp parallel for
	for(int i=0; i<N; i++)
	{
		printf("tab%d : \n", i);
		aff(B[i], K);	
		B[i]=tri(B[i], K);
	}
	/*for(j=0; j<N; j++)
	{
		k=(j%2);
		#pragma omp parallel firstprivate( k) 
		{
				#pragma omp for
				for(i=0; i<N/2; i++)
				{
					int b1=(k+2*i)%N;
					int b2=(k+2*i+1)%N;
					if(b1<b2)
					{
						min=b1;
						max=b2;
					}
					else
					{
						min=b2;
						max=b1;
					}
					#pragma omp critical
					{
						printf("99 min=%d, max=%d\n", min, max);
						aff(B[min], K);
						aff(B[max], K);
						DeuxTab tab2=tri_merge(B[min], B[max], K);
						B[min]=tab2.tab1;
						B[max]=tab2.tab2;
						printf("105\n");
						aff(B[min], K);
						aff(B[max], K);
					}
				}
		}
	}
	printf("\n\nfinal : \n");
	for(i=0; i<N; i++)
	{
		printf("tab%d : \n", i);
		aff(B[i], K);
	}*/
	for(int j=0; j<N; j++)
	{
		int k=j%2;
		#pragma omp parallel for
		for(int i =0; i<N/2; i++)
		{
			int b1=(k+2*i)%N;
			int b2=(k+2*i+1)%N;
			int min, max;
			if(b1<b2)
			{
				min=b1;
				max=b2;
			}
			else
			{
				min=b2;
				max=b1;
			}
			tri_merge(B[min], B[max], K);	
		}
	}
	printf("\n\nfinal : \n");
	for(int i=0; i<N; i++)
	{
		printf("tab%d : \n", i);
		aff(B[i], K);
	}

	return B;

}


float * fusion(float * A, int tailleA, float * B, int tailleB)//B peut etre d'une case plus grand que A, donc il faut avoir les 2 tailles
{
	float * C=malloc((tailleA+tailleB)*sizeof(float));
	int i=0, j=0, k=0, max, min;
	
	
	for(k=0; k<2*tailleB; k++)
	{
		if(i==tailleA)//si tous les elements de A sont placés, on met le prochain element de B
		{
			C[k]=B[j];
			j++;
		}
		else if(j==tailleB)//si tous les elements de B sont places, on met le prochain element de A
		{
			C[k]=A[i];
			i++;
		}
		else if(A[i]<B[j])//on met le plus petit, donc ici la prochaine valeur de A
		{
			C[k]=A[i];
			i++;
		}
		else //soit B est plus petit, donc on le met, soit les deux sont égaux, donc si on place la valeur de A ou la valeur de B avant, cela ne change rien
		{
			C[k]=B[j];
			j++;
		}
	}
	
	return C;
}
		

void tri_merge (float * B1, float * B2, int K)
{
	float * tab=fusion(B1, K, B2, K);
	
	memcpy(B1, tab, K*sizeof(float));
	memcpy(B2, &(tab[K]), K*sizeof(float));
	
	return ;
	
}

void generator( float ** B1, int N, int K)
{
	srand(time(NULL));
	int i, j;
	//azerty double region parallele
	omp_set_nested(1);
	#pragma omp parallel for
	for(i=0; i<N; i++)
	{
		#pragma omp parallel for
		for(j=0; j<K; j++)
		{			
			B1[i][j]=(random()%10000)/100.00;
		}
	}
	
	
}

void aff (float * tab, int taille)
{
	int i;
	for(i=0; i<taille; i++)
	{
		printf("tab[%d]=%.2f\n", i, tab[i]);
	}
	return;
}
