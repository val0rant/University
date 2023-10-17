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
__asm 
{
	//* calcolo fattoriale di N (da salvare in Num)
	MOV EBX, N //EBX = N
	CALL caso_base_fattoriale
	MOV Num, EAX //Num = fattoriale(N)

	//* azzero i registri
	XOR EAX, EAX
	XOR EBX, EBX
	XOR EDI, EDI //? non so quanto potrò usarli entrambi
	XOR ESI, ESI //? nel dubbio li azzero

	//* EAX = N * Num = BASE (moltiplicato per 4 perchè siamo in assembly e usiamo gli INT)
	MOV EAX, Num //Num è calcolato con la funzione fattoriale (vedi sopra)
	MOV EDX, N
	MUL EDX //BASE = N * Num
	MOV EDI, 4
	MUL EDI //1 int = 4 bytes

	//ora EAX = BASE * 4 (scala)

	//? fino a qui SO che va bene
	//* mette i primi N numeri nel vettore Perm, dopo tutte le possibili permutazioni
	XOR ECX, ECX //ECX parte da 0
	CALL inizializza_vettore

	//? fino a qui DOVREBBE andare bene

	//* riazzero i registri
	XOR EBX, EBX //contatore per mettere elementi in array
	XOR ECX, ECX
	XOR EDX, EDX
	//EDI = 4 (da prima) per scala INT
	//EAX = BASE * 4 = N * Num * 4 (da prima)

	//* algoritmo di Heap per calcolo permutazioni
	MOV ESI, N //ESI è la dimensione
	CALL algoritmo_heap
	JMP fine

	//*********************************************** FINE FUNZIONE MAIN (ASM)

	//! calcolo fattoriale di N (da salvare in Num)
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

	//! mette i primi N elementi dopo tutte le possibili permutazioni nel vettore
	inizializza_vettore:	
		PUSH EAX //conserviamo il valore di EAX

		MOV EAX, ECX
		MUL EDI //EDI = 4 (int size), EAX = ECX * 4
		MOV EBX, EAX //EBX = ECX * 4

		//ripristino EAX
		POP EAX //EAX = BASE * 4 = N * Num * 4

		//EBX è l'indice per scorrere il vettore
		ADD EBX, EAX //EBX = BASE * 4 + ECX * 4

		INC ECX

		MOV Perm[EBX], ECX //Perm[BASE * 4 + ECX * 4] = ECX + 1
		
		CMP ECX, N
		JBE inizializza_vettore
		RET

	//fine inizializzazione vettore

	//? fino a qui PENSO che vada bene

	//! algoritmo di Heap
	//recap:
	//EAX = BASE * 4 = N * Num * 4	(indice BASE nel vettore)
	//EBX = 0 						(indice che scorre vettore)
	//ECX = 0 						(scorre ciclo)
	//EDX = 0						(modulo (resto) -> pari/dispari)
	//EDI = 4						(convertitore scala int)
	//ESI = 0						(dimensione) (ausiliario)

	//* algoritmo di Heap
	algoritmo_heap:	
		////CALL permutazioni
		////RET
		//// fine_algoritmo:
		//// RET

		//// fine algoritmo di Heap

		//* calcolo permutazioni elementi vettore
		////permutazioni:	
		//* blocco if
		CMP ESI, 1
		JE dimensione_uno
		JMP ciclo_scorri_vettore

		//* caso base
		dimensione_uno: 
			CALL aggiungi_elemento
			RET

		//* scorri il vettore
		ciclo_scorri_vettore:
			DEC ESI		//chiamata ricorsiva su (dimensione-1)
			CALL algoritmo_heap
			INC ESI		//ripristina la dimensione della chiamata ricorsiva corrente

			//* calcolo resto (pari/dispari)
			TEST ESI, 1

			JZ pari
			JNZ dispari

			//* n° elementi dispari
			dispari:
				PUSH EBX

				//EBX = BASE * 4 + 0
				MOV EBX, EAX

				JMP scambia_elementi

			//* n°elementi pari
			pari:
				PUSH EBX

				//EBX = BASE * 4 + ECX * 4
				PUSH EAX
				
				MOV EAX, ECX
				MUL EDI
				MOV EBX, EAX

				POP EAX

				ADD EBX, EAX

				JMP scambia_elementi

			scambia_elementi:
				//EDX = BASE * 4 + ESI * 4 - 4
				PUSH EAX

				MOV EAX, ESI
				MUL EDI
				MOV EDX, EAX
				SUB EDX, 4

				POP EAX

				ADD EDX, EAX

				PUSH EAX
				PUSH ECX

				//EAX, ECX variabili temporanee
				//* XCHG Perm[EBX], Perm[EDX]
				MOV EAX, Perm[EBX]
				MOV ECX, Perm[EDX]

				XCHG EAX, ECX

				MOV Perm[EBX], EAX
				MOV Perm[EDX], ECX

				POP ECX
				POP EAX

				POP EBX

			//* passa alla permutazione successiva
			////continua:
				//ciclo seguente
				INC ECX
				CMP ECX, ESI //ESI è la dimensione del vettore al momento considerato dall'algoritmo
				JB ciclo_scorri_vettore
		RET

	// fine calcolo permutazioni

	//* aggiungi elemento all'array nella prima posizione libera
	aggiungi_elemento:	
		//conserva registri
		PUSH ECX
		PUSH EDX

		//inizializza contatore ciclo
		XOR ECX, ECX
		
		ciclo_aggiunta:
			MOV EDX, EAX	//EDX = BASE * 4

			//conserva registri
			PUSH EAX

			MOV EAX, ECX

			PUSH EDX

			MUL EDI

			POP EDX

			ADD EDX, EAX	// EDX = BASE * 4 + ECX * 4 = (BASE + i) * 4

			//aggiunge elemento a vettore
			//* MOV Perm[EDX*4], Perm[EAX]
			MOV EAX, Perm[EDX]	

			//EBX = contatore posizione in vettore

			//* MOV Perm[EDX*4], Perm[EAX]
			MOV Perm[EBX], EAX

			//ripristina registri
			POP EAX

			//ciclo seguente
			ADD EBX, 4 //aggiorniamo il contatore tenendo conto della dimensione degli int
			INC ECX
			
			CMP EBX, EAX	//fine posizioni libere
			JE fine_aggiunta

			CMP ECX, N		//fine elementi da aggiungere
			JB ciclo_aggiunta
			//JMP fine_aggiunta

		fine_aggiunta:
			//ripristina registri
			POP EDX
			POP ECX
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