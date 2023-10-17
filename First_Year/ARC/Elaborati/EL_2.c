/********************************************************************************
 *                                                                              *
 *                   Architetture dei sistemi di Elaborazione                   *
 *                                                                              *
 ********************************************************************************

 Elaborato 2
 Descrizione: Generare tutte le permutazioni dei primi N numeri naturali.
              Le permutazioni generate vanno inserite all'interno di un unico
		    array di interi. Ad esempio, se N=3, l'array deve contenere:
		    {1,2,3,1,3,2,2,1,3,2,3,1,3,1,2,3,2,1}.

 ********************************************************************************/


#include <stdio.h>


void main()
{
// Variabili
int N=4;	// numero di interi (provare con valori <=6)
int Perm[4326];	// array permutazioni: la dimensione è sufficiente per N<=6
int Num; 	// Alla fine deve contenere il numero di permutazioni generate

// Blocco assembler
	__asm
	{
	}

// Stampa su video
	{
		int i,j,k;
		printf("Permutazioni dei primi %d numeri naturali\n",N);
		for (i=k=0;i<Num;i++)
		{
			for (j=0;j<N;j++)
			{
				printf("%3d",Perm[k++]);
			}
			printf("\n");
		}
	}
}
