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

//TO-DO LIST
//* TODO: - fattoriale
//* TODO: - inizializzazione vettore
//* TODO: - fix scala elementi
//  TODO: - algoritmo di Heap
//! FIX THAT FUCKING LOOP
		//* TODO: - aggiungi elemento
		//* TODO: - permutazioni
				//* TODO: - dispari/pari
				//  TODO: continua


int Perm[4326];

void main()
{
// Variabili
int N = 4;	// numero di interi (provare con valori <=6)
//! int Perm[4326];	// array permutazioni: la dimensione è sufficiente per N<=6
int Num; 	// Alla fine deve contenere il numero di permutazioni generate

// Blocco assembler
__asm {
	
	".section .data"
	global x
	x:
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0
		dd 0

	".section .text"
	global _start

	_start:
	
	//* calcolo fattoriale di N (da salvare in Num)
	MOV EBX, N //EBX = N
	CALL caso_base_fattoriale		//EAX = N! = Num
	MOV Num, EAX //Num = fattoriale(N)
	//EAX = Num
	//? OK NON CONTROLLARE PIU'
	/*
	//* azzero i registri
	XOR EAX, EAX
	XOR EBX, EBX
	XOR EDI, EDI
	XOR ESI, ESI
	*/

	//* ESI = N * Num * 4 = BASE * 4 (AUSILIARIO)
	//EAX = Num
	MOV EBX, N
	MUL EBX //BASE = N * Num
	MOV EBX, 4
	MUL EBX //1 int = 4 bytes
	MOV ESI, EAX

	//ora ESI = BASE * 4 (scala)

	//* EDI = N * Num * 4 - N * 4	(NUMERI)
	MOV EAX, N
	//EBX = 4
	MUL EBX
	MOV EDI, ESI
	SUB EDI, EAX

	//? OK FINO A QUI

	//? fino a qui SO che va bene
	//* mette i primi N numeri nel vettore Perm, dopo tutte le possibili permutazioni
	XOR ECX, ECX //ECX parte da 0
	CALL inizializza_vettore_numeri
	XOR ECX, ECX
	CALL inizializza_vettore_ausiliario

	//? fino a qui DOVREBBE andare bene

	//* riazzero i registri
	XOR EAX, EAX
	XOR EBX, EBX //contatore per mettere elementi in array
	XOR ECX, ECX
	XOR EDX, EDX

	//* algoritmo di Heap per calcolo permutazioni
	////MOV ESI, N	//ESI è la dimensione
	CALL aggiungi_elemento
	MOV ECX, 1
	CALL algoritmo_heap
	JMP fine

	//! *********************************************** FINE FUNZIONE MAIN (ASM)

	//* calcolo fattoriale di N (da salvare in Num)
	//* caso base
	caso_base_fattoriale:	
		CMP EBX, 1
		JG calcolo_fattoriale
		MOV EAX, 1
		RET

	//* chiamata ricorsiva
	calcolo_fattoriale:		
		DEC EBX
		CALL caso_base_fattoriale
		INC EBX
		MUL EBX //EAX = EAX * EBX
		RET

	// fine calcolo fattoriale

	//* mette i primi N elementi dopo tutte le possibili permutazioni nel vettore
	inizializza_vettore_numeri:	
		////PUSH EAX //conserviamo il valore di EAX

		MOV EAX, ECX
		MOV EBX, 4
		MUL EBX //EAX = ECX * 4

		//EAX è l'indice per scorrere il vettore
		ADD EAX, EDI //EAX = EDI + ECX * 4

		INC ECX

		MOV Perm[EAX], ECX //Perm[EDI + ECX * 4] = ECX + 1
		
		CMP ECX, N
		JB inizializza_vettore_numeri
		RET

	inizializza_vettore_ausiliario:
		MOV EAX, ECX
		MOV EBX, 4
		MUL EBX
		ADD EAX, ESI	//EAX = ESI + ECX * 4

		MOV Perm[EAX], 0

		INC ECX
		CMP ECX, N
		JB inizializza_vettore_ausiliario
		RET

	//fine inizializzazione vettore

	//? fino a qui PENSO che vada bene

	//* algoritmo di Heap
	//recap:
	//EAX = 0						(vario)
	//EBX = 0 						(indice che scorre vettore)
	//ECX = 0 						(scorre ciclo)
	//EDX = 0						(vario)
	//EDI 							(NUMERI)
	//ESI 							(AUSILIARIO)

	//* algoritmo di Heap
				
	algoritmo_heap:
		//? metto i controlli all'inizio
		CMP ECX, N
		JE arresta_algoritmo
	
		//c[t], con c vettore ausiliario, in questo caso le N posizioni a partire da ESI nel vettore Perm
		MOV EAX, ECX
		PUSH EBX
		MOV EBX, 4
		MUL EBX
		POP EBX
		ADD EAX, ESI	//EAX = ESI + ECX * 4

		CMP Perm[EAX], ECX
		JB minore
		JMP altrimenti

		minore:
			//* calcolo resto (pari/dispari)
			TEST ECX, 1

			JZ pari
			JNZ dispari

			//* n° elementi dispari
			pari:
				////PUSH EBX

				//EAX = EDI + 0
				MOV EAX, EDI

				JMP scambia_elementi

			//* n°elementi pari
			dispari:
				//EAX = EDI + c[t]
				
				MOV EAX, ECX
				PUSH EBX
				MOV EBX, 4
				MUL EBX
				POP EBX
				//EAX = ECX * 4

				ADD EAX, ESI		//EAX = ESI + ECX * 4
				MOV EDX, Perm[EAX]	//    = c[t]
				MOV EAX, EDX
				ADD EAX, EDI		//EAX = EDI + c[t]

				JMP scambia_elementi

			scambia_elementi:
				//EDX = EDI + ECX * 4
				PUSH EBX
				PUSH EAX

				MOV EAX, ECX
				MOV EBX, 4
				MUL EBX		//EAX = ECX * 4
				MOV EDX, EAX				
				ADD EDX, EDI	//EDX = EDI + ECX * 4

				POP EAX

				////PUSH EBX
				PUSH ECX

				//EAX, ECX variabili temporanee
				//* XCHG Perm[EBX], Perm[EDX]
				MOV EBX, Perm[EAX]
				MOV ECX, Perm[EDX]

				XCHG EBX, ECX

				MOV Perm[EAX], EBX
				MOV Perm[EDX], ECX

				POP ECX
				POP EBX

				////POP EBX
			CALL aggiungi_elemento

			MOV EAX, ECX
			PUSH EBX
			MOV EBX, 4
			MUL EBX			//EAX = ECX * 4
			POP EBX
			
			ADD EAX, ESI			//EAX = ESI + ECX * 4
			MOV EDX, Perm[EAX]		//EDX = c[i]
			INC EDX					//(c[i])++
			MOV Perm[EAX], EDX

			MOV ECX, 1				

			////JMP continua
			JMP algoritmo_heap


		altrimenti:
			MOV EAX, ECX
			PUSH EBX
			MOV EBX, 4
			MUL EBX			//EAX = ECX * 4
			POP EBX
			ADD EAX, ESI	//EAX = ESI + ECX * 4

			MOV Perm[EAX], 0
			INC ECX
			JMP algoritmo_heap

			/*
			//* passa alla permutazione successiva (ciclo seguente)
			continua:
				CMP ECX, N
				JB algoritmo_heap
				RET
			*/
		arresta_algoritmo:
			RET

	// fine calcolo permutazioni

	//* aggiungi elemento all'array nella prima posizione libera
	aggiungi_elemento:
		//conserva registri
		PUSH EAX
		PUSH ECX
		PUSH EDX

		//inizializza contatore ciclo
		XOR ECX, ECX
		
		ciclo_aggiunta:
			CMP EBX, ESI //fine posizioni libere
			JE fine_aggiunta

			CMP ECX, N		//fine elementi da aggiungere
			JE fine_aggiunta
			//? ho messo i controlli all'inizio, ma credo non cambi nulla


			//?MOV EDX, EDI

			////conserva registri
			

			MOV EAX, ECX

			//?PUSH EDX
			//conserva EBX
			PUSH EBX

			MOV EBX, 4
			MUL EBX
			//EAX = ECX * 4

			//ripristina EBX
			POP EBX
			//?POP EDX

			ADD EAX, EDI // EAX = EDI + ECX * 4

			////PUSH EDX

			//aggiunge elemento a vettore
			//* MOV Perm[EDX*4], Perm[EAX]
			MOV EDX, Perm[EAX]

			//EBX = contatore posizione in vettore

			//* MOV Perm[EDX*4], Perm[EAX]
			MOV Perm[EBX], EDX

			//ripristina registri
			////POP EDX
			

			//ciclo seguente
			ADD EBX, 4 //aggiorniamo il contatore tenendo conto della dimensione degli int
			INC ECX
			
			/*
			//? i controlli sono all'inizio
			CMP EBX, ESI //fine posizioni libere
			JE fine_aggiunta

			CMP ECX, N		//fine elementi da aggiungere
			JB ciclo_aggiunta
			//JMP fine_aggiunta
			*/
			JMP ciclo_aggiunta

		fine_aggiunta:
			//ripristina registri
			POP EDX
			POP ECX
			POP EAX
			RET

	// fine aggiungi elemento
	//* fine blocco ASM

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