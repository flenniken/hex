
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "malloc.h"
#include "string.h"
#include "conio.h"
#include "assert.h"
#include "hex.h"

#define DEBUG 0
#define DISPLAYBOARD 0
#define DISPLAYREMOVE 0
#define STACKOVERFLOW 0

ULONG RemoveCount;

extern PIECE *Allpiece[12];   /* 12 pointers, where each is a pointer to an array of 
							placements of one piece. */
extern int Placements[12]; /* Number of placements for each piece. */
extern SQUARE *Square[60];
extern int SquareCount[60];
extern ULONG Bitsq[32];

BOARD Board;
ULONG over;

int P[12];			/* Position Allsquare (j). */
int Stacktop;
int Bitsquare;
clock_t Cstart;
int Solutions;

void main(void);
ULONG shift6(ULONG bit, int *low);
void DisplayPieceNum(ULONG bit, int low);
void findnextsquare(void);
void trynextsquare(int k);
void RemovePiece(PIECE *piece);
void DisplayBoard(FILE *file);
int findnumber(int square);


void main(void)
{
	int i;
	int j;
	int m;
	int k;
	int c;
#if DEBUG
	int sq;
	int n;
#endif

	Cstart = clock();

	CalculateBitsq();

	CalculateAllpiece();

	printf("Pieces calculated\n");

	CalculateSquare();

#if DEBUG
	for (sq = 0; sq < 60; sq++)
	{	
		int numbits;

		printf("square %d\n", sq);
		printf("elapsed time %4.2f sec\n", ((float)clock() - Cstart) / CLK_TCK); 
		if (sq == 1)
			exit(1);

		for (k = 0; k < SquareCount[sq]; k++)
		{
			i = Square[sq][k].i;
			j = Square[sq][k].j;

#if DEBUG
			assert(i >= 0 && i < 12);
#endif

			numbits = 0;
			for (n = 0; n < 4; n++)
			{
				if (Allpiece[i][j].u.bb[n] != 0)
				{
					for (m = 0; m < 8; m++)
						if (Allpiece[i][j].u.bb[n] & Bitsq[m])
							numbits++;
				}
			}
			if (numbits != 5)
			{
				DisplayPiece(&Allpiece[i][j], i);
				printf("\n");
				printf("error numbits %d\n", numbits);
				printf("9876 54321098 76543210 98765432 10987654 32109876 54321098 76543210\n");
				PrintPattern(&Allpiece[i][j]);
				printf("%d, %lx\n", Allpiece[i][j].location, Allpiece[i][j].u.base);
				printf("\n");
				exit(1);
			}
		}
	}
#endif

	printf("Squares calculated\n");

	for (c = 0; c < SquareCount[0]; c++)
	{
		i = Square[0][c].i;
		j = Square[0][c].j;

#if DEBUG
		assert(i >=0 && i < 12);
#endif

		printf("Square %d\n", c);

		for (m = 0; m < 12; m++)
			P[m] = -1;

		Board.u.hl.low = Board.u.hl.high = 0l;

		placepiece(&Allpiece[i][j]);

#if DEBUG
		assert(i < 12);
#endif
		P[i] = j;
		Stacktop = 1;

#if DISPLAYBOARD		
		DisplayBoard(NULL);
		printf("\n");
#endif

		Bitsquare = 1;
		findnextsquare();

		for (k = 0; k < SquareCount[Bitsquare]; k++)
		{
			m = Square[Bitsquare][k].i;
			if (P[m] == -1)
				trynextsquare(k);
			else	
				k += Square[Bitsquare][k].next-1;
		}
	}
	printf("Total time %4.2f sec\n", ((float)clock() - Cstart) / CLK_TCK); 
}

void trynextsquare(int kk)
{
	int i;
	int j;
	int m;
	int k;
//	PIECE *pPiece;

#if STACKOVERFLOW
	static int top = -1;
	if (top < Stacktop)
	{
		printf("Stacktop %d\n", Stacktop);
		top = Stacktop;
	}
#endif

	i = Square[Bitsquare][kk].i;
	j = Square[Bitsquare][kk].j;

#if DEBUG
	/* Check piece to see that it fits over this square. */
	if (!IsPieceOnSquare(&Allpiece[i][j], Bitsquare))
	{
		printf("i=%d, j=%d square=%d error in program\n", i, j, Bitsquare);
		DisplayPiece(&Allpiece[i][j], i);
		exit(1);
	}
#endif

	if (placepiece( &Allpiece[i][j] ))
	{
#if DEBUG
		assert(i < 12);
#endif
		P[i] = j;
		Stacktop++;

#if DISPLAYBOARD		
		DisplayBoard(NULL);
		printf("\n");
#endif

		if (Stacktop == 12)
		{
			FILE *file;
			if ((file = fopen("solution.out", "at+")) == NULL)
				exit(1);

			Solutions++;

			printf("Found solution %d\n", Solutions);
			printf("elapsed time %4.2f sec\n", ((float)clock() - Cstart) / CLK_TCK); 
			DisplayBoard(NULL);
			printf("\n");

			fprintf(file, "Found solution %d\n", Solutions);
			fprintf(file, "elapsed time %4.2f sec\n", ((float)clock() - Cstart) / CLK_TCK); 
			DisplayBoard(file);
			fprintf(file, "\n");

			fclose(file);
//			exit(1);
		}
		else
		{
			findnextsquare();

			for (k = 0; k < SquareCount[Bitsquare]; k++)
			{
				m = Square[Bitsquare][k].i;
				if (P[m] == -1)
					trynextsquare(k);
				else
					k += Square[Bitsquare][k].next-1;
			}
		}
#if DISPLAYREMOVE
		RemoveCount++;
		printf("Before remove %lu\n", RemoveCount);		
		DisplayBoard(NULL);
		printf("\n");
#endif
		/* remove piece */
		RemovePiece(&Allpiece[i][j]);

#if USEASM
		pPiece = &Allpiece[i][j];

	_asm
	{
		push si
		mov si, pPiece

		mov bl, [si+4] ; loc
		sub bh, bh

		lodsw				; piece->b0, piece->b1
		xor word ptr Board[bx], ax

		lodsw
		xor word ptr Board[bx+2], ax

		pop si
	}
#endif

		Stacktop--;
#if DEBUG
		assert(i < 12);
#endif
		P[i] = -1;

#if DISPLAYREMOVE
		printf("Remove %lu\n", RemoveCount);		
		DisplayBoard(NULL);
		printf("\n");
#endif

//		Bitsquare = 0;
//		findnextsquare();

		Bitsquare = Allpiece[i][j].lowestbit + 8 * Allpiece[i][j].location;
	}		
}

void findnextsquare(void)
{
	while (1)
	{
		if (Bitsquare < 32)
		{
			if (!(Board.u.hl.low & Bitsq[Bitsquare]))
				return;
		}
		else
		{
			if (!(Board.u.hl.high & Bitsq[Bitsquare-32]))
				return;
		}
		Bitsquare++;
	}
}

/*
Display the board.
*/
void DisplayBoard(FILE *file)
{
	int i, j;
	int sq;
	BOOLEAN found;
	int num;

	for (j = 5; j >= 0; j--)
	{
		for (i = 0; i < 10; i++)
		{
			sq = j + i * 6;
			if (sq < 32)
			{
				if (Board.u.hl.low & Bitsq[sq])
					found = TRUE;
				else
					found = FALSE;
			}
			else
			{
				if (Board.u.hl.high & Bitsq[sq-32])
					found = TRUE;
				else
					found = FALSE;
			}
			if (found)
			{
				num = findnumber(sq);
				if (num < 9)
				{
					if (file)
						fprintf(file, "%d", num+1);
					else
						printf("%d", num+1);
				}
				else	
				{
					if (file)
						fprintf(file, "%c", 'a' + num + 1 - 10);
					else
						printf("%c", 'a' + num + 1 - 10);
				}
			}
			else
				if (file)
					fprintf(file, "x");
				else
					printf("x");
		}	
		if (file)
			fprintf(file, "\n");
		else
			printf("\n");
	}	
}

int findnumber(int square)
{
	int i;
	int p;

	for (i = 0; i < 12; i++)
	{
		p = P[i];
		if (p != -1)
		{
			if (IsPieceOnSquare(&Allpiece[i][p], square))
				return i;
		}
	}
	printf("findnumber error i %d, j %d, square %d\n", i, p, square);
	exit(1);
}




