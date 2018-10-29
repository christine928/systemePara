#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct 
{
	int * tab1;
	int * tab2;
}DeuxTab;

int * tri(int * B, int taille);
int** tri_parallele (int ** B, int N, int K);
DeuxTab tri_merge (int * B1, int * B2, int K);
void generator (int ** B1, int N, int K);
int * fusion(int * A, int tailleA, int * B, int tailleB);
void aff (int * tab, int taille);


int main(){
	int N, K;
	char *tab=calloc(40*sizeof(char), 1);
	//char tab[40]=" ";//calloc(40*sizeof(char), 1);
	int * tab2;
	int taille=0;
	int finCmde=0;
	printf("combien de tableaux?\n");
	scanf(" %d",&N);
	printf("quelle taille de tableaux?\n");
	scanf(" %d", &K);
	int ** tableau=calloc(N, sizeof(int*));
	int i, j;
	for(i=0; i<N; i++)
	{
		tableau[i]=calloc(K, sizeof(int));
		/*for(j=0; j<K; j++)
		{
			printf("tab%d[%d]=? ", i, j);
			scanf(" %d", &tableau[i][j]);
		}*/
	}
	generator(tableau, N, K);
	tri_parallele(tableau, N, K);
	
/*	// a supp apres, c est pour les tests
	printf("rentre ton tableaux séparé d'espaces (pas plus de 20 cases)\n");
	fgets(tab, 40*sizeof(char), stdin);	
	do
	{
		char *rep=strsep(&tab, " ");
		if(rep!=NULL)
		{
			if((strcmp(rep, "\n")!=0)&&(strcmp(rep, "")!=0))//si il y avait plusieurs espaces a la suite, on ne les garde pas
			{
				int i;
				taille++;
				if(taille==1)//donc à 0 avant incrementation
				{
					tab2=malloc(sizeof(int));
					tab2[0]=atoi(rep);
				}
				else
				{
					tab2=realloc(tab2, taille*sizeof(int));
					tab2[taille-1]=atoi(rep);
				}
			}
		}
		else
		{
			finCmde=1;
		}

	}while(finCmde==0);
	//int tab[7]={6, 5, 8, 1, 7, 3, 0};
	tab2=tri(tab2, taille);
	int i;
	for(i=0; i<taille; i++)
	{
		printf("tab2[%d]=%d\n", i, tab2[i]);
	}*/
	
	return 0;
}

int * tri(int * B, int taille)
{
	int i;
	int * C =malloc((taille*2)*sizeof(int));;
	//sleep(1);

	if(taille>1)
	{
		//on copie dans B l'appel récursif la premiere partie du tableau
		memcpy(B, tri(B, (taille)/2), ((taille)/2)*sizeof(int));
		
		
		//on créé un pointeur sur la partie non traitee du tableau
		C=&(B[(taille)/2]);
		
		//on copie dans la deuxieme partie du tableau l'appel récursif de cette moitie de tableau
		memcpy(C, tri(C, (taille+1)/2), ((taille+1)/2)*sizeof(int));
		
		B=fusion(B, taille/2, C, taille-taille/2);//on fusionne les 2 tableaux en 1 seul
		
	}
	return B;
}

int** tri_parallele (int ** B, int N, int K)//on a choisi le tri fusion pour sa complexité en temps qui est de n.logn(n), avec n la taille du tableau d'entrée
{
	//B est un tableau de tableau, N la taille de B (donc le nombre de tableaux), K la taille de chacun des tableaux
	int i, j, k, b1, b2, min, max;
	for(i=0; i<N; i++)
	{
		printf("tab%d : \n", i);
		aff(B[i], K);	
		B[i]=tri(B[i], K);
	}
	for(j=0; j<N; j++)//commence de 0 et pas de 1
	{
		k=(j%2);//+1 enleve
		for(i=0; i<N/2; i++)//commence de 0 et pas de 1, va jusque N/2 et non N/2-1
		{
			b1=(k+2*i)%N;//+1 enleve
			b2=(k+2*i+1)%N;//K->k //+1 enleve
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
			//printf("on merge les tab %d et %d\n", min, max);
			DeuxTab tab2=tri_merge(B[min], B[max], K);
			B[min]=tab2.tab1;
			B[max]=tab2.tab2;
			//printf("j=%d, i=%d, tabMin : \n", j, i);
			//aff(B[min], K);
			//printf("j=%d, i=%d, tabMax : \n", j, i);
			//aff(B[max], K);
		}
	}
	printf("\n\nfinal : \n");
	for(i=0; i<N; i++)
	{
		printf("tab%d : \n", i);
		aff(B[i], K);
	}
	return B;

}

int * fusion(int * A, int tailleA, int * B, int tailleB)//B peut etre d'une case plus grand que A, donc il faut avoir les 2 tailles
{
	int * C=malloc((tailleA+tailleB)*sizeof(int));
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
		

DeuxTab tri_merge (int * B1, int * B2, int K)
{
	int * tab=fusion(B1, K, B2, K);
	DeuxTab retour;
	retour.tab1=calloc(K, sizeof(int));
	retour.tab2=calloc(K, sizeof(int));
	memcpy(retour.tab1, tab, K*sizeof(int));
		
	memcpy(retour.tab2, &(tab[K]), K*sizeof(int));
	
	return retour;
	
}

void generator( int ** B1, int N, int K)
{
	srand(time(NULL));
	int i, j;
	for(i=0; i<N; i++)
	{
		for(j=0; j<K; j++)
		{
			
			B1[i][j]=random()%100;
			//printf("tab%d[%d]=%d \n", i, j, B1[i][j]);
		}
	}
	
	
}


void aff (int * tab, int taille)
{
	int i;
	for(i=0; i<taille; i++)
	{
		printf("tab[%d]=%d\n", i, tab[i]);
	}
	return;
}
