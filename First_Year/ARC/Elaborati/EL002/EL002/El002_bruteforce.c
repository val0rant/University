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

//int Perm[4326];

void main()
{
// Variabili
int N = 6;	// numero di interi (provare con valori <=6)
int Perm[4326];	// array permutazioni: la dimensione è sufficiente per N<=6
int Num; 	// Alla fine deve contenere il numero di permutazioni generate

// Blocco assembler
__asm 
{
	//* calcolo fattoriale di N (da salvare in Num)
	MOV EBX, N //EBX = N
	CALL caso_base_fattoriale		//EAX = N! = Num
	MOV Num, EAX //Num = fattoriale(N)
	//EAX = Num

	//* ESI = N * Num * 4 = BASE * 4 (BASE CONTATORE RIPETIZIONI)
	//EAX = Num
	MOV EBX, N
	MUL EBX //BASE = N * Num
	MOV EBX, 4
	MUL EBX //1 int = 4 bytes
	MOV ESI, EAX

	//ora ESI = BASE * 4 (scala)

	//* EDI = 0	(CONTATORE AGGIUNGI ELEMENTO) 
	MOV EAX, N
	MOV EBX, 4
	MUL EBX
	MOV EDI, EAX
	//EDI parte da N*4

	
	//* mette i primi N numeri all'inizio del vettore Perm, dopo tutte le possibili permutazioni
	XOR ECX, ECX //ECX parte da 0
	CALL inizializza_vettore_numeri

	//* azzero i registri
	XOR EAX, EAX
	XOR EBX, EBX
	XOR ECX, ECX
	XOR EDX, EDX

	MOV EBX, 0	//EBX = runner
	MOV EDX, 1	//EDX = mul

	//* inizializza contatore (123456)
	MOV ECX, N
	ciclo_inizializza_runner:
		MOV EAX, EDX	//EAX = mul
		PUSH EDX
		MUL ECX			//EAX = ECX * mul
		POP EDX			//EDX = mul
		
		ADD EBX, EAX

		PUSH EAX
		MOV EAX, 10
		////PUSH EDX
		MUL EDX
		////POP EDX
		MOV EDX, EAX	//EDX = EDX * 10
		POP EAX

		DEC ECX
		CMP ECX, 0
		JA ciclo_inizializza_runner

	//OK: EBX=runner

	//* inizializza numero max (654321)
	PUSH EBX	//mantiene EBX

	MOV EBX, EDX	//EBX = mul
	XOR EAX, EAX	//EAX = max
	MOV ECX, N
	//SEMBRA FUNZIONARE
	ciclo_inizializza_max:
		PUSH EAX

		MOV EAX, EBX
		PUSH ECX
		MOV ECX, 10
		XOR EDX, EDX
		DIV ECX			//EAX = mul / 10
		POP ECX
		MOV EBX, EAX
		MUL ECX			//EAX = mul * ECX
		MOV EDX, EAX

		POP EAX

		ADD EAX, EDX	//EAX = max + (mul * ECX)

		MOV EDX, EBX

		DEC ECX
		CMP ECX, 0
		JA ciclo_inizializza_max

	POP EBX		//ripristina EBX

	//? DOVREBBE FUNZIONARE, SE NON HO FATTO CASINI... RICONTROLLA DOMATTINA!!

	//EAX = max
	//EBX = runner
	//ECX = i
	//EDX = mul

	//! FINO A QUI I VALORI SONO CORRETTI, NON CONTROLLARE PIU': IL PROBLEMA E' DOPO

	//* CALCOLO PERMUTAZIONI (E PURE IN ORDINE :)
	INC EBX		//parti dalla seconda permutazione
	ciclo_scorri_elementi:
		//* mette N zeri nel vettore Perm, dopo tutte le possibili permutazioni
		CALL inizializza_vettore_contatori

		////JMP fine	//! DEBUG
		//EAX = dividend
		//EDX = remainder

		PUSH EAX
		PUSH ECX
		PUSH EDX

		MOV EAX, EBX	//dividend = runner
		XOR EDX, EDX	//remainder = 0	

		XOR ECX, ECX	//i = 0
		ciclo_per_ogni_cifra:
			PUSH ECX

			XOR EDX, EDX	//useful for division
			MOV ECX, 10
			DIV ECX			//remainder (EDX) = dividend % 10
							//dividend (EAX) /= 10
			//now EAX is quotient (next dividend) and EDX is remainder
			////POP ECX

			////PUSH ECX
			XOR ECX, ECX
			ciclo_controllo_cifre_giuste:
				INC ECX		//i+1
				CMP EDX, ECX
				//? non credo che cambi i flag
				JE cifra_corretta
				JMP continua

				cifra_corretta:
					DEC ECX

					PUSH EAX
					PUSH ECX
					PUSH EDX

					MOV EAX, ECX
					MOV ECX, 4
					MUL ECX
					ADD EAX, ESI
					ADD Perm[EAX], 1

					POP EDX
					POP ECX
					POP EAX

					MOV ECX, 10	//così smette di cercare inutilmente altre cifre

				continua:
					////INC ECX
					CMP ECX, N
					JB ciclo_controllo_cifre_giuste

			POP ECX

			INC ECX
			CMP ECX, N
			JB ciclo_per_ogni_cifra

		////POP EDX
		////POP ECX

		//EAX = product
		//EDX = multiplier
		PUSH EAX
		PUSH ECX
		PUSH EDX

		XOR ECX, ECX
		MOV EAX, 1
		
		//IF THE ERROR AIN'T IN THE DIGIT CHECKER, YOU'RE THE NEXT, EVEN THOUGH I GUESS YOU JUST WORK FINE
		ciclo_controlla_prodotto:
			PUSH ECX
			PUSH EAX

			MOV EAX, ECX

			

			MOV ECX, 4
			MUL ECX
			////POP ECX
			ADD EAX, ESI
			MOV ECX, Perm[EAX]

			POP EAX

			MUL ECX

			POP ECX

			INC ECX
			CMP ECX, N
			JB ciclo_controlla_prodotto

		CMP EAX, 1
		
		POP EDX
		POP ECX
		POP EAX

		JE permutazione_da_aggiungere
		JMP permutazione_successiva

		permutazione_da_aggiungere:
			//EAX is dividend
			CALL aggiungi_elemento

		permutazione_successiva:
			POP EDX
			POP ECX
			POP EAX

			INC EBX
			CMP EBX, EAX
			////JMP fine	//! DEBUG
			JBE ciclo_scorri_elementi

	//! IL BUG PUO' ESSERE SOLO IN 2 POSTI:
	//- CALCOLO PRODOTTO
	//* - INIZIALIZZAZIONE POSTI FINALI DEL VETTORE (CONTATORE)
	//OPPURE, HO SCAZZATO ALCUNI REGISTRI


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

	//! IL PROBLEMA E' DA QUI IN POI

	//* mette i primi N elementi all'inizio del vettore
	inizializza_vettore_numeri:	
		MOV EAX, ECX
		MOV EBX, 4
		MUL EBX //EAX = ECX * 4

		INC ECX

		MOV Perm[EAX], ECX //Perm[ECX * 4] = ECX + 1
		
		CMP ECX, N
		JB inizializza_vettore_numeri
		RET

	//* resetta tutti i contatori delle cifre a 0
	inizializza_vettore_contatori:
		PUSH EAX
		PUSH EBX
		PUSH ECX

		XOR ECX, ECX

		ciclo_inizializza_vettore_contatori:
			MOV EAX, ECX
			MOV EBX, 4
			PUSH EDX
			MUL EBX
			POP EDX
			ADD EAX, ESI	//EAX = ESI + ECX * 4

			MOV Perm[EAX], 0

			INC ECX
			CMP ECX, N
			JB ciclo_inizializza_vettore_contatori

		POP ECX
		POP EBX
		POP EAX
		RET

	//! GUESS HERE'S THE BUG
	//* aggiungi elemento all'array nella prima posizione libera
	aggiungi_elemento:
		//EAX is dividend
		PUSH ECX
		PUSH EDX

		//counter+=N
		PUSH EAX
		MOV EAX, N
		MOV ECX, 4
		MUL ECX			//EAX = N * 4
		ADD EDI, EAX	//EDI = EDI + N * 4
		POP EAX

		XOR EDX, EDX	//remainder = 0
		XOR ECX, ECX

		MOV EAX, EBX
		ciclo_aggiunta:

			PUSH EBX
			XOR EDX, EDX	//useful for division
			MOV EBX, 10
			DIV EBX
			POP EBX
			//EAX is quotient (new dividend)
			//EDX is remainder

			//counter--
			SUB EDI, 4

			//POP EDX
			MOV Perm[EDI], EDX
			//PUSH EDX

			INC ECX
			CMP ECX, N
			JB ciclo_aggiunta

		//counter+=N
		PUSH EAX
		MOV EAX, N
		MOV ECX, 4
		MUL ECX
		ADD EDI, EAX
		POP EAX

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