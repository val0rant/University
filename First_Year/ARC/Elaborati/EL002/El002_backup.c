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
		//! calcolo fattoriale di N (da salvare in Num)
		MOV EBX, N
		CALL caso_base_fattoriale
		MOV Num, EAX
		//azzero i registri
		XOR EAX, EAX
		XOR EBX, EBX
		//? fino a qui so che va bene
		//EAX = N * Num (BASE) (moltiplicato per 4 perchè siamo in assembly e usiamo gli INT)
		MOV EAX, Num	//Num è calcolato con la funzione fattoriale (vedi sopra)
		MOV EDX, N
		MUL EDX			//BASE = N * Num
		MOV EDX, 4
		MUL EDX			//1 int = 4 bytes

		//ora EAX = BASE * 4

		//mette i primi N numeri nel vettore Perm, dopo tutte le possibili permutazioni
		MOV ECX, 1	//ECX parte da 1 per convenienza
		miniciclo:	PUSH EAX				//conserviamo il valore di EAX

					MOV EAX, ECX
					MUL EDX					//EDX = 4 (int size), EAX = ECX * 4
					MOV EBX, EAX			//EBX = ECX * 4

					POP EAX					//EAX = BASE = N * Num
					ADD EBX, EAX			//EBX = BASE + ECX * 4
					SUB EBX, 4				//EBX = BASE + (ECX - 1) * 4
					MOV Perm[EBX], ECX		//Perm[BASE + (ECX - 1) * 4] = ECX

					INC ECX
					CMP ECX, N
					JBE miniciclo

		//? fino a qui DOVREBBE andare bene

		//riazzero
		XOR ECX, ECX
		XOR EBX, EBX	//contatore per mettere elementi in array
		//EDX = 4 (da prima) per scala INT
		//EAX = BASE = N * Num (da prima)
		CALL algoritmo_heap
		JMP fine





		caso_base_fattoriale:	CMP EBX, 1
								JG calcolo_fattoriale
								MOV EAX, 1
								RET

		calcolo_fattoriale:		DEC EBX
								CALL caso_base
								INC EBX
								MUL EBX			//EAX = EAX * EBX
								RET

		//algoritmo di Heap

		//aggiungi elemento all'array nella prima posizione libera
		aggiungi_elemento:	PUSH ECX
					ciclo:	PUSH EAX
							MOV EAX, ECX
							MUL EBX
							MOV ECX, EAX
							POP EAX
							ADD EAX, ECX

							PUSH EBX
							MOV EBX, Perm[EAX]
							//MOV Perm[EDX*4], Perm[EAX]
							XCHG EBX, Perm[EDX*4]

							SUB EAX, ECX
							PUSH EAX
							MOV EAX, ECX
							DIV EBX
							MOV ECX, EAX
							POP EAX

							INC EDX
							INC ECX
							CMP ECX, N
							JB ciclo
							POP ECX
							RET



		algoritmo_heap:	PUSH EDX
						MOV EDX, N		//EDX è la dimensione
						CALL permutazioni
						RET

		permutazioni:	CMP EDX, 1
						JE dimensione_uno
						JMP scorri_vettore

						
						dimensione_uno: CALL aggiungi_elemento
										RET

						scorri_vettore:	DEC EDX
										CALL permutazioni
										PUSH EBX
										PUSH EAX
										MOV EAX, EDX
										PUSH EDX
										MOV EDX, 2
										DIV EDX
										CMP EDX, 1
										POP EDX
										JE dispari
										JMP pari

			dispari:		POP EAX
							PUSH EDX
							MOV EDX, EAX
							ADD EDX, EBX
							SUB EDX, 1
							PUSH ECX
							MOV ECX, Perm[EAX]
							XCHG ECX, Perm[EDX]
							POP ECX
							POP EDX
							JMP continua

			pari:			POP EAX
							PUSH EDX
							MOV EDX, EAX
							ADD ECX, EAX
							ADD EDX, EBX
							SUB EDX, 1
							PUSH EBX
							MOV EBX, Perm[EDX]
							XCHG Perm[ECX], EBX
							SUB ECX, EAX
							POP EBX
							POP EDX
							JMP continua

			continua:	POP EDX
						INC ECX
						CMP ECX, EBX
						JB scorri_vettore
						RET
										

		//ECX scorre il ciclo, EDX è per il modulo (resto) (pari o dispari) e come contatore per il vettore, EAX come indice base nel vettore

		fine:	NOP
	}

// Stampa su video
	{
		//i : n° di permutazioni (Num)	--> da calcolare con fattoriale o con tabella già determinata
		//j : n° di cifre per ogni permutazione (N)
		//k : indice per scorrere l'array (k = N*i + j)
		int i,j,k;
		printf("Permutazioni dei primi %d numeri naturali\n",N);
		//printf("%d", Num);
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
/*
procedure generate(k : integer, A : array of any) :
	if k = 1 then
		output(A)
	else
		// Generate permutations with kth unaltered
		// Initially k == length(A) --> k=N
		generate(k - 1, A)

		// Generate permutations for kth swapped with each k-1 initial
		for i : = 0; i < k - 1; i += 1 do
			// Swap choice dependent on parity of k (even or odd)
			if k is even then
				swap(A[i], A[k - 1]) // zero-indexed, the kth is at k-1
			else
				swap(A[0], A[k - 1])
				end if
				generate(k - 1, A)

				end for
				end if
*/